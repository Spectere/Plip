/* GameBoyInstance.Video.cpp
 *
 * Handles functions related to emulating the Game Boy's PPU.
 */

#include <algorithm>
#include <cassert>
#include <cstring>

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

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
    assert(pos < (ScreenWidth * ScreenHeight));

    if(m_ppuLcdOff) {
        m_videoFormat.plot(m_videoBuffer, pos, 0, 0, 0);
        return;
    }

    const auto paletteRam = objPalette ? m_ppuCgbObjPaletteRam : m_ppuCgbBgPaletteRam;
    const auto index = (((palette & 0b111) * 4) + (color & 0b11)) * 2;
    const uint16_t colorDefinition = (paletteRam->GetByte(index + 1, true) << 8)
                                   | paletteRam->GetByte(index, true);
    const uint8_t red = PPU_ScaleColorChannel(colorDefinition & 0b11111);
    const uint8_t green = PPU_ScaleColorChannel((colorDefinition >> 5) & 0b11111);
    const uint8_t blue = PPU_ScaleColorChannel((colorDefinition >> 10) & 0b11111);
    m_videoFormat.plot(m_videoBuffer, pos, red, green, blue);
}

void GameBoyInstance::PPU_Plot_DMG(int color, const int pos) const {
    assert(pos < (ScreenWidth * ScreenHeight));
    assert(color < 0b100);

    if(m_ppuLcdOff) color = 0b00;

    switch(color) {
        case 0b00:  // White
            m_videoFormat.plot(m_videoBuffer, pos, 0xFF, 0xFF, 0xFF);
            break;

        case 0b01:  // Light Grey
            m_videoFormat.plot(m_videoBuffer, pos, 0xAA, 0xAA, 0xAA);
            break;

        case 0b10:  // Dark Gray
            m_videoFormat.plot(m_videoBuffer, pos, 0x55, 0x55, 0x55);
            break;

        case 0b11:  // Black
            m_videoFormat.plot(m_videoBuffer, pos, 0x00, 0x00, 0x00);
            break;

        default:  // wtf? Red, I guess?
            m_videoFormat.plot(m_videoBuffer, pos, 0xFF, 0x00, 0x00);
            break;
    }
}

void GameBoyInstance::PPU_Reset() {
    // Paint the framebuffer white.
    memset(m_videoBuffer, 0xFF, m_videoBufferSize);

    // Reset PPU variables.
    m_ppuDotClock = 0;
    m_ppuLastLcdControl = 0;
    m_ppuLcdOff = false;
    m_ppuMode = PPU_Mode::OamScan;
    m_ppuLcdXCoordinate = 0;
    m_ppuLcdYCoordinate = 0;
    m_ppuOamAccess = true;
    m_ppuVramAccess = true;
    m_ppuSkip = false;

#ifndef NDEBUG
    m_DBG_ppuFrameDotClocks = 0;
#endif // NDEBUG
}

void GameBoyInstance::PPU_SetMemoryPermissions() {
    switch(m_ppuMode) {
        case PPU_Mode::HBlank:
        case PPU_Mode::VBlank: {
            m_ppuOamAccess = true;
            m_ppuVramAccess = true;
            UpdateSharedPermissions();

            m_ppuCgbBgPaletteRam->SetReadable(true);
            m_ppuCgbBgPaletteRam->SetWritable(true);
            m_ppuCgbObjPaletteRam->SetReadable(true);
            m_ppuCgbObjPaletteRam->SetWritable(true);
            break;
        }
        case PPU_Mode::OamScan: {
            m_ppuOamAccess = false;
            m_ppuVramAccess = true;
            UpdateSharedPermissions();

            m_ppuCgbBgPaletteRam->SetReadable(true);
            m_ppuCgbBgPaletteRam->SetWritable(true);
            m_ppuCgbObjPaletteRam->SetReadable(true);
            m_ppuCgbObjPaletteRam->SetWritable(true);
            break;
        }
        case PPU_Mode::Output: {
            m_ppuOamAccess = false;
            m_ppuVramAccess = false;
            UpdateSharedPermissions();

            m_ppuCgbBgPaletteRam->SetReadable(false);
            m_ppuCgbBgPaletteRam->SetWritable(false);
            m_ppuCgbObjPaletteRam->SetReadable(false);
            m_ppuCgbObjPaletteRam->SetWritable(false);
            break;
        }
        default:
            throw PlipEmulationException("PPU mode out of range");
    }
}

