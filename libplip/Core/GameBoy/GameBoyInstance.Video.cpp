/* GameBoyInstance.Video.cpp
 *
 * Simulated graphics hardware for the GameBoy core.
 */

#include <cstring>
#include <sstream>

#include "../../PlipEmulationException.h"
#include "../../PlipUtility.h"

#include "GameBoyInstance.h"

namespace Plip::Core::GameBoy {
    void GameBoyInstance::VideoCycle() {
        bool result;

        switch(m_videoMode) {
            case m_videoModeHBlank:
                result = VideoHBlank();
                break;
            case m_videoModeVBlank:
                result = VideoVBlank();
                break;
            case m_videoModeOamSearch:
                result = VideoOamSearch();
                break;
            case m_videoModePicGen:
                result = VideoVidGen();
                break;
            default:
                std::stringstream ex;
                ex << "Invalid video mode: "
                   << PlipUtility::FormatHex(m_videoMode, 1) << "\n\n"
                   << m_cpu->DumpRegisters();
                throw Plip::PlipEmulationException(ex.str().c_str());
        }

        if(result) {
            m_dotCount += 1;
        } else {
            // Mode transition.
            VideoModePreTransition();
            m_videoMode = ++m_videoMode > 3 ? 0 : m_videoMode;
            if(m_videoMode == 1 && m_videoLy < m_screenHeight)
                m_videoMode++;  // only transition to 1 after the last HBlank
            VideoModePostTransition();
        }

        auto stat = m_ioRegisters->GetByte(m_regLcdcStatus) & 0b11111000;
        stat |= (m_videoCoincidence << 2) | m_videoMode;
        m_ioRegisters->SetByte(m_regLcdcStatus, stat);
    }

    void GameBoyInstance::VideoExecute() {
        auto lcdc = m_ioRegisters->GetByte(m_regLcdControl);

        // Run 4 dot clock cycles (4.19MHz) if the display is enabled.
        if(BIT_TEST(m_videoLastLcdc, 7) && !BIT_TEST(lcdc, 7)) {
            // The LCD display was disabled during this CPU cycle. Flag all
            // video memory as being writable.
            m_oam->SetWritable(true);
            m_videoRam->SetWritable(true);

            // When the LCD is disabled, the screen should go blank.
            memset(m_videoBuffer, 0xFF, m_videoBufferSize);
            m_video->BeginDraw();
            m_video->Draw(m_videoBuffer);
            m_video->EndDraw();
            m_video->Render();
        } else if(!BIT_TEST(m_videoLastLcdc, 7) && BIT_TEST(lcdc, 7)) {
            // The LCD display was enabled during this CPU cycle. Set the
            // video memory accessibility appropriately.
            VideoSetMemoryPermissions();
            m_lcdBlankFrame = true;
        }

        if(BIT_TEST(lcdc, 7)) {
            for(auto dotCycle = 0; dotCycle < m_dotsPerCycle; dotCycle++) {
                VideoCycle();
            }
        }

        m_ioRegisters->SetByte(m_regLy, m_videoLy);
        m_videoLastLcdc = lcdc;
    }

    bool GameBoyInstance::VideoHBlank() const {
        return m_dotCount < m_videoScanlineTime;
    }

    bool GameBoyInstance::VideoOamSearch() {
        if(m_dotCount != 0) return m_dotCount < m_videoOamScanTime;

        auto spriteHeight = ((m_ioRegisters->GetByte(m_regLcdControl) >> 2) & 1) ? 16 : 8;

        // Just do everything at once since the OAM will be locked at this point.
        m_spriteListIdx = 0;
        memset(m_spriteList, 0xFF, m_maxSpritesPerScanline);
        for(auto sprIdx = 0; sprIdx < 0; sprIdx++) {
            auto sprAddr = 4 * sprIdx;
            auto sprY = m_oam->GetByte(sprAddr);
            auto sprX = m_oam->GetByte(sprAddr + 1);

            if(sprX == 0 || sprX >= m_screenWidth + 8) continue;
            if(m_videoLy >= sprY - spriteHeight || m_videoLy < sprY - (16 - spriteHeight)) {
                m_spriteList[m_spriteListIdx++] = sprIdx;
            }

            if(m_spriteListIdx == m_maxSpritesPerScanline) break;
        }

        return m_dotCount < m_videoOamScanTime;
    }

