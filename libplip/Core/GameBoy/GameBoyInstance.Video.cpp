/* GameBoyInstance.Video.cpp
 *
 * Handles functions related to emulating the Game Boy's PPU.
 */

#include <cstring>

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::PPU_Cycle() {
    const auto currentLcdControl = m_ioRegisters->GetByte(IoRegister::LcdControl);
    const auto currentLcdStatus = m_ioRegisters->GetByte(IoRegister::LcdStatus);

    if(BIT_TEST(m_ppuLastLcdControl, 7) && !BIT_TEST(currentLcdControl, 7)) {
        // LCD was disabled during this cycle. Flag all memory as writable and blank the screen.
        m_oam->SetWritable(true);
        m_videoRam->SetWritable(true);

        memset(m_videoBuffer, 0xFF, m_videoBufferSize);
        m_video->BeginDraw();
        m_video->Draw(m_videoBuffer);
        m_video->EndDraw();
    } else if(!BIT_TEST(m_ppuLastLcdControl, 7) && BIT_TEST(currentLcdControl, 7)) {
        // LCD was enabled during this cycle. Reset VRAM permissions and signal for the
        // screen to be drawn during the next frame.
        PPU_SetMemoryPermissions();
        m_ppuLcdOff = true;
    }

    if(BIT_TEST(currentLcdControl, 7)) {
        // Run 4 dot clock cycles per CPU cycle.
        for(auto dotCycle = 0; dotCycle < PPU_DotsPerCycle; dotCycle++) {
            PPU_DotClock(currentLcdControl, currentLcdStatus);
        }
    }

    m_ioRegisters->Video_SetYCoordinate(m_ppuLcdYCoordinate);
    m_ppuLastLcdControl = currentLcdControl;

    // Update the LCD STAT register.
    m_ioRegisters->Video_SetLcdStatus((m_ppuLyc ? 0b100 : 0) | static_cast<uint8_t>(m_ppuMode));
}

void GameBoyInstance::PPU_DotClock(const uint8_t lcdControl, const uint8_t lcdStatus) {
    bool performModeTransition = false;

    switch(m_ppuMode) {
        case PPU_Mode::HBlank:
            performModeTransition = m_ppuDotClock >= PPU_ScanlineTime;
            break;
        case PPU_Mode::VBlank:
            if(m_ppuDotClock > 0) {
                if(m_ppuDotClock % (PPU_OamScanTime + PPU_ScanlineTime) == 0) {
                    ++m_ppuLcdYCoordinate;
                }
            }

            performModeTransition = m_ppuDotClock >= PPU_FrameTime;
            break;
        case PPU_Mode::OamScan:
            performModeTransition = !PPU_DotClock_OamScan();
            break;
        case PPU_Mode::Output:
            performModeTransition = !PPU_DotClock_Output(lcdControl);
            break;
    }

    if(!performModeTransition) {
        ++m_ppuDotClock;
    } else {
        // Transition to the next video mode.
        PPU_VideoModeTransition();
        PPU_FinishTransition(lcdStatus);
    }
}

bool GameBoyInstance::PPU_DotClock_OamScan() {
    // TODO: Sprite support. In the meantime, just kill time.
    return m_ppuDotClock < PPU_OamScanTime;
}

bool GameBoyInstance::PPU_DotClock_Output(const uint8_t lcdControl) {
    // The exact number of clocks this takes depends on the SCX register, so we need
    // this to handle its own timing. This should only indicate that it's ready to
    // transition to the next PPU mode when it's plotted the number of horizontal
    // pixels on the system's screen (160).
    switch(m_ppuOutputStage) {
        case PPU_OutputStage::BackgroundScrolling: {
            if(++m_ppuOutputClock > (m_ppuScrollX & 0b111)) {
                m_ppuOutputClock = 0;
                m_ppuOutputStage = PPU_OutputStage::Drawing;
            }
            break;
        }

        case PPU_OutputStage::Drawing: {
            PPU_DotClock_Output_Drawing(lcdControl);
            ++m_ppuLcdXCoordinate;
            break;
        }
    }

    return m_ppuLcdXCoordinate < ScreenWidth;
}