void GameBoyInstance::PPU_Cycle() {
    if(m_doubleSpeed) {
        if(m_ppuSkip) {
            m_ppuSkip = false;
            return;
        }

        m_ppuSkip = true;
    }

    const auto currentLcdControl = m_ioRegisters->GetByte(IoRegister::LcdControl);
    const auto currentLcdStatus = m_ioRegisters->GetByte(IoRegister::LcdStatus);

    PPU_DisableLcdCheck(currentLcdControl);

    if(BIT_TEST(currentLcdControl, 7)) {
        ++m_ppuDotClock;

        switch(m_ppuMode) {
            case PPU_Mode::HBlank:
                if(m_ppuDotClock == PPU_ScanlineTime) PPU_TransitionMode(currentLcdStatus);
                break;
            case PPU_Mode::VBlank:
                if(m_ppuDotClock == PPU_ScanlineTime) {
                    m_ppuDotClock = 0;
                    if(++m_ppuLcdYCoordinate == PPU_Scanlines) {
                        PPU_TransitionMode(currentLcdStatus);
                    }
                }
                break;
            case PPU_Mode::OamScan:
                PPU_Mode_OamScan(currentLcdControl, currentLcdStatus);
                break;
            case PPU_Mode::Output:
                PPU_Mode_Output(currentLcdControl, currentLcdStatus);
                break;
            default:
                throw PlipEmulationException("PPU mode out of range");
        }

#ifndef NDEBUG
        ++m_DBG_ppuFrameDotClocks;
#endif // !NDEBUG
    }

    PPU_UpdateRegisters(currentLcdStatus);

    assert(m_ppuDotClock < PPU_ScanlineTime);
    assert(m_ppuLcdXCoordinate <= static_cast<int>(ScreenWidth));
    assert(m_ppuLcdYCoordinate < PPU_Scanlines);
#ifndef NDEBUG
    assert(m_DBG_ppuFrameDotClocks <= PPU_DBG_TotalDotClocksPerFrame);
#endif // !NDEBUG
}

void GameBoyInstance::PPU_UpdateRegisters(const uint8_t lcdStatus) {
    // There's a fun little quirk where LY will only equal 153 for 4 dot clocks before rolling over to 0.
    const uint8_t regLy = (m_ppuLcdYCoordinate == 153 && m_ppuDotClock >= PPU_LyRolloverClock)
        ? static_cast<uint8_t>(0)
        : m_ppuLcdYCoordinate;

    // Perform LY == LYC comparison.
    const auto lyc = (regLy == m_ioRegisters->GetByte(IoRegister::LcdYCompare));
    if(BIT_TEST(lcdStatus, 6) && lyc) {
        // LYC == LY interrupt
        m_ioRegisters->RaiseInterrupt(Cpu::SharpSm83Interrupt::Stat);
    }

    // Update the PPU status registers.
    m_ioRegisters->Video_SetYCoordinate(regLy);
    m_ioRegisters->Video_SetLcdStatus((lyc ? 0b100 : 0) | static_cast<uint8_t>(m_ppuMode));
}