    void GameBoyInstance::VideoModePostTransition() {
        uint8_t stat = m_ioRegisters->GetByte(m_regLcdcStatus);
        VideoSetMemoryPermissions();

        // New mode.
        switch(m_videoMode) {
            case m_videoModeHBlank:
                if(BIT_TEST(stat, 3)) m_cpu->Interrupt(INTERRUPT_LCDSTAT);
                break;

            case m_videoModeVBlank:
                m_dotCount = 0;

                // Plot the current buffer to the screen and flip buffers.
                m_video->BeginDraw();
                m_video->Draw(m_videoBuffer);
                m_video->EndDraw();
                m_video->Render();

                m_lcdBlankFrame = false;

                m_cpu->Interrupt(INTERRUPT_VBLANK);
                if(BIT_TEST(stat, 4)) m_cpu->Interrupt(INTERRUPT_LCDSTAT);
                break;

            case m_videoModeOamSearch:
                memset(m_spriteList, 0x00, m_maxSpritesPerScanline);
                m_dotCount = 0;
                m_videoCoincidence = m_videoLy == m_ioRegisters->GetByte(m_regLyCompare) ? 1 : 0;

                if(BIT_TEST(stat, 5))  // OAM interrupt
                    m_cpu->Interrupt(INTERRUPT_LCDSTAT);

                if(m_videoCoincidence && BIT_TEST(stat, 6))  // LYC=LY interrupt.
                    m_cpu->Interrupt(INTERRUPT_LCDSTAT);

                break;

            case m_videoModePicGen:
                m_dotCount = 0;
                m_vidGenStage = BackgroundScrolling;
                break;

            default:
                std::stringstream ex;
                ex << "Invalid video mode: "
                   << PlipUtility::FormatHex(m_videoMode, 1) << "\n\n"
                   << m_cpu->DumpRegisters();
                throw Plip::PlipEmulationException(ex.str().c_str());
        }
    }

    void GameBoyInstance::VideoModePreTransition() {
        // Previous mode.
        switch(m_videoMode) {
            case m_videoModeHBlank:
                m_videoLx = 0;
                m_videoLy++;
                break;
            case m_videoModeVBlank:
                m_videoLy = 0;
                break;
            case m_videoModeOamSearch:
            case m_videoModePicGen:
                break;
            default:
                std::stringstream ex;
                ex << "Invalid video mode: "
                   << PlipUtility::FormatHex(m_videoMode, 1) << "\n\n"
                   << m_cpu->DumpRegisters();
                throw Plip::PlipEmulationException(ex.str().c_str());
        }
    }

    void GameBoyInstance::VideoSetMemoryPermissions() {
        switch(m_videoMode) {
            case m_videoModeHBlank:
            case m_videoModeVBlank:
                m_oam->SetWritable(true);
                m_videoRam->SetWritable(true);
                break;
            case m_videoModeOamSearch:
                m_oam->SetWritable(false);
                m_videoRam->SetWritable(true);
                break;
            case m_videoModePicGen:
                m_oam->SetWritable(false);
                m_videoRam->SetWritable(false);
                break;
            default:
                std::stringstream ex;
                ex << "Invalid video mode: "
                   << PlipUtility::FormatHex(m_videoMode, 1) << "\n\n"
                   << m_cpu->DumpRegisters();
                throw Plip::PlipEmulationException(ex.str().c_str());
        }
    }