void GameBoyInstance::PPU_DotClock_Output_Drawing(const uint8_t lcdControl) const {
    const uint32_t pixelOffset = (m_ppuLcdYCoordinate * ScreenWidth) + m_ppuLcdXCoordinate;

    if(!BIT_TEST(lcdControl, 7)) {
        // The LCD should not be disabled here. Plot an error pixel.
        PPU_Plot(255, pixelOffset);
    }

    if(BIT_TEST(lcdControl, 0)) {
        // Background/Window drawing is enabled.
        const auto backgroundPalette = m_ioRegisters->GetByte(IoRegister::BgPalette);
        const auto scrollY = m_ioRegisters->GetByte(IoRegister::ScrollY);
        
        const auto backgroundTileMapAddress = PPU_TileMapBase + (BIT_TEST(lcdControl, 3) ? PPU_TileMapBlockOffset : 0);

        const auto tilesUseBlock2 = (BIT_TEST(lcdControl, 4)) == 0;
        const auto tileDataAddressLow = PPU_TileBase + (tilesUseBlock2 ? 0x1000 : 0);
        constexpr uint16_t tileDataAddressHigh = PPU_TileBase + 0x800;

        PPU_DrawBackgroundOrWindow(pixelOffset, false, backgroundPalette, m_ppuScrollX, scrollY, backgroundTileMapAddress, tileDataAddressLow, tileDataAddressHigh);

        if(BIT_TEST(lcdControl, 5)) {
            // Window drawing is enabled.
            const auto windowX = m_ioRegisters->GetByte(IoRegister::WindowX) - 7;
            const auto windowY = m_ioRegisters->GetByte(IoRegister::WindowY);

            const auto windowTileMapAddress = PPU_TileMapBase + (BIT_TEST(lcdControl, 6) ? PPU_TileMapBlockOffset : 0);

            PPU_DrawBackgroundOrWindow(pixelOffset, true, backgroundPalette, windowX, windowY, windowTileMapAddress, tileDataAddressLow, tileDataAddressHigh);
        }
    } else {
        // Background/Window drawing is disabled.
        PPU_Plot(0b00, pixelOffset);
    }
}

void GameBoyInstance::PPU_DrawBackgroundOrWindow(const uint32_t pixelOffset, const bool isWindow, const uint8_t palette, const uint8_t offsetX, const uint8_t offsetY, const uint16_t tileMapAddress, const uint16_t tileDataAddress0, const uint16_t tileDataAddress1) const {
    uint8_t surfacePixelX {};
    uint8_t surfacePixelY {};

    if(isWindow) {
        if(offsetX > 166 || offsetY > 143) { return; }  // Window is off screen.
        if(m_ppuLcdXCoordinate < offsetX || m_ppuLcdYCoordinate < offsetY) { return; }  // Current pixel is up/left of window.

        surfacePixelX = m_ppuLcdXCoordinate - offsetX;
        surfacePixelY = m_ppuLcdYCoordinate - offsetY;
    } else {
        surfacePixelX = m_ppuLcdXCoordinate + offsetX;
        surfacePixelY = m_ppuLcdYCoordinate + offsetY;
    }
    
    const auto tileX = (surfacePixelX / PPU_TileSizeX) % PPU_MapTileCountX;
    const auto tileY = (surfacePixelY / PPU_TileSizeY) % PPU_MapTileCountY;

    const auto tilePixelX = surfacePixelX % PPU_TileSizeX;
    const auto tilePixelY = surfacePixelY % PPU_TileSizeY;

    const auto mapIndex = (tileY * PPU_MapTileCountX) + tileX;
    const auto tileIndex = m_videoRam->GetByte(tileMapAddress + mapIndex);

    const auto lineOffset = tilePixelY * 2;

    const auto tileDataAddress = (tileIndex < 128) ? tileDataAddress0 : tileDataAddress1; 
    const auto pixelDataLow = m_videoRam->GetByte(tileDataAddress + (tileIndex % 128 * 16) + lineOffset, true);
    const auto pixelDataHigh = m_videoRam->GetByte(tileDataAddress + (tileIndex % 128 * 16) + lineOffset + 1, true);
    const auto tileShift = 7 - tilePixelX;
    const auto pixelData = (((pixelDataHigh >> tileShift) & 0b1) << 1)
                         | ((pixelDataLow >> tileShift) & 0b1);
    const auto pixelColor = (palette >> (pixelData * 2)) & 0b11;

    PPU_Plot(pixelColor, pixelOffset);
}

void GameBoyInstance::PPU_FinishTransition(const uint8_t lcdStatus) {
    PPU_SetMemoryPermissions();
    m_ppuDotClock = 0;

    switch(m_ppuMode) {
        case PPU_Mode::HBlank:
            if(BIT_TEST(lcdStatus, 3)) {
                m_ioRegisters->RaiseInterrupt(Cpu::SharpLr35902Interrupt::Lcd);
            }
            break;

        case PPU_Mode::VBlank:
            PPU_FinishTransition_VBlank(lcdStatus);
            break;

        case PPU_Mode::OamScan:
            PPU_FinishTransition_OamScan(lcdStatus);
            break;

        case PPU_Mode::Output:
            m_ppuOutputStage = PPU_OutputStage::BackgroundScrolling;
            m_ppuScrollX = m_ioRegisters->GetByte(IoRegister::ScrollX);
            break;
    }
}