void GameBoyInstance::PPU_DisableLcdCheck(const uint8_t lcdControl) {
    if(BIT_TEST(m_ppuLastLcdControl, 7) && !BIT_TEST(lcdControl, 7)) {
        // LCD was disabled during this clock cycle. Flag all video RAM as writable, blank the screen,
        // and reset the PPU mode to 0 (HBlank).
        m_ppuOamAccess = m_ppuVramAccess = true;
        UpdateSharedPermissions();

        // Immediately send a blank frame to Plip.
        std::memset(m_videoBuffer, 0xFF, m_videoBufferSize);
        m_video->BeginDraw();
        m_video->Draw(m_videoBuffer);
        m_video->EndDraw();

        // Reset various PPU values.
        m_ppuMode = PPU_Mode::HBlank;  // This is intentional. Enabling the LCD does *not* put the PPU into OAM scan mode.
        m_ppuDotClock = 0;
        m_ppuLcdXCoordinate = 0;
        m_ppuLcdYCoordinate = 0;

#ifndef NDEBUG
        m_DBG_ppuFrameDotClocks = 0;
#endif // !NDEBUG
    } else if(!BIT_TEST(m_ppuLastLcdControl, 7) && BIT_TEST(lcdControl, 7)) {
        // LCD was enabled during this clock cycle. Reset VRAM permissions and enable the screen
        // on the next frame.
        PPU_SetMemoryPermissions();
        m_ppuLcdOff = true;
    }

    m_ppuLastLcdControl = lcdControl;
}

void GameBoyInstance::PPU_TransitionMode(const uint8_t lcdStatus) {
    switch(m_ppuMode) {
        case PPU_Mode::HBlank:  // HBlank->OamScan (mode 2) || HBlank->VBlank (mode 1)
            assert(m_ppuDotClock == PPU_ScanlineTime);
            assert(m_ppuLcdYCoordinate < ScreenHeight);

            m_ppuMode = (++m_ppuLcdYCoordinate == ScreenHeight) ? PPU_Mode::VBlank : PPU_Mode::OamScan;
            m_ppuDotClock = 0;
            m_ppuLcdXCoordinate = 0;

            m_ppuLcdOff = false;

            if(m_ppuMode == PPU_Mode::VBlank) {
                ++m_totalVBlankCount;
                m_ioRegisters->RaiseInterrupt(Cpu::SharpSm83Interrupt::VBlank);

                // Send the contents of the buffer to the frontend.
                m_video->BeginDraw();
                m_video->Draw(m_videoBuffer);
                m_video->EndDraw();
            }

            break;
        case PPU_Mode::VBlank:  // VBlank->OamScan (mode 2)
            assert(m_ppuLcdYCoordinate >= ScreenHeight);

            m_ppuMode = PPU_Mode::OamScan;
            m_ppuDotClock = 0;
            m_ppuLcdXCoordinate = 0;
            m_ppuLcdYCoordinate = 0;
            m_ppuLcdOff = false;

#ifndef NDEBUG
            m_DBG_ppuFrameDotClocks = 0;
#endif // !NDEBUG
            break;
        case PPU_Mode::OamScan:  // OamScan->Output (mode 3)
            assert(m_ppuDotClock == PPU_OamScanTime);
            m_ppuMode = PPU_Mode::Output;
            m_ppuOamScanIndex = 0;

            // DMG prioritizes objects with a lower X position.
            if(!m_cgbMode) {
                std::ranges::sort(m_ppuObjectDrawList, [](const PPU_Object lhs, const PPU_Object rhs) {
                    return lhs.X < rhs.X;
                });
            }

            m_ppuInitOutput = true;

            // Set up the FIFOs.
            m_ppuFifoBgStage = m_ppuFifoObjStage = PPU_FifoStage::GetTileT1;

            break;
        case PPU_Mode::Output:  // Output->HBlank (mode 0)
            assert(m_ppuLcdXCoordinate == ScreenWidth);
            m_ppuMode = PPU_Mode::HBlank;
            m_ppuLcdXCoordinate = 0;
            m_ppuObjectDrawList.clear();
            break;
        default:
            throw PlipEmulationException("PPU mode out of range");
    }

    PPU_TransitionStatInterrupt(lcdStatus);
    PPU_SetMemoryPermissions();
}

