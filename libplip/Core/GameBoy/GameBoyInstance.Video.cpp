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

    // Reset fetcher.
    m_ppuFetcherResult.fill(0);
    m_ppuFetchingObj = false;
    m_ppuFetcherStage = PPU_FetcherStage::Idle;
    m_ppuFetcherTile = {};

    // Reset BG/Window properties.
    m_ppuBgUseMap1 = false;
    m_ppuBgUseBlock2 = false;
    m_ppuBgPriority = false;

    // Reset OBJ properties.
    m_ppuObjectDrawList.clear();
    m_ppuOamScanIndex = 0;

    // Reset FIFOs.
    m_ppuFifoBg.clear();
    m_ppuFifoObj.clear();

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
            m_ppuInitOutput = true;
            m_ppuPlot = false;

            // Sort by X coordinate. CGB priorities will be resolved by the fetcher.
            std::ranges::sort(m_ppuObjectDrawList, [](const PPU_Object lhs, const PPU_Object rhs) {
                return lhs.X < rhs.X;
            });

            // Set up the FIFOs.
            m_ppuFetcherStage = PPU_FetcherStage::GetTileT1;
            m_ppuFifoBg.clear();
            m_ppuFifoObj.clear();

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
        m_ppuDiscardBg = m_ppuScrollX & 0b111;
        m_ppuFifoXCoordinate = -8;
        m_ppuLcdXCoordinate = -8;
        m_ppuInitOutput = false;
    }

    // Clock the fetcher.
    PPU_Fetcher_Setup(lcdControl);
    PPU_Fetcher_Clock();
    if(m_ppuFetcherStage == PPU_FetcherStage::Push) {
        PPU_Fetcher_Finish();
    }

    if(m_ppuPlot && !m_ppuFifoBg.empty()) {
        // Plot pixels (if they would appear on screen).
        const auto pixel = m_ppuFifoBg.front();
        m_ppuFifoBg.pop_front();

        if(m_ppuLcdXCoordinate >= 0) {
            int color;
            const auto pixelOffset = static_cast<int>((m_ppuLcdYCoordinate * ScreenWidth) + m_ppuLcdXCoordinate);

            if(!m_cgbMode && !BIT_TEST(lcdControl, 0)) {
                // In DMG mode, the BG can be disabled (even mid-scanline). If it's disabled, the BG
                // should be white (ignore BGP). The BG tile fetch otherwise functions per normal.
                color = 0;
            } else {
                const auto bgp = m_ioRegisters->GetByte(IoRegister::BgPalette);
                color = (bgp >> (pixel.Color * 2)) & 0b11;
            }

            PPU_Plot_DMG(color, pixelOffset);
        }

        if(m_ppuDiscardBg-- <= 0) {
            ++m_ppuLcdXCoordinate;
            assert(m_ppuLcdXCoordinate <= static_cast<int32_t>(ScreenWidth));
        }
    }

    if(m_ppuLcdXCoordinate == ScreenWidth) {
        PPU_TransitionMode(lcdStatus);
    }
}

uint16_t GameBoyInstance::PPU_GetFetchTileIndex() const {
    const auto adjustedX = std::max(m_ppuFifoXCoordinate + m_ppuScrollX, 0 + m_ppuScrollX);
    const auto adjustedY = m_ppuLcdYCoordinate + m_ppuScrollY;

    const auto tileX = (adjustedX / PPU_TileSizeX) % PPU_MapTileCountX;
    const auto tileY = (adjustedY / PPU_TileSizeY) % PPU_MapTileCountY;
    return (tileY * PPU_MapTileCountX) + tileX;
}

