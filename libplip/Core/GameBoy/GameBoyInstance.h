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

        Plip::Cpu::SharpLr35902 *m_cpu;
        bool m_running = false;

        MemoryBankController m_mbc = None;
        bool m_hasBattery = false;
        bool m_hasCamera = false;
        bool m_hasRam = false;
        bool m_hasRtc = false;
        bool m_hasSensor = false;

        uint16_t m_romBanks = 0;
        uint16_t m_cartRamBanks = 0;

        Plip::PlipMemoryRom *m_rom = nullptr;
        uint8_t m_unusableContents[0x60] {};
        Plip::PlipMemoryRom *m_unusable;

        Plip::PlipMemoryRam *m_cartRam = nullptr;
        Plip::PlipMemoryRam *m_workRam;
        Plip::PlipMemoryRam *m_videoRam;
        Plip::PlipMemoryRam *m_oam;
        Plip::PlipMemoryRam *m_ioRegisters;
        Plip::PlipMemoryRam *m_highRam;

        uint8_t m_keypad = 0;
    };
}
