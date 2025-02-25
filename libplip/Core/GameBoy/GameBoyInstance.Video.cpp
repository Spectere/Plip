/* GameBoyInstance.Video.cpp
 *
 * Handles functions related to emulating the GameBoy's PPU.
 */

#include <cstring>

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::PPU_Cycle() {
    const auto currentLcdControl = m_ioRegisters->GetByte(IOReg_LcdControl);

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
            PPU_DotClock();
        }
    }

    m_ioRegisters->SetByte(IOReg_LcdYCoordinate, m_ppuLcdYCoordinate);
    m_ppuLastLcdControl = currentLcdControl;
}

void GameBoyInstance::PPU_DotClock() {
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
            performModeTransition = !PPU_DotClock_Output();
            break;
    }

    if(!performModeTransition) {
        ++m_ppuDotClock;
    } else {
        // Transition to the next video mode.
        PPU_VideoModeTransition();
        PPU_FinishTransition();
    }
}

bool GameBoyInstance::PPU_DotClock_OamScan() {
    // TODO: Sprite support. In the meantime, just kill time.
    return m_ppuDotClock < PPU_OamScanTime;
}

bool GameBoyInstance::PPU_DotClock_Output() {
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
            PPU_DotClock_Output_Drawing();
            ++m_ppuLcdXCoordinate;
            break;
        }
    }

    return m_ppuLcdXCoordinate < ScreenWidth;
}

void GameBoyInstance::PPU_DotClock_Output_Drawing() const {
    const auto pixelOffset = (m_ppuLcdYCoordinate * ScreenWidth) + m_ppuLcdXCoordinate;
    const auto lcdControl = m_ioRegisters->GetByte(IOReg_LcdControl);

    if(!BIT_TEST(lcdControl, 7)) {
        // The LCD should not be disabled here. Plot an error pixel.
        PPU_Plot(255, pixelOffset);
    }

    if(BIT_TEST(lcdControl, 0)) {
        // Background/Window drawing is enabled.
        const auto backgroundPalette = m_ioRegisters->GetByte(IOReg_BGPalette);
        const auto scrollY = m_ioRegisters->GetByte(IOReg_ScrollY);

        const auto tileDataAddrOffset = (BIT_TEST(lcdControl, 4)) ? 0 : PPU_TileBaseBlockOffset;
        const auto tileDataAddr = PPU_TileBase + tileDataAddrOffset;

        const auto tileMapAddrOffset = (BIT_TEST(lcdControl, 3)) ? PPU_TileMapBlockOffset : 0;
        const auto tileMapAddr = PPU_TileMapBase + tileMapAddrOffset;

        const auto tileX = ((m_ppuScrollX + m_ppuLcdXCoordinate) / PPU_TileSizeX) % PPU_MapTileCountX;
        const auto tileY = ((scrollY + m_ppuLcdYCoordinate) / PPU_TileSizeY) % PPU_MapTileCountY;

        const auto tilePixelX = (m_ppuScrollX + m_ppuLcdXCoordinate) % PPU_TileSizeX;
        const auto tilePixelY = (scrollY + m_ppuLcdYCoordinate) % PPU_TileSizeY;

        const auto mapIndex = (tileY * PPU_MapTileCountX) + tileX;
        const auto tileIndex = m_videoRam->GetByte(tileMapAddr + mapIndex);

        const auto lineOffset = tilePixelY * 2;

        const auto pixelDataLow = m_videoRam->GetByte(tileDataAddr + (tileIndex * 16) + lineOffset, true);
        const auto pixelDataHigh = m_videoRam->GetByte(tileDataAddr + (tileIndex * 16) + lineOffset + 1, true);
        const auto tileShift = 7 - tilePixelX;
        const auto pixelData = (((pixelDataHigh >> tileShift) & 0b1) << 1)
                             | ((pixelDataLow >> tileShift) & 0b1);
        const auto pixelColor = (backgroundPalette >> (pixelData * 2)) & 0b11;

        PPU_Plot(pixelColor, pixelOffset);
    } else {
        // Background/Window drawing is disabled.
        PPU_Plot(0b00, pixelOffset);
    }
}

void GameBoyInstance::PPU_FinishTransition() {
    const uint8_t lcdStatus = m_ioRegisters->GetByte(IOReg_LcdStatus);
    PPU_SetMemoryPermissions();
    m_ppuDotClock = 0;

    switch(m_ppuMode) {
        case PPU_Mode::HBlank:
            if(BIT_TEST(lcdStatus, 3)) {
                RaiseInterrupt(Cpu::SharpLr35902Interrupt::Lcd);
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
            m_ppuScrollX = m_ioRegisters->GetByte(IOReg_ScrollX);
            break;
    }
}

void GameBoyInstance::PPU_FinishTransition_OamScan(const uint8_t lcdStatus) const {
    const auto lcdYComparison = m_ppuLcdYCoordinate == m_ioRegisters->GetByte(IOReg_LcdYCompare);

    if(BIT_TEST(lcdStatus, 5)) {
        // OAM interrupt.
        RaiseInterrupt(Cpu::SharpLr35902Interrupt::Lcd);
    }

    if(lcdYComparison) {
        // LYC == LY interrupt.
        RaiseInterrupt(Cpu::SharpLr35902Interrupt::Lcd);
        m_ioRegisters->SetByte(IOReg_LcdStatus, BIT_SET(lcdStatus, 2));
    }
}

void GameBoyInstance::PPU_FinishTransition_VBlank(const uint8_t lcdStatus) {
    // Send the contents of the screen to the emulator frontend.
    m_video->BeginDraw();
    m_video->Draw(m_videoBuffer);
    m_video->EndDraw();

    m_ppuLcdOff = false;

    RaiseInterrupt(Cpu::SharpLr35902Interrupt::VBlank);
    if(BIT_TEST(lcdStatus, 4)) {
        RaiseInterrupt(Cpu::SharpLr35902Interrupt::Lcd);
    }
}

std::map<std::string, Plip::DebugValue> GameBoyInstance::PPU_GetDebugInfo() const {
    return {
        { "LCDC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IOReg_LcdControl))) },
        { "LY", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IOReg_LcdYCoordinate))) },
        { "LYC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IOReg_LcdYCompare))) },
        { "STAT", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IOReg_LcdStatus))) },
        { "SCX", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IOReg_ScrollX))) },
        { "SCY", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IOReg_ScrollY))) },
        { "WX", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IOReg_WindowX))) },
        { "WY", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IOReg_WindowY))) },
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

