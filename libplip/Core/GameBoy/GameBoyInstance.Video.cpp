/* GameBoyInstance.Video.cpp
 *
 * Simulated graphics hardware for the GameBoy core.
 */

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
            if(m_videoMode == 1 && m_videoLy <= m_screenHeight)
                m_videoMode++;  // only transition to 1 after the last HBlank
            VideoModePostTransition();
        }

        auto stat = m_memory->GetByte(m_regLcdcStatus) & 0b11111000;
        stat |= (m_videoCoincidence << 2) | m_videoMode;
        m_memory->SetByte(m_regLcdcStatus, stat);
    }

    bool GameBoyInstance::VideoHBlank() const {
        return m_dotCount < m_videoScanlineTime;
    }

    bool GameBoyInstance::VideoOamSearch() {
        if(m_dotCount != 0) return m_dotCount < m_videoOamScanTime;

        auto spriteHeight = ((m_memory->GetByte(m_regLcdControl) >> 2) & 1) ? 16 : 8;

        // Just do everything at once since the OAM will be locked at this point.
        m_spriteListIdx = 0;
        for(auto sprIdx = 0; sprIdx < 0; sprIdx++) {
            auto sprAddr = m_addrOam + (4 * sprIdx);
            auto sprY = m_memory->GetByte(sprAddr);
            auto sprX = m_addrOam + (4 * sprIdx);

            if(sprX == 0 || sprX >= m_screenWidth + 8) continue;
            if(m_videoLy >= sprY - spriteHeight || m_videoLy < sprY - (16 - spriteHeight)) {
                m_spriteList[m_spriteListIdx++] = sprIdx;
            }

            if(m_spriteListIdx == m_maxSpritesPerScanline) break;
        }

        return m_dotCount < m_videoOamScanTime;
    }

    void GameBoyInstance::VideoModePostTransition() {
        uint8_t stat = m_memory->GetByte(m_regLcdcStatus);

        // New mode.
        switch(m_videoMode) {
            case m_videoModeHBlank:
                m_oam->SetWritable(true);
                m_videoRam->SetWritable(true);
                if(BIT_TEST(stat, 3)) m_cpu->Interrupt(INTERRUPT_LCDSTAT);
                break;

            case m_videoModeVBlank:
                m_oam->SetWritable(true);
                m_videoRam->SetWritable(true);
                m_dotCount = 0;

                // Plot the current buffer to the screen and flip buffers.
                m_video->BeginDraw();
                m_video->Draw(m_videoBuffer);
                m_video->EndDraw();
                m_video->Render();

                m_cpu->Interrupt(INTERRUPT_VBLANK);
                if(BIT_TEST(stat, 4)) m_cpu->Interrupt(INTERRUPT_LCDSTAT);
                break;

            case m_videoModeOamSearch:
                m_oam->SetWritable(false);
                m_videoRam->SetWritable(true);
                memset(m_spriteList, 0x00, m_maxSpritesPerScanline);
                m_dotCount = 0;
                m_videoCoincidence = m_videoLy == m_memory->GetByte(m_regLyCompare) ? 1 : 0;

                if(BIT_TEST(stat, 5))  // OAM interrupt
                    m_cpu->Interrupt(INTERRUPT_LCDSTAT);

                if(m_videoCoincidence && BIT_TEST(stat, 6))  // LYC=LY interrupt.
                    m_cpu->Interrupt(INTERRUPT_LCDSTAT);

                break;

            case m_videoModePicGen:
                m_oam->SetWritable(false);
                m_videoRam->SetWritable(false);
                m_dotCount = 0;
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

    bool GameBoyInstance::VideoVBlank() {
        if(m_dotCount > 0)
            if(m_dotCount % (m_videoOamScanTime + m_videoScanlineTime) == 0)
                m_videoLy++;

        return m_dotCount < m_videoVBlankTime;
    }

    bool GameBoyInstance::VideoVidGen() {
        uint8_t scx, scy;
        uint8_t wx, wy;
        uint8_t bgp, obp0, obp1;
        uint8_t lcdc, stat;
        uint8_t tileX, tileY, tilePX, tilePY, tileIdx, tile, pixelColor;
        uint8_t pixelData, lineOffset;
        uint16_t tileDataAddr, tileMapAddr;
        auto pos = (m_videoLy * m_screenWidth) + m_videoLx;

        switch(m_vidGenStage) {
            case BackgroundScrolling:
                // Pause the dot clock to simulate the background shifter.
                scx = m_memory->GetByte(m_regScx);
                if(m_vidGenTick++ > scx % 8) {
                    m_vidGenTick = 0;
                    m_vidGenStage = Drawing;
                }
                break;

            case Drawing:
                //if(--m_vidGenTick > 0) break;

                scx = m_memory->GetByte(m_regScx);
                scy = m_memory->GetByte(m_regScy);
                bgp = m_memory->GetByte(m_regBgp);
                obp0 = m_memory->GetByte(m_regObp0);
                obp1 = m_memory->GetByte(m_regObp1);

                // BG palette: FF47 (m_regBgp)
                // Object palettes: FF48, FF49 (m_regObp0, m_regObp1)
                lcdc = m_memory->GetByte(m_regLcdControl);
                if(!BIT_TEST(lcdc, 7)) break;

                if(BIT_TEST(lcdc, 0)) { // BG/Window Display Enabled
                    tileDataAddr = BIT_TEST(lcdc, 4) ? 0x8800 : 0x8000;
                    tileMapAddr = BIT_TEST(lcdc, 3) ? 0x9800 : 0x9C00;
                    tileX = (scx + m_videoLx) % 32;
                    tileY = (scy + m_videoLy) % 32;
                    tilePX = (scx + m_videoLx) % 8;
                    tilePY = (scy + m_videoLy) % 8;
                    tileIdx = tileY * 32 * 2;
                    lineOffset = tilePX >= 4 ? 1 : 0;
                    tile = m_memory->GetByte(tileMapAddr + tileIdx);
                    pixelData = m_memory->GetByte(tileDataAddr * tile + lineOffset);

                    pixelData = (pixelData >> ((tilePX & 0b11) * 2)) & 0b11;
                    pixelColor = (bgp >> (pixelData * 2)) & 0b11;
                    Plot(pixelColor, pos);

                    if(BIT_TEST(lcdc, 5)) {
                        wx = m_memory->GetByte(m_regWx) - 7;
                        wy = m_memory->GetByte(m_regWy);

                        if(!(wx > 166 || wy > 143) || m_videoLx >= wx || m_videoLy >= wy) {
                            tileMapAddr = BIT_TEST(lcdc, 6) ? 0x9800 : 0x9C00;
                            tileX = (m_videoLx - wx) % 32;
                            tileY = (m_videoLy - wy) % 32;
                            tilePX = (m_videoLx - wx) % 8;
                            tilePY = (m_videoLy - wy) % 8;
                            tileIdx = tileY * 32 * 2;
                            lineOffset = tilePX >= 4 ? 1 : 0;
                            pixelData = m_memory->GetByte(tileDataAddr * tile + lineOffset);

                            // Draw window.  TODO: implement me.
                        }
                    }
                } else {
                    // BG/Window display disabled. Draw a white pixel.
                    Plot(0b11, pos);
                }

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
}