void GameBoyInstance::PPU_Fetcher_Clock() {
    switch(m_ppuFetcherStage) {
        case PPU_FetcherStage::Idle:
            return;

        case PPU_FetcherStage::GetTileT1:  // Delay
            m_ppuFetcherStage = PPU_FetcherStage::GetTileT2;
            break;

        case PPU_FetcherStage::GetTileDataLowT1:  // Delay
            m_ppuFetcherStage = PPU_FetcherStage::GetTileDataLowT2;
            break;

        case PPU_FetcherStage::GetTileDataHighT1:  // Delay
            m_ppuFetcherStage = PPU_FetcherStage::GetTileDataHighT2;
            break;

        case PPU_FetcherStage::GetTileT2: {  // Fetch
            if(!m_ppuFetchingObj) {
                // BG/Window
                const auto baseMapAddr = PPU_TileMapBase + (m_ppuBgUseMap1 ? PPU_TileMapBlockOffset : 0);
                const auto tileIdx = m_videoRam->GetByte(baseMapAddr + PPU_GetFetchTileIndex(), true);
                m_ppuFetcherTile.TileAddress = PPU_GetBaseBgTileAddr(tileIdx, m_ppuTileLine);
            } else {
                // TODO: OBJ
            }

            m_ppuFetcherStage = PPU_FetcherStage::GetTileDataLowT1;
            break;
        }

        case PPU_FetcherStage::GetTileDataLowT2: {  // Fetch
            m_ppuFetcherTile.TileDataLow = m_videoRam->GetByte(m_ppuFetcherTile.TileAddress, true);
            m_ppuFetcherStage = PPU_FetcherStage::GetTileDataHighT1;
            break;
        }

        case PPU_FetcherStage::GetTileDataHighT2: {  // Fetch
            m_ppuFetcherTile.TileDataHigh = m_videoRam->GetByte(m_ppuFetcherTile.TileAddress + 1, true);
            m_ppuFetcherStage = PPU_FetcherStage::Push;
            break;
        }

        case PPU_FetcherStage::Push:  // Handled below.
        default:
            break;
    }

    // Pixels can be pushed on the same cycle of the high tile data retrieval.
    if(m_ppuFetcherStage == PPU_FetcherStage::Push) {
        for(auto i = 0; i < PPU_TileSizeX; ++i) {
            const auto shift = 7 - i;

            const auto lowField = (m_ppuFetcherTile.TileDataLow >> shift) & 0b1;
            const auto highField = (m_ppuFetcherTile.TileDataHigh >> shift) & 0b1;

            m_ppuFetcherResult[i] = static_cast<uint8_t>(lowField | (highField << 1));
        }
    }
}

void GameBoyInstance::PPU_Fetcher_Finish() {
    const auto fifo = m_ppuFetchingObj ? &m_ppuFifoObj : &m_ppuFifoBg;

    for(auto i = 0; i < 8; ++i) {
        fifo->push_back({
            .Color = m_ppuFetcherResult[i],
            .Palette = 0,      // TODO: CGB
            .ObjPriority = 0,  // TODO: CGB
            .BgPriority = m_ppuBgPriority
        });
    }

    m_ppuPlot = true;
    m_ppuFetcherStage = PPU_FetcherStage::Idle;
    m_ppuFifoXCoordinate += 8;

    assert(fifo->size() <= PPU_FifoLength);
}

void GameBoyInstance::PPU_Fetcher_Setup(const uint8_t lcdControl) {
    if(m_ppuFetcherStage != PPU_FetcherStage::Idle) {
        // Fetcher is busy. Return.
        return;
    }

    if(m_ppuFifoBg.empty()) {
        // BG FIFO is empty. Lock in SCX, SCY, and other attributes at the start of the tile fetch.
        // Only the upper five bits of SCX can be changed after output begins.
        m_ppuScrollX = (m_ioRegisters->GetByte(IoRegister::ScrollX) & 0b11111000) | (m_ppuScrollX & 0b111);
        m_ppuScrollY = m_ioRegisters->GetByte(IoRegister::ScrollY);
        m_ppuBgUseBlock2 = !BIT_TEST(lcdControl, 4);
        m_ppuBgUseMap1 = BIT_TEST(lcdControl, 3);
        m_ppuBgPriority = BIT_TEST(lcdControl, 0);  // CGB. DMG's BG/Window disable is handled separately.
        m_ppuTileLine = (m_ppuLcdYCoordinate + m_ppuScrollY) % PPU_TileSizeY;

        m_ppuFetcherStage = PPU_FetcherStage::GetTileT1;

        return;
    }
}
