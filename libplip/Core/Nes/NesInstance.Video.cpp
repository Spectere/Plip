/* NesInstance.Video.cpp
 *
 * Handles emulating the PPU.
 */

#include "NesInstance.h"
#include "../../PlipEmulationException.h"
#include "../../PlipIo.h"

using Plip::Core::Nes::NesInstance;

void NesInstance::PPU_CheckForNextScanline() {
    if(++m_ppuScanlineCycle != m_ppuScanlineEnd) return;

    ++m_ppuScanlineY;
    m_ppuScanlineCycle = 0;
    if(m_ppuScanlineY == 261) {
        m_ppuScanlineY = -1;
        m_ppuScanlineEnd = m_ppuRegisters->GetOddFrame() ? 339 : 340;
    } else {
        m_ppuScanlineEnd = 340;
    }
}

void NesInstance::PPU_Cycle() {
    if(m_ppuScanlineY < 240) {
        // Fetching/rendering
        PPU_Cycle_FetchAndRender();
    } else if(m_ppuScanlineY == 240) {
        // Idle
        PPU_CheckForNextScanline();
    } else if(m_ppuScanlineY > 240) {
        // VBlank
        PPU_Cycle_VBlank();
    }
}

void NesInstance::PPU_Cycle_FetchAndRender() {
    const auto thisCycle = m_ppuScanlineCycle;
    const auto thisPixelX = thisCycle - 1;
    
    if(m_ppuScanlineY == -1 && thisCycle <= 320) {
        // Unset VBlank.
        if(thisCycle == 0) {
            m_ppuRegisters->SetStatusVBlank(false);
            m_cpu->ClearNmi();
        }
        
        // Nothing is displayed on this line, so wait until we can do something productive
        // (i.e. fetch data for the next scanline).
        ++m_ppuScanlineCycle;
        return;
    }

    if(thisCycle == 0) {
        // Idle
    } else if(thisCycle <= 256) {
        // Fetching upcoming background tiles and drawing.
        m_ppuNametablePointer = ((m_ppuScanlineY / 8) * 32) + (thisPixelX / 8) + 2;

        if(thisPixelX < (256 - 16) && thisCycle % 2 == 0) {
            // Fetch data every other cycle.
            PPU_ReadMemory(false);
        }

        // Pop a tile from the queue every 8 cycles.
        if(thisPixelX % 8 == 0) {
            m_ppuNametablePointerPlotting = m_ppuTileQueue.front(); m_ppuTileQueue.pop();
            m_ppuCurrentAttribute = m_ppuTileQueue.front(); m_ppuTileQueue.pop();
            m_ppuCurrentTileLow = m_ppuTileQueue.front(); m_ppuTileQueue.pop();
            m_ppuCurrentTileHigh = m_ppuTileQueue.front(); m_ppuTileQueue.pop();
        }

        // Draw!
        PPU_Draw_Background(thisPixelX);
    } else if(thisCycle <= 320) {
        // Fetching sprites for the next scanline.
    } else if(thisCycle <= 336 && m_ppuScanlineY != 239) {
        // Fetching the next two tiles for the next scanline.
        m_ppuNametablePointer = (((m_ppuScanlineY + 1) / 8) * 32) + ((thisCycle - 321) / 8);
        if(thisCycle % 2) { PPU_ReadMemory(false); }
    } else if(thisCycle <= 336 && m_ppuScanlineY == 239) {
        // Just wait a sec.
    } else if(thisCycle <= 340) {
        // Fetching two unused nametable bytes.
        if(thisCycle % 2) { PPU_ReadMemory(false, true); }
    }

    PPU_CheckForNextScanline();
}

void NesInstance::PPU_Cycle_VBlank() {
    if(m_ppuScanlineY == 241 && m_ppuScanlineCycle == 1) {
        // Send the video buffer to the emulator frontend.
        m_video->BeginDraw();
        m_video->Draw(m_videoBuffer);
        m_video->EndDraw();
        
        // Set the PPUSTATUS VBlank flag and raise an NMI on cycle 1 (the second cycle) of scanline 241 (if enabled).
        m_ppuRegisters->SetStatusVBlank(true);

        if(m_ppuRegisters->GetVBlankNmi()) {
            m_cpu->FlagNmi();
        }
    }

    PPU_CheckForNextScanline();
}

