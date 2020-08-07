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
            if(m_videoMode == 1 && m_videoLy < m_screenHeight)
                m_videoMode++;  // only transition to 1 after the last hblank
            VideoModePostTransition();
        }

        auto stat = m_memory->GetByte(m_regLcdcStatus) & 0b11111000;
        stat |= (m_videoCoincidence << 2) | m_videoMode;
        m_memory->SetByte(m_regLcdcStatus, stat);
    }

    bool GameBoyInstance::VideoHBlank() {
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
        bool coincidenceIntEnabled;

        // New mode.
        switch(m_videoMode) {
            case m_videoModeHBlank:
                m_oam->SetWritable(true);
                m_videoRam->SetWritable(true);
                break;
            case m_videoModeVBlank:
                m_oam->SetWritable(true);
                m_videoRam->SetWritable(true);
                m_dotCount = 0;
                m_cpu->Interrupt(INTERRUPT_VBLANK);
                break;
            case m_videoModeOamSearch:
                m_oam->SetWritable(false);
                m_videoRam->SetWritable(true);
                memset(m_spriteList, 0x00, m_maxSpritesPerScanline);
                m_dotCount = 0;
                m_videoCoincidence = m_videoLy == m_memory->GetByte(m_regLyCompare) ? 1 : 0;
                coincidenceIntEnabled = BIT_TEST(m_memory->GetByte(m_regLcdcStatus), 6);

                if(coincidenceIntEnabled) {
                    // Coincidence flag interrupt enabled.
                    m_cpu->Interrupt(INTERRUPT_LCDSTAT);
                }

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
        return false;
    }
}