void GameBoyInstance::PPU_TransitionStatInterrupt(const uint8_t lcdStatus) const {
    bool raiseInterrupt {};

    switch(m_ppuMode) {
        case PPU_Mode::HBlank:   // Mode 0
            if(BIT_TEST(lcdStatus, 3)) raiseInterrupt = true;
            break;
        case PPU_Mode::VBlank:   // Mode 1
            if(BIT_TEST(lcdStatus, 4)) raiseInterrupt = true;
            break;
        case PPU_Mode::OamScan:  // Mode 2
            if(BIT_TEST(lcdStatus, 5)) raiseInterrupt = true;
            break;
        case PPU_Mode::Output:   // Mode 3
            // Mode 3 never triggers a STAT interrupt.
            break;
        default:
            throw PlipEmulationException("PPU mode out of range");
    }

    if(raiseInterrupt) {
        m_ioRegisters->RaiseInterrupt(Cpu::SharpSm83Interrupt::Stat);
    }
}

void GameBoyInstance::PPU_Mode_OamScan(const uint8_t lcdControl, const uint8_t lcdStatus) {
    assert(m_ppuDotClock <= PPU_OamScanTime);

    if(m_ppuDotClock % 2 == 1) return;  // Run on even clocks (2, 4, 6...80).

    if(m_ppuObjectDrawList.size() < PPU_ObjectsPerScanline) {
        const bool tallSprites = BIT_TEST(lcdControl, 2);

        const auto ptr = m_ppuOamScanIndex * 4;
        const auto y = m_oam->GetByte(ptr, true);

        const auto minY = y - 16;
        const auto maxY = minY + (tallSprites ? 16 : 8);

        if((minY <= m_ppuLcdYCoordinate) && (m_ppuLcdYCoordinate < maxY)) {
            // Object overlaps LY.
            m_ppuObjectDrawList.push_back(PPU_Object {
                .Y = y,
                .X = m_oam->GetByte(ptr + 1, true),
                .Index = m_oam->GetByte(ptr + 2, true),
                .Flags = m_oam->GetByte(ptr + 3, true)
            });
        }
    }

    if(++m_ppuOamScanIndex == PPU_MaximumObjectCount) PPU_TransitionMode(lcdStatus);  // Scan complete. Switch modes.
}

void GameBoyInstance::PPU_Mode_Output(const uint8_t lcdControl, const uint8_t lcdStatus) {
    if(m_ppuInitOutput) {
        m_ppuScrollX = m_ioRegisters->GetByte(IoRegister::ScrollX);
        m_ppuBgScrollPenalty = m_ppuScrollX & 0b111;
        m_ppuFifoXCoordinate = -8;
        m_ppuLcdXCoordinate = 0;
        m_ppuInitOutput = false;
    }

    PPU_Fetcher_Background(lcdControl);

    if(!m_ppuFifoBgPixel.empty()) {
        // Plot pixels (if they would appear on screen).
        const auto pixel = m_ppuFifoBgPixel.front();
        m_ppuFifoBgPixel.pop();

        if(m_ppuFifoXCoordinate > 0) {
            if(m_ppuBgScrollPenalty <= 0) {
                const auto pixelOffset = static_cast<int>((m_ppuLcdYCoordinate * ScreenWidth) + m_ppuLcdXCoordinate);
                const auto bgp = m_ioRegisters->GetByte(IoRegister::BgPalette);
                const auto color = (bgp >> (pixel.Color * 2)) & 0b11;
                PPU_Plot_DMG(color, pixelOffset);

                ++m_ppuLcdXCoordinate;
            } else {
                --m_ppuBgScrollPenalty;
            }
        }
    }

    if(m_ppuLcdXCoordinate == ScreenWidth) {
        PPU_TransitionMode(lcdStatus);
    }
}

uint16_t GameBoyInstance::PPU_GetBaseBgTileAddr(const uint8_t idx, const int lineOffset) const {
    const auto blockBaseAddress = (idx >= 0x80)
        ? PPU_Block1
        : (m_ppuBgUseBlock2 ? PPU_Block2 : PPU_Block0);
    return blockBaseAddress + ((idx % 128) * 16) + (lineOffset * 2);
}