void NesInstance::PPU_Draw_Background(const int pixelX) {
    const auto currentColor = ((m_ppuCurrentTileHigh & 0x80) >> 6) | (m_ppuCurrentTileLow >> 7);
    const auto actualPixel = pixelX + (ScreenWidth * m_ppuScanlineY);
    m_ppuCurrentTileHigh <<= 1;
    m_ppuCurrentTileLow <<= 1;
    
    const auto quad = (m_ppuNametablePointerPlotting & 0b1000000) >> 5 | ((m_ppuNametablePointerPlotting & 0b10) >> 2);
    const auto paletteOffset = ((m_ppuCurrentAttribute >> (quad * 2)) & 0b11) * 4;
    
    const auto color = currentColor == 0
        ? PPU_GetColorPtr(m_ppuRegisters->GetBytePalette(0))  // backdrop
        : PPU_GetColorPtr(m_ppuRegisters->GetBytePalette(paletteOffset + currentColor));

    m_videoFormat.plot(m_videoBuffer, actualPixel, *color, *(color + 1), *(color + 2));
}

uint8_t* NesInstance::PPU_GetColorPtr(int index) {
    index = (index % 0x40) * 3;
    return &m_nesPalette[index];
}

Plip::PlipError NesInstance::PPU_LoadPalette(const std::string &path) {
    if(!PlipIo::FileExists(path)) {
        return PlipError::FileNotFound;
    }

    if(PlipIo::GetSize(path) < m_nesPaletteSize) {
        return PlipError::AssetFileTruncated;
    }

    const auto paletteData = PlipIo::ReadFile(path, m_nesPaletteSize);
    for(auto i = 0; i < m_nesPaletteSize; ++i) {
        m_nesPalette[i] = static_cast<uint8_t>(paletteData[i]);
    }

    return PlipError::Success;
}

void NesInstance::PPU_ReadMemory(const bool spriteQueue, const bool holdStage) {
    auto& queue = spriteQueue ? m_ppuSpriteQueue : m_ppuTileQueue;
    const auto nametableBase = m_ppuRegisters->GetBaseNamespaceAddress();
    const auto bgPatternBase = m_ppuRegisters->GetBackgroundPatternAddress();
    
    // Correct the Y offset for reads that occur on the previous scanline.
    const auto offset = (m_ppuScanlineY % 8) + (m_ppuScanlineCycle > 256 ? 1 : 0);

    uint8_t pushValue = 0;
    
    switch(m_ppuReadStage) {
        case 0: {
            // Nametable byte. We're going to push the current pointer instead, though.
            m_ppuLastPatternIndex = m_mapper->GetBytePpu(nametableBase + m_ppuNametablePointer); 
            pushValue = m_ppuNametablePointer;
            break;
        }

        case 1: {
            // Attribute table byte.
            // m_ppuNametablePointer bits -> 0b000000_yyyyy_xxxxx (given the nametable)
            // Divide each coordinate by 4 (shift right by 2) to get the coordinate in the attribute table.
            // Final result -> 0b00yyyxxx
            auto attributeOffset = (m_ppuNametablePointer >> 2) & 0b111;
            attributeOffset |= (m_ppuNametablePointer >> 4) & 0b111000;
            pushValue = m_mapper->GetBytePpu(nametableBase + 0x3C0 + attributeOffset);
            break;
        }

        case 2: {
            // Pattern table tile low.
            pushValue = m_mapper->GetBytePpu(bgPatternBase + (16 * m_ppuLastPatternIndex) + offset);
            break;
        }

        case 3: {
            // Pattern table tile high.
            pushValue = m_mapper->GetBytePpu(bgPatternBase + (16 * m_ppuLastPatternIndex) + offset + 8);
            break;
        }

        default: {
            throw PlipEmulationException("NES: PPU memory fetch is in an invalid state.");
        }
    }

    if(!holdStage) {
        queue.push(pushValue);
        if(++m_ppuReadStage > 3) {
            m_ppuReadStage = 0;
        }
    }
}

std::map<std::string, Plip::DebugValue> NesInstance::PPU_GetDebugInfo() const {
    return {
            { "Scanline", DebugValue(DebugValueType::Int16Le, static_cast<uint64_t>(m_ppuScanlineY)) },
            { "Scanline Cycle", DebugValue(DebugValueType::Int16Le, static_cast<uint64_t>(m_ppuScanlineCycle)) },
        };
}