    bool GameBoyInstance::VideoVBlank() {
        if(m_dotCount > 0)
            if(m_dotCount % (m_videoOamScanTime + m_videoScanlineTime) == 0)
                m_videoLy++;

        return m_dotCount < m_videoVBlankTime;
    }

    bool GameBoyInstance::VideoVidGen() {
        uint8_t scx;

        switch(m_vidGenStage) {
            case BackgroundScrolling:
                // Pause the dot clock to simulate the background shifter.
                scx = m_ioRegisters->GetByte(m_regScx);
                if(++m_vidGenTick > scx % 8) {
                    m_vidGenTick = 0;
                    m_vidGenStage = Drawing;
                }
                break;

            case Drawing:
                VideoVidGenDraw();
                m_videoLx++;

                break;

            default:
                std::stringstream ex;
                ex << "Invalid video generation stage: "
                   << PlipUtility::FormatHex((int)m_vidGenStage, 1) << "\n\n"
                   << m_cpu->DumpRegisters();
                throw Plip::PlipEmulationException(ex.str().c_str());
        }

        return m_videoLx < m_screenWidth;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-function-cognitive-complexity"
    inline void GameBoyInstance::VideoVidGenDraw() {
        // TODO: Add timing routine.
        uint8_t pixelColor, pixelDataLow, pixelDataHigh, pixelDataCombined;
        uint8_t tileIdx, tilePX, tilePY, tileShift;
        uint8_t lineOffset;

        auto pos = (m_videoLy * m_screenWidth) + m_videoLx;

        auto lcdc = m_ioRegisters->GetByte(m_regLcdControl);
        bool lcdEnabled = BIT_TEST(lcdc, 7);
        if(!lcdEnabled) {
            Plot(255, pos);
            return;
        }

        auto scx = m_ioRegisters->GetByte(m_regScx);
        auto scy = m_ioRegisters->GetByte(m_regScy);
        auto bgp = m_ioRegisters->GetByte(m_regBgp);

        if(BIT_TEST(lcdc, 0)) { // BG/Window Display Enabled
            auto tileDataAddr = m_vramTileBase + (BIT_TEST(lcdc, 4) ? 0 : m_vramTileBlockOffset);
            auto tileMapAddr = m_vramBgBase + (BIT_TEST(lcdc, 3) ? m_vramBgBlockOffset : 0);
            auto tileX = ((scx + m_videoLx) / 8) % 32;
            auto tileY = ((scy + m_videoLy) / 8) % 32;
            tilePX = (scx + m_videoLx) % 8;
            tilePY = (scy + m_videoLy) % 8;

            auto mapIdx = (tileY * 32) + tileX;
            tileIdx = m_videoRam->GetByte(tileMapAddr + mapIdx);

            lineOffset = tilePY * 2;

            pixelDataLow = m_videoRam->GetBytePrivileged(tileDataAddr + (tileIdx * 16) + lineOffset);
            pixelDataHigh = m_videoRam->GetBytePrivileged(tileDataAddr + (tileIdx * 16) + lineOffset + 1);
            tileShift = 7 - tilePX;
            pixelDataCombined = (((pixelDataHigh >> tileShift) & 0b1) << 1)
                              | ((pixelDataLow >> tileShift) & 0b1);

            pixelColor = (bgp >> (pixelDataCombined * 2)) & 0b11;
            Plot(pixelColor, pos);

            if(BIT_TEST(lcdc, 5)) { // Window Display Enabled
                auto wx = m_ioRegisters->GetByte(m_regWx) - 7;
                auto wy = m_ioRegisters->GetByte(m_regWy);

                if(!(wx > 166 || wy > 143) || m_videoLx >= wx || m_videoLy >= wy) {
                    tileMapAddr = m_vramBgBase + (BIT_TEST(lcdc, 6) ? m_vramBgBlockOffset : 0);
                    tileX = ((m_videoLx + wx) / 8) % 32;
                    tileY = ((m_videoLy + wy) / 8) % 32;
                    tilePX = (m_videoLx + wx) % 8;
                    tilePY = (m_videoLy + wy) % 8;

                    mapIdx = (tileY * 32) + tileX;
                    tileIdx = m_videoRam->GetBytePrivileged(tileMapAddr + mapIdx);

                    lineOffset = tilePY * 2;

                    pixelDataLow = m_videoRam->GetBytePrivileged(tileDataAddr + (tileIdx * 16) + lineOffset);
                    pixelDataHigh = m_videoRam->GetBytePrivileged(tileDataAddr + (tileIdx * 16) + lineOffset + 1);
                    tileShift = 7 - tilePX;
                    pixelDataCombined = (((pixelDataHigh >> tileShift) & 0b1) << 1)
                                        | ((pixelDataLow >> tileShift) & 0b1);

                    pixelColor = (bgp >> (pixelDataCombined * 2)) & 0b11;
                    Plot(pixelColor, pos);
                }
            }
        } else {
            // BG/Window display disabled. Draw a white pixel.
            Plot(0b00, pos);
        }

        if(!BIT_TEST(lcdc, 1)) return;

        // OBJ Display Enabled
        auto obp0 = m_ioRegisters->GetByte(m_regObp0);
        auto obp1 = m_ioRegisters->GetByte(m_regObp1);
        bool doubleHeight = BIT_TEST(lcdc, 2);

        for(auto i = 0; i < m_maxSpritesPerScanline; i++) {
            if(m_spriteList[i] == 0xFF) break;

            auto base = 4 * m_spriteList[i];

            // Fetch the sprite attributes from the OAM.
            auto sprAttr = m_oam->GetBytePrivileged(base + 3);
            bool sprFlipX = BIT_TEST(sprAttr, 5);
            bool sprFlipY = BIT_TEST(sprAttr, 6);
            bool sprPriority = !BIT_TEST(sprAttr, 7);
            auto sprPalette = BIT_TEST(sprAttr, 4) ? obp1 : obp0;

            // Position and tile number.
            auto sprX = m_oam->GetBytePrivileged(base);
            auto sprY = m_oam->GetBytePrivileged(base + 1);
            tileIdx = m_oam->GetBytePrivileged(base + 2);
            if(doubleHeight) tileIdx &= 0b11111110; // LSB is ignored for 8x16 sprites.

            // Check to see if the sprite should even be drawn.
            if(sprX - 8 < scx || sprX + 8 > scx) continue; // X value out of range.
            if(!sprPriority && pixelColor > 0) continue; // Background/window overlaps sprite.

            // Figure out which pixel should be considered.
            auto sprHeight = doubleHeight ? 16 : 8;
            tilePX = (sprFlipX ? 8 : 0) - scx - (sprX - 8);
            tilePY = (sprFlipY ? sprHeight : 0) - scy - (sprY - 16);
            if(doubleHeight && tilePY >= 8) {
                // Move to the second tile of the 8x16 sprite.
                tilePY %= 8;
                tileIdx |= 0b1;
            }
            lineOffset = tilePY * 2;

            pixelDataLow = m_videoRam->GetBytePrivileged(m_vramTileBase + (tileIdx * 16) + lineOffset);
            pixelDataHigh = m_videoRam->GetBytePrivileged(m_vramTileBase + (tileIdx * 16) + lineOffset + 1);
            tileShift = 7 - tilePX;
            pixelDataCombined = (((pixelDataHigh >> tileShift) & 0b1) << 1)
                              | ((pixelDataLow >> tileShift) & 0b1);

            if(pixelDataCombined == 0) continue; // Transparent pixel.

            pixelColor = (sprPalette >> (pixelDataCombined * 2)) & 0b11;
            Plot(pixelColor, pos);
        }
    }
#pragma clang diagnostic pop
}
