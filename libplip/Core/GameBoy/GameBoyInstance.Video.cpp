/* GameBoyInstance.Video.cpp
 *
 * Handles functions related to emulating the Game Boy's PPU.
 */

#include <algorithm>
#include <cstring>

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::PPU_Cycle() {
    const auto currentLcdControl = m_ioRegisters->GetByte(IoRegister::LcdControl);
    const auto currentLcdStatus = m_ioRegisters->GetByte(IoRegister::LcdStatus);

    if(BIT_TEST(m_ppuLastLcdControl, 7) && !BIT_TEST(currentLcdControl, 7)) {
        // LCD was disabled during this cycle. Flag all memory as writable, blank the screen, and reset the PPU mode
        // to 0 (HBlank).
        m_oam->SetReadable(true);
        m_oam->SetWritable(true);
        m_videoRam->SetReadable(true);
        m_videoRam->SetWritable(true);

        memset(m_videoBuffer, 0xFF, m_videoBufferSize);
        m_video->BeginDraw();
        m_video->Draw(m_videoBuffer);
        m_video->EndDraw();

        m_ppuMode = PPU_Mode::HBlank;
    } else if(!BIT_TEST(m_ppuLastLcdControl, 7) && BIT_TEST(currentLcdControl, 7)) {
        // LCD was enabled during this cycle. Reset VRAM permissions and signal for the
        // screen to be drawn during the next frame.
        PPU_SetMemoryPermissions();
        m_ppuLcdOff = true;
    }

    if(BIT_TEST(currentLcdControl, 7)) {
        // Run 2/4 dot clock cycles per CPU cycle.
        const auto dotClocks = m_doubleSpeed ? PPU_DotsPerCycleHighSpeed : PPU_DotsPerCycleLowSpeed; 
        for(auto dotCycle = 0; dotCycle < dotClocks; dotCycle++) {
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
    if(!m_ppuOamScanComplete) {
        std::vector<PPU_Object> candidates;

        const auto lcdControl = m_ioRegisters->GetByte(IoRegister::LcdControl);
        const bool tallSprites = BIT_TEST(lcdControl, 2);
    
        // Clear draw list array.
        m_ppuObjectDrawList.clear();

        // Determine drawing candidates for this scanline.
        for(auto i = 0; i < PPU_MaximumObjectCount; ++i) {
            const auto y = m_oam->GetByte(i * 4, true);

            const auto minY = y - 16;
            const auto maxY = minY + (tallSprites ? 16 : 8);

            if((minY <= m_ppuLcdYCoordinate) && (m_ppuLcdYCoordinate < maxY)) {
                // Object is a valid candidate.
                candidates.push_back(PPU_Object {
                    .Y = y,
                    .X = m_oam->GetByte(i * 4 + 1, true),
                    .Index = m_oam->GetByte(i * 4 + 2, true),
                    .Flags = m_oam->GetByte(i * 4 + 3, true)
                });
            }
        }

        // Copy up to PPU_ObjectsPerScanline objects into the drawing list.
        int objectsCopied = 0;
        for(auto candidate : candidates) {
            m_ppuObjectDrawList.push_back(candidate);
            if(++objectsCopied == PPU_ObjectsPerScanline) {
                break;
            }
        }

        // DMG prioritizes candidates with a lower X position.
        if(!m_cgbMode) {
            std::sort(m_ppuObjectDrawList.begin(), m_ppuObjectDrawList.end(), [](const PPU_Object lhs, const PPU_Object rhs) {
                return lhs.X < rhs.X;
            });
        }

        // Done!
        m_ppuOamScanComplete = true;
    }
    
    return m_ppuDotClock < PPU_OamScanTime;
}

bool GameBoyInstance::PPU_DotClock_Output(const uint8_t lcdControl) {
    // The exact number of clocks this takes depends on a number of factors and can range from
    // 172 to 289.

    // When the window is enabled, a 6-dot penalty is incurred while the BG fetcher is being set
    // up for the window layer.
    m_ppuWindowEnabled = BIT_TEST(lcdControl, 5);
    if(m_ppuWindowEnabled) {
        m_ppuWindowX = m_ioRegisters->GetByte(IoRegister::WindowX) - 7;
        m_ppuWindowY = m_ioRegisters->GetByte(IoRegister::WindowY);

        if(!m_ppuWindowSetUp && m_ppuLcdXCoordinate >= m_ppuWindowX && m_ppuLcdYCoordinate >= m_ppuWindowY) {
            m_ppuWindowSetUp = true;
            m_ppuOutputClock = 6;  // 6 dot penalty
            m_ppuOutputStage = PPU_OutputStage::WindowPreparation;
        }
    }
    
    switch(m_ppuOutputStage) {
        case PPU_OutputStage::BackgroundScrolling: {
            // Background scrolling causes a delay of (SCX % 8) dot clocks.
            if(++m_ppuOutputClock > (m_ppuScrollX & 0b111)) {
                m_ppuOutputClock = 0;
                m_ppuOutputStage = PPU_OutputStage::Drawing;
            }
            break;
        }

        case PPU_OutputStage::WindowPreparation:
        case PPU_OutputStage::ObjectPenalty: {
            if(--m_ppuOutputClock == 0) {
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

void GameBoyInstance::PPU_DotClock_Output_Drawing(const uint8_t lcdControl) {
    const uint32_t pixelOffset = (m_ppuLcdYCoordinate * ScreenWidth) + m_ppuLcdXCoordinate;

    if(!BIT_TEST(lcdControl, 7)) {
        // The LCD should not be disabled here. Plot an error pixel.
        PPU_Plot_DMG(255, pixelOffset);
    }

    int lastBgColor = 0;
    const bool lcdcEnabledOrPriority = BIT_TEST(lcdControl, 0);
    if(lcdcEnabledOrPriority || m_cgbMode) {
        // Background/Window drawing is enabled (or we're in CGB mode).
        const auto backgroundPalette = m_ioRegisters->GetByte(IoRegister::BgPalette);
        const auto scrollY = m_ioRegisters->GetByte(IoRegister::ScrollY);
        
        const auto backgroundTileMapAddress = PPU_TileMapBase + (BIT_TEST(lcdControl, 3) ? PPU_TileMapBlockOffset : 0);

        const auto tilesUseBlock2 = (BIT_TEST(lcdControl, 4)) == 0;
        const auto tileDataAddressLow = PPU_TileBase + (tilesUseBlock2 ? 0x1000 : 0);
        constexpr uint16_t tileDataAddressHigh = PPU_TileBase + 0x800;

        lastBgColor = PPU_DrawBackgroundOrWindow(pixelOffset, false, backgroundPalette, m_ppuScrollX, scrollY, backgroundTileMapAddress, tileDataAddressLow, tileDataAddressHigh, lcdcEnabledOrPriority);

        if(m_ppuWindowEnabled) {
            // Window drawing is enabled.
            const auto windowTileMapAddress = PPU_TileMapBase + (BIT_TEST(lcdControl, 6) ? PPU_TileMapBlockOffset : 0);

            const auto windowColor = PPU_DrawBackgroundOrWindow(pixelOffset, true, backgroundPalette, m_ppuWindowX, m_ppuWindowY, windowTileMapAddress, tileDataAddressLow, tileDataAddressHigh, lcdcEnabledOrPriority);
            if(windowColor >= 0) lastBgColor = windowColor;
        }
    } else {
        // Background/Window drawing is disabled.
        PPU_Plot_DMG(0b00, pixelOffset);
    }

    if(BIT_TEST(lcdControl, 1)) {
        // Object drawing is enabled and can potentially have priority.
        for(const auto object : m_ppuObjectDrawList) {
            if(PPU_DrawObject(pixelOffset, object, BIT_TEST(lcdControl, 2), lastBgColor, lcdcEnabledOrPriority)) {
                break;  // An object pixel has already been considered for this position.
            }
        }
    }
}

int GameBoyInstance::PPU_DrawBackgroundOrWindow(const uint32_t pixelOffset, const bool isWindow, const uint8_t palette, const int offsetX, const int offsetY, const uint16_t tileMapAddress, const uint16_t tileDataAddress0, const uint16_t tileDataAddress1, const bool lcdcPriority) const {
    uint8_t surfacePixelX {};
    uint8_t surfacePixelY {};
    
    if(isWindow) {
        if(offsetX > 166 || offsetY > 143) { return -1; }  // Window is off-screen.
        if(m_ppuLcdXCoordinate < offsetX || m_ppuLcdYCoordinate < offsetY) { return -1; }  // Current pixel is up/left of window.

        surfacePixelX = m_ppuLcdXCoordinate - offsetX;
        surfacePixelY = m_ppuLcdYCoordinate - offsetY;
    } else {
        surfacePixelX = m_ppuLcdXCoordinate + offsetX;
        surfacePixelY = m_ppuLcdYCoordinate + offsetY;
    }
    
    const auto tileX = (surfacePixelX / PPU_TileSizeX) % PPU_MapTileCountX;
    const auto tileY = (surfacePixelY / PPU_TileSizeY) % PPU_MapTileCountY;
    const auto mapIndex = (tileY * PPU_MapTileCountX) + tileX;

    bool priority = false;
    bool flipX = false;
    bool flipY = false;
    bool bank1 = false;
    int cgbPalette = 0;
    if(m_cgbMode) {
        const auto cgbBgMapAttributes = m_videoRam->GetByte(GameBoyMapper::VideoRamLength + tileMapAddress + mapIndex, true);
        priority = BIT_TEST(cgbBgMapAttributes, 7);
        flipY = BIT_TEST(cgbBgMapAttributes, 6);
        flipX = BIT_TEST(cgbBgMapAttributes, 5);
        bank1 = BIT_TEST(cgbBgMapAttributes, 3);
        cgbPalette = cgbBgMapAttributes & 0b111;
    }

    auto tilePixelX = surfacePixelX % PPU_TileSizeX;
    auto tilePixelY = surfacePixelY % PPU_TileSizeY;

    if(flipX) {
        tilePixelX = 7 - tilePixelX;
    }

    if(flipY) {
        tilePixelY = 7 - tilePixelY;
    }

    const auto tileIndex = m_videoRam->GetByte(tileMapAddress + mapIndex, true);
    
    const auto lineOffset = tilePixelY * 2;

    const auto tileDataAddress = (tileIndex < 128) ? tileDataAddress0 : tileDataAddress1 + (bank1 ? GameBoyMapper::VideoRamLength : 0); 
    const auto pixelDataLow = m_videoRam->GetByte(tileDataAddress + (tileIndex % 128 * 16) + lineOffset, true);
    const auto pixelDataHigh = m_videoRam->GetByte(tileDataAddress + (tileIndex % 128 * 16) + lineOffset + 1, true);
    const auto tileShift = 7 - tilePixelX;
    const auto pixelData = (((pixelDataHigh >> tileShift) & 0b1) << 1)
                         | ((pixelDataLow >> tileShift) & 0b1);
    const auto pixelColor = (palette >> (pixelData * 2)) & 0b11;
    
    if(m_model == GameBoyModel::DMG) {
        PPU_Plot_DMG(pixelColor, pixelOffset);
        return pixelColor;
    }
    
    PPU_Plot_CGB(false, cgbPalette, m_cgbMode ? pixelData : pixelColor, pixelOffset);
    return pixelData | (priority ? (1 << 7) : 0);
}

bool GameBoyInstance::PPU_DrawObject(const uint32_t pixelOffset, const PPU_Object object, const bool tallSprites, const int thisBgColor, const bool lcdcPriority) const {
    // TODO: Emulate object dot clock penalties. :(
    const auto objX = object.X - 8;
    const auto objY = object.Y - 16;
    
    if((m_ppuLcdXCoordinate < objX) || (m_ppuLcdXCoordinate >= objX + 8)) {
        // Sprite does not exist on the current X coordinate.
        return false;
    }

    // Handle BG/OBJ priorities.
    if(!m_cgbMode) {
        if(BIT_TEST(object.Flags, 7) && (thisBgColor & 0b11)) return true;
    } else {
        const auto bgColor = thisBgColor & 0b11;
        const bool bgPriority = BIT_TEST(thisBgColor, 7);
        if(lcdcPriority && (BIT_TEST(object.Flags, 7) || bgPriority) && bgColor) return true;
    }

    auto objPixelX = m_ppuLcdXCoordinate - objX;
    auto objPixelY = m_ppuLcdYCoordinate - objY;
    const auto cgbPalette = m_cgbMode ? object.Flags & 0b111 : (BIT_TEST(object.Flags, 4) ? 1 : 0);

    auto tileIndex = object.Index;
    if(tallSprites) {
        // The base tile for 8x16 sprites always has its LSB set to 0.
        tileIndex &= 0b11111110;
    }

    if(BIT_TEST(object.Flags, 5)) {
        // Flip horizontally.
        objPixelX = 7 - objPixelX;
    }

    if(BIT_TEST(object.Flags, 6)) {
        // Flip vertically.
        objPixelY = (tallSprites ? 15 : 7) - objPixelY;
    }
    
    const auto lineOffset = objPixelY * 2;
    const auto pixelDataLow = m_videoRam->GetByte(PPU_TileBase + (tileIndex * 16) + lineOffset, true);
    const auto pixelDataHigh = m_videoRam->GetByte(PPU_TileBase + (tileIndex * 16) + lineOffset + 1, true);
    const auto objShift = 7 - objPixelX;
    const auto pixelData = (((pixelDataHigh >> objShift) & 0b1) << 1)
                         | ((pixelDataLow >> objShift) & 0b1);
    if(pixelData == 0) return false;  // Color 0b00 is always transparent.
    
    const auto objPalette = m_ioRegisters->GetByte(BIT_TEST(object.Flags, 4) ? IoRegister::Obj1Palette : IoRegister::Obj0Palette);
    const auto pixelColor = (objPalette >> (pixelData * 2)) & 0b11;

    // Finally, draw the point!
    if(m_model == GameBoyModel::DMG) {
        PPU_Plot_DMG(pixelColor, pixelOffset);
    } else {
        PPU_Plot_CGB(true, cgbPalette, m_cgbMode ? pixelData : pixelColor, pixelOffset);
    }
    return true;
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
            m_ppuWindowSetUp = false;
            break;
    }
}

void GameBoyInstance::PPU_FinishTransition_OamScan(const uint8_t lcdStatus) {
    m_ppuLyc = m_ppuLcdYCoordinate == m_ioRegisters->GetByte(IoRegister::LcdYCompare);
    m_ppuOamScanComplete = false;

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

void GameBoyInstance::PPU_Plot_CGB(const bool objPalette, const int palette, const int color, const int pos) const {
    if(m_ppuLcdOff) {
        m_videoFormat.plot(m_videoBuffer, pos, 0, 0, 0);
        return;
    }

    const auto paletteRam = objPalette ? m_ppuCgbObjPaletteRam : m_ppuCgbBgPaletteRam;
    const auto index = (((palette & 0b111) * 4) + (color & 0b11)) * 2;
    const uint16_t colorDefinition = (paletteRam->GetByte(index + 1, true) << 8)
                                   | paletteRam->GetByte(index, true);
    const uint8_t red = (colorDefinition & 0b11111) * 8;
    const uint8_t green = ((colorDefinition & 0b11111 << 5) >> 5) * 8;
    const uint8_t blue = ((colorDefinition & 0b11111 << 10) >> 10) * 8;
    m_videoFormat.plot(m_videoBuffer, pos, red, green, blue);
}

void GameBoyInstance::PPU_Plot_DMG(int color, const int pos) const {
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
        case PPU_Mode::VBlank: {
            m_oam->SetReadable(true);
            m_oam->SetWritable(true);
            m_videoRam->SetReadable(true);
            m_videoRam->SetWritable(true);
            m_ppuCgbBgPaletteRam->SetReadable(true);
            m_ppuCgbBgPaletteRam->SetWritable(true);
            m_ppuCgbObjPaletteRam->SetReadable(true);
            m_ppuCgbObjPaletteRam->SetWritable(true);
            break;
        }
        case PPU_Mode::OamScan: {
            m_oam->SetReadable(false);
            m_oam->SetWritable(false);
            m_videoRam->SetReadable(true);
            m_videoRam->SetWritable(true);
            m_ppuCgbBgPaletteRam->SetReadable(true);
            m_ppuCgbBgPaletteRam->SetWritable(true);
            m_ppuCgbObjPaletteRam->SetReadable(true);
            m_ppuCgbObjPaletteRam->SetWritable(true);
            break;
        }
        case PPU_Mode::Output: {
            m_oam->SetReadable(false);
            m_oam->SetWritable(false);
            m_videoRam->SetReadable(false);
            m_videoRam->SetWritable(false);
            m_ppuCgbBgPaletteRam->SetReadable(false);
            m_ppuCgbBgPaletteRam->SetWritable(false);
            m_ppuCgbObjPaletteRam->SetReadable(false);
            m_ppuCgbObjPaletteRam->SetWritable(false);
            break;
        }
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
            m_ppuWindowSetUp = false;
            break;
    }
}

