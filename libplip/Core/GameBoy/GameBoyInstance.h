/* GameBoyInstance.h
 *
 * A GameBoy core for Plip.
 */

#pragma once

#include "../PlipCore.h"
#include "../../Cpu/SharpLr35902/SharpLr35902.h"
#include "../../Memory/PlipMemoryMap.h"
#include "../../Memory/PlipMemoryRam.h"
#include "../../Memory/PlipMemoryRom.h"

namespace Plip::Core::GameBoy {
    class GameBoyInstance : public PlipCore {
    public:
        GameBoyInstance(PlipAudio *audio, PlipInput *input, PlipVideo *video, PlipConfig *config);
        ~GameBoyInstance();

        void Delta(long ns) override;
        std::string DumpRegisters() override;
        PlipError Load(const std::string &path) override;
        void Redraw() override;

        const int BaseClockRate = 4194304;
        const int InputRight = 0;
        const int InputLeft = 1;
        const int InputUp = 2;
        const int InputDown = 3;
        const int InputA = 4;
        const int InputB = 5;
        const int InputSelect = 6;
        const int InputStart = 7;

    private:
        inline void Plot(uint8_t color, int pos) {
            switch(color) {
                case 0b00:
                    // White
                    m_videoFmt.plot(m_videoBuffer, pos, 255, 255, 255, 255);
                    break;
                case 0b01:
                    // Light Gray
                    m_videoFmt.plot(m_videoBuffer, pos, 172, 172, 172, 255);
                    break;
                case 0b10:
                    // Dark Gray
                    m_videoFmt.plot(m_videoBuffer, pos, 86, 86, 86, 255);
                    break;
                case 0b11:
                    // Black
                    m_videoFmt.plot(m_videoBuffer, pos, 0, 0, 0, 255);
                    break;
                default:
                    // Huh. Okay, make it pretty obvious that something's up.
                    m_videoFmt.plot(m_videoBuffer, pos, 255, 0, 0, 255);
                    break;
            }
        }

        inline void IncrementTimer() {
            m_timer = m_ioRegisters->GetByte(m_regTima);

            if(++m_timer == 0) {
                // Timer overflowed. Reset TIMA to TMA and schedule an interrupt.
                m_timerIntScheduled = true;
                m_timer = m_ioRegisters->GetByte(m_regTma);
            }

            m_ioRegisters->SetByte(m_regTima, m_timer);
        }

        uint16_t GetRomBankCount();
        void InitCartRam();
        void ReadCartFeatures();
        void ReadInput();
        void RegisterInput();

        // GameBoyInstance.mbc.cpp
        void MbcInit();
        void MbcCycle(PlipMemoryValue lastWrite);
        void Mbc1Cycle(PlipMemoryValue lastWrite);

        // GameBoyInstance.video.cpp
        void VideoCycle();
        [[nodiscard]] bool VideoHBlank() const;
        void VideoModePostTransition();
        void VideoModePreTransition();
        bool VideoOamSearch();
        void VideoSetMemoryPermissions();
        bool VideoVBlank();
        bool VideoVidGen();

        // Core
        bool m_bootRomFlag = false;
        std::string m_bootRomPath;
        Plip::Cpu::SharpLr35902 *m_cpu;
        long m_cycleRemaining = 0;
        int m_dotCyclesRemaining = 0;
        const int m_dotsPerCycle = 4;
        uint8_t *m_videoBuffer;
        size_t m_videoBufferSize;
        PlipVideoFormatInfo m_videoFmt {};

        // Cartridge Information
        enum MemoryBankController {
            None,
            Mbc1,
            Mbc2,
            Mbc3,
            Mbc5,
            Mbc6,
            Mbc7,
            Mmm01,
            PocketCamera,
            BandaiTama5,
            HuC1,
            HuC3
        };

        MemoryBankController m_mbc = None;
        bool m_hasBattery = false;
        bool m_hasCamera = false;
        bool m_hasRam = false;
        bool m_hasRtc = false;
        bool m_hasSensor = false;

        uint16_t m_romBanks = 0;
        uint16_t m_cartRamBanks = 0;

        // MBC
        uint8_t m_mbcMode = 0;
        uint8_t m_mbcRomBank = 0;
        uint8_t m_mbcRamBank = 0;
        bool m_mbcRamEnabled = false;

        // Memory
        Plip::PlipMemoryRom *m_bootRom = nullptr;
        Plip::PlipMemoryRom *m_rom = nullptr;
        uint8_t m_unusableContents[0x60] {};
        Plip::PlipMemoryRom *m_unusable;

