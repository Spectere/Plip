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
        PlipError Load(const std::string &path) override;

        const int BaseClockRate = 4194304 / 4;
        const int InputRight = 0;
        const int InputLeft = 1;
        const int InputUp = 2;
        const int InputDown = 3;
        const int InputA = 4;
        const int InputB = 5;
        const int InputSelect = 6;
        const int InputStart = 7;

    private:
        uint16_t GetRomBankCount();
        void InitCartRam();
        void InitMbc();
        void ReadInput();
        void RegisterInput();

        // GameBoyInstance.video.cpp
        void VideoCycle();
        bool VideoHBlank();
        void VideoModePostTransition();
        void VideoModePreTransition();
        bool VideoOamSearch();
        bool VideoVBlank();
        bool VideoVidGen();

        // Core
        Plip::Cpu::SharpLr35902 *m_cpu;
        long m_cycleTime {};
        long m_cycleRemaining = 0;
        bool m_running = false;
        int m_dotCyclesRemaining = 0;
        const int m_dotsPerCycle = 4;

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

        // Memory
        Plip::PlipMemoryRom *m_rom = nullptr;
        uint8_t m_unusableContents[0x60] {};
        Plip::PlipMemoryRom *m_unusable;

        Plip::PlipMemoryRam *m_cartRam = nullptr;
        Plip::PlipMemoryRam *m_workRam;
        Plip::PlipMemoryRam *m_videoRam;
        Plip::PlipMemoryRam *m_oam;
        Plip::PlipMemoryRam *m_ioRegisters;
        Plip::PlipMemoryRam *m_highRam;

        const uint32_t m_addrRom = 0x0000;
        const uint32_t m_addrVideoRam = 0x8000;
        const uint32_t m_addrWorkRam = 0xC000;
        const uint32_t m_addrCartRam = 0xA000;
        const uint32_t m_addrEchoRam = 0xE000;
        const uint32_t m_addrOam = 0xFE00;
        const uint32_t m_addrUnusable = 0xFEA0;
        const uint32_t m_addrRegisters = 0xFF00;
        const uint32_t m_addrHighRam = 0xFF80;

        // Input
        uint8_t m_keypad = 0;

        // Video
        static const uint32_t m_regLcdControl = 0xFF40;
        static const uint32_t m_regLcdcStatus = 0xFF41;
        static const uint32_t m_regScy = 0xFF42;   // R/W scroll Y
        static const uint32_t m_regScx = 0xFF43;   // R/W - scroll X
        static const uint32_t m_regLy = 0xFF44;    // R
        static const uint32_t m_regLyCompare = 0xFF45;   // R/W
        static const uint32_t m_regOamDmaTransfer = 0xFF46;  // R/W
        static const uint32_t m_regBgp = 0xFF47;   // R/W - BG/window palette
        static const uint32_t m_regObp0 = 0xFF48;  // R/W - object palette 0
        static const uint32_t m_regObp1 = 0xFF49;  // R/W - object palette 0
        static const uint32_t m_regWy = 0xFF4A;    // R/W - window Y
        static const uint32_t m_regWx = 0xFF7B;    // R/W - window X - 7

        static const uint32_t m_vramTileBase = 0x8000;
        static const uint32_t m_vramTileBlockOffset = 0x0800;
        static const uint32_t m_vramBgBase = 0x9800;
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

        int m_dotCount = 0;
        uint8_t *m_spriteList;
        uint8_t m_spriteListIdx = 0;
        int m_videoCoincidence = 0;
        int m_videoMode = 0;
        int m_videoLy = 0;
    };
}