uint16_t GameBoyInstance::PPU_GetFetchTileIndex() const {
    const auto adjustedX = std::max(m_ppuFifoXCoordinate + m_ppuScrollX, 0);
    const auto adjustedY = std::max(m_ppuLcdYCoordinate + m_ppuScrollY, 0);

    const auto tileX = (adjustedX / PPU_TileSizeX) % PPU_MapTileCountX;
    const auto tileY = (adjustedY / PPU_TileSizeY) % PPU_MapTileCountY;
    return (tileY * PPU_MapTileCountX) + tileX;
}

void GameBoyInstance::PPU_Fetcher_Background(const uint8_t lcdControl) {
    switch(m_ppuFifoBgStage) {
        case PPU_FifoStage::GetTileT1:
            // Lock in SCX, SCY, and other attributes at the start of the tile fetch. Only the upper five
            // bits of SCX can be changed after output begins.
            m_ppuScrollX = (m_ioRegisters->GetByte(IoRegister::ScrollX) & 0b11111000) | (m_ppuScrollX & 0b111);
            m_ppuScrollY = m_ioRegisters->GetByte(IoRegister::ScrollY);
            m_ppuBgUseBlock2 = !BIT_TEST(lcdControl, 4);
            m_ppuBgUseMap1 = BIT_TEST(lcdControl, 3);
            m_ppuBgPriority = BIT_TEST(lcdControl, 0);
            m_ppuTileLine = (m_ppuLcdYCoordinate + m_ppuScrollY) % PPU_TileSizeY;

            // Now, fall through.
        case PPU_FifoStage::GetTileDataLowT1:
        case PPU_FifoStage::GetTileDataHighT1:
            // Advance to the fetch stage (this is how enums were meant to be used, right? :D)
            m_ppuFifoBgStage = static_cast<PPU_FifoStage>(static_cast<int>(m_ppuFifoBgStage) + 1);
            break;
        case PPU_FifoStage::GetTileT2: {
            const auto t = PPU_GetFetchTileIndex();
            const auto baseMapAddr = PPU_TileMapBase + (m_ppuBgUseMap1 ? PPU_TileMapBlockOffset : 0);
            const auto tileIdx = m_videoRam->GetByte(baseMapAddr + PPU_GetFetchTileIndex(), true);
            m_ppuThisBgTile.TileAddress = PPU_GetBaseBgTileAddr(tileIdx, m_ppuTileLine);
            m_ppuFifoBgStage = PPU_FifoStage::GetTileDataLowT1;
            break;
        }
        case PPU_FifoStage::GetTileDataLowT2: {
            m_ppuThisBgTile.TileDataLow = m_videoRam->GetByte(m_ppuThisBgTile.TileAddress, true);
            m_ppuFifoBgStage = PPU_FifoStage::GetTileDataHighT1;
            break;
        }
        case PPU_FifoStage::GetTileDataHighT2: {
            m_ppuThisBgTile.TileDataHigh = m_videoRam->GetByte(m_ppuThisBgTile.TileAddress + 1, true);
            m_ppuFifoBgStage = PPU_FifoStage::Push;
            break;
        }
        case PPU_FifoStage::Push:  // Handled below.
        default:
            break;
    }

    // Pixels can be pushed on the same cycle that the high tile data is retrieved.
    if(m_ppuFifoBgStage == PPU_FifoStage::Push) {
        if(m_ppuFifoBgPixel.empty()) {
            // Push all eight pixels.
            for(auto i = 0; i < PPU_TileSizeX; ++i) {
                const auto shift = (7 - i);

                const auto lowField = (m_ppuThisBgTile.TileDataLow >> shift) & 0b1;
                const auto highField = (m_ppuThisBgTile.TileDataHigh >> shift) & 0b1;

                m_ppuFifoBgPixel.push({
                    .Color = static_cast<uint8_t>(lowField | (highField << 1)),
                    .Palette = 0,      // TODO: CGB
                    .ObjPriority = 0,  // TODO: CGB
                    .BgPriority = m_ppuBgPriority
                });
            }

            m_ppuFifoBgStage = PPU_FifoStage::GetTileT1;
            m_ppuFifoXCoordinate += 8;
        }
    }

    assert(m_ppuFifoBgPixel.size() <= PPU_FifoLength);
}