        Plip::PlipMemoryRam *m_cartRam = nullptr;
        Plip::PlipMemoryRam *m_workRam;
        Plip::PlipMemoryRam *m_videoRam;
        Plip::PlipMemoryRam *m_oam;
        Plip::PlipMemoryRam *m_ioRegisters;
        Plip::PlipMemoryRam *m_highRam;

        static const uint32_t m_addrRom = 0x0000;
        static const uint32_t m_addrBankedRom = 0x4000;
        static const uint32_t m_addrVideoRam = 0x8000;
        static const uint32_t m_addrWorkRam = 0xC000;
        static const uint32_t m_addrCartRam = 0xA000;
        static const uint32_t m_addrEchoRam = 0xE000;
        static const uint32_t m_addrOam = 0xFE00;
        static const uint32_t m_addrUnusable = 0xFEA0;
        static const uint32_t m_addrRegisters = 0xFF00;
        static const uint32_t m_addrHighRam = 0xFF80;

        // Input
        uint8_t m_keypad = 0;

        const uint32_t m_regJoypad = 0xFF00 - m_addrRegisters;

        // Timer
        static const uint32_t m_regDivider = 0xFF04 - m_addrRegisters;
        static const uint32_t m_regTima = 0xFF05 - m_addrRegisters;
        static const uint32_t m_regTma = 0xFF06 - m_addrRegisters;
        static const uint32_t m_regTac = 0xFF07 - m_addrRegisters;

        uint8_t m_divider = 0;
        uint8_t m_dividerTick = 0;
        uint8_t m_timer = 0;
        uint8_t m_timerTick;
        bool m_timerIntScheduled = false;

        // Video
        static const uint32_t m_regLcdControl = 0xFF40 - m_addrRegisters;
        static const uint32_t m_regLcdcStatus = 0xFF41 - m_addrRegisters;
        static const uint32_t m_regScy = 0xFF42 - m_addrRegisters;   // R/W scroll Y
        static const uint32_t m_regScx = 0xFF43 - m_addrRegisters;   // R/W - scroll X
        static const uint32_t m_regLy = 0xFF44 - m_addrRegisters;    // R
        static const uint32_t m_regLyCompare = 0xFF45 - m_addrRegisters;   // R/W
        static const uint32_t m_regOamDmaTransfer = 0xFF46 - m_addrRegisters;  // R/W
        static const uint32_t m_regBgp = 0xFF47 - m_addrRegisters;   // R/W - BG/window palette
        static const uint32_t m_regObp0 = 0xFF48 - m_addrRegisters;  // R/W - object palette 0
        static const uint32_t m_regObp1 = 0xFF49 - m_addrRegisters;  // R/W - object palette 0
        static const uint32_t m_regWy = 0xFF4A - m_addrRegisters;    // R/W - window Y
        static const uint32_t m_regWx = 0xFF7B - m_addrRegisters;    // R/W - window X - 7

        static const uint32_t m_vramTileBase = 0x8000 - m_addrVideoRam;
        static const uint32_t m_vramTileBlockOffset = 0x0800;
        static const uint32_t m_vramBgBase = 0x9800 - m_addrVideoRam;
        static const uint32_t m_vramBgBlockOffset = 0x0400;

        static const uint8_t m_maxSpritesPerScanline = 10;

        static const uint32_t m_videoHBlankTime = 0;
        static const uint32_t m_videoOamScanTime = 80;
        static const uint32_t m_videoScanlineTime = 376;
        static const uint32_t m_videoVBlankTime = 4560;

        static const uint32_t m_videoModeHBlank = 0;
        static const uint32_t m_videoModeVBlank = 1;
        static const uint32_t m_videoModeOamSearch = 2;
        static const uint32_t m_videoModePicGen = 3;

        static const uint32_t m_screenWidth = 160;
        static const uint32_t m_screenHeight = 144;

        enum VidGenStage {
            BackgroundScrolling,
            Drawing
        };

        int m_dotCount = 0;
        uint8_t *m_spriteList;
        uint8_t *m_spriteListSorted;
        uint8_t m_spriteListIdx = 0;
        int m_videoCoincidence = 0;
        int m_videoMode = 0;
        uint8_t m_videoLastLcdc = 0;
        int m_videoLx = 0;
        int m_videoLy = 0;
        VidGenStage m_vidGenStage = BackgroundScrolling;
        int m_vidGenTick = 0;
    };
}