void GameBoyInstance::PPU_FinishTransition_OamScan(const uint8_t lcdStatus) {
    m_ppuLyc = m_ppuLcdYCoordinate == m_ioRegisters->GetByte(IoRegister::LcdYCompare);

    if(BIT_TEST(lcdStatus, 5)) {
        // OAM interrupt.
        m_ioRegisters->RaiseInterrupt(Cpu::SharpLr35902Interrupt::Lcd);
    }

    if(BIT_TEST(lcdStatus, 6) && m_ppuLyc) {
        // LYC == LY interrupt.
        m_ioRegisters->RaiseInterrupt(Cpu::SharpLr35902Interrupt::Lcd);
    }
}

void GameBoyInstance::PPU_FinishTransition_VBlank(const uint8_t lcdStatus) {
    // Send the contents of the screen to the emulator frontend.
    m_video->BeginDraw();
    m_video->Draw(m_videoBuffer);
    m_video->EndDraw();

    m_ppuLcdOff = false;

    m_ioRegisters->RaiseInterrupt(Cpu::SharpLr35902Interrupt::VBlank);
    if(BIT_TEST(lcdStatus, 4)) {
        m_ioRegisters->RaiseInterrupt(Cpu::SharpLr35902Interrupt::Lcd);
    }
}

std::map<std::string, Plip::DebugValue> GameBoyInstance::PPU_GetDebugInfo() const {
    return {
        { "LCDC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::LcdControl))) },
        { "LY", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::LcdYCoordinate))) },
        { "LYC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::LcdYCompare))) },
        { "STAT", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::LcdStatus))) },
        { "SCX", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::ScrollX))) },
        { "SCY", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::ScrollY))) },
        { "WX", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::WindowX))) },
        { "WY", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::WindowY))) },
        { "BGP", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::BgPalette))) },
        { "Clock", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ppuDotClock)) },
        { "Mode", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ppuMode)) },
    };
}

void GameBoyInstance::PPU_Plot(int color, const int pos) const {
    if(m_ppuLcdOff) color = 0b00;

    switch(color) {
        case 0b00:  // White
            m_videoFormat.plot(m_videoBuffer, pos, 255, 255, 255);
            break;

        case 0b01:  // Light Grey
            m_videoFormat.plot(m_videoBuffer, pos, 172, 172, 172);
            break;

        case 0b10:  // Dark Gray
            m_videoFormat.plot(m_videoBuffer, pos, 86, 86, 86);
            break;

        case 0b11:  // Black
            m_videoFormat.plot(m_videoBuffer, pos, 0, 0, 0);
            break;

        default:  // wtf? Red, I guess?
            m_videoFormat.plot(m_videoBuffer, pos, 255, 0, 0);
            break;
    }
}

void GameBoyInstance::PPU_Reset() {
    // Paint the framebuffer white.
    memset(m_videoBuffer, 0xFF, m_videoBufferSize);

    // Reset PPU variables.
    m_ppuDotClock = 0;
    m_ppuDrawTime = 0;
    m_ppuLastLcdControl = 0;
    m_ppuLcdOff = false;
    m_ppuLyc = false;
    m_ppuMode = PPU_Mode::OamScan;
    m_ppuLcdXCoordinate = 0;
    m_ppuLcdYCoordinate = 0;
}

void GameBoyInstance::PPU_SetMemoryPermissions() const {
    switch(m_ppuMode) {
        case PPU_Mode::HBlank:
        case PPU_Mode::VBlank:
            m_oam->SetWritable(true);
            m_videoRam->SetWritable(true);
            break;
        case PPU_Mode::OamScan:
            m_oam->SetWritable(false);
            m_videoRam->SetWritable(true);
            break;
        case PPU_Mode::Output:
            m_oam->SetWritable(false);
            m_videoRam->SetWritable(false);
            break;
    }
}

void GameBoyInstance::PPU_VideoModeTransition() {
    switch(m_ppuMode) {
        case PPU_Mode::HBlank:
            m_ppuLcdXCoordinate = 0;
            ++m_ppuLcdYCoordinate;

            m_ppuMode = m_ppuLcdYCoordinate < ScreenHeight
                ? PPU_Mode::OamScan
                : PPU_Mode::VBlank;
            break;

        case PPU_Mode::VBlank:
            m_ppuLcdYCoordinate = 0;
            m_ppuMode = PPU_Mode::OamScan;
            break;

        case PPU_Mode::OamScan:
            m_ppuMode = PPU_Mode::Output;
            break;

        case PPU_Mode::Output:
            m_ppuMode = PPU_Mode::HBlank;
            break;
    }
}

