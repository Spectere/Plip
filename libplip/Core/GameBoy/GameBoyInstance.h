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
        GameBoyInstance(PlipAudio *audio, PlipInput *input, PlipVideo *video);

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
        int VideoCycle();

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

        // Input
        uint8_t m_keypad = 0;

        // Video
        const uint32_t m_addrLcdControl = 0xFF40;
        const uint32_t m_lcdcStatus = 0xFF41;
        const uint32_t m_scy = 0xFF42;   // R/W scroll Y
        const uint32_t m_scx = 0xFF43;   // R/W - scroll X
        const uint32_t m_lcdcY = 0xFF44;  // R
        const uint32_t m_lyCompare = 0xFF45;   // R/W
        const uint32_t m_oamDmaTransfer = 0xFF46;  // R/W
        const uint32_t m_bgp = 0xFF47;   // R/W - BG/window palette
        const uint32_t m_obp0 = 0xFF48;  // R/W - object palette 0
        const uint32_t m_obp1 = 0xFF49;  // R/W - object palette 0
        const uint32_t m_wy = 0xFF4A;    // R/W - window Y
        const uint32_t m_wx = 0xFF7B;    // R/W - window X - 7

        const uint32_t m_vramTileBase = 0x8000;
        const uint32_t m_vramTileBlockOffset = 0x800;

        int m_videoMode = 0;
    };
}
