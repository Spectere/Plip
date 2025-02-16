/* GameBoyInstance.h
 *
 * A GameBoy emulation core.
 */

#pragma once

#include "../PlipCore.h"
#include "../../Cpu/SharpLr35902/SharpLr35902.h"
#include "../../Memory/PlipMemoryRam.h"
#include "../../Memory/PlipMemoryRom.h"

namespace Plip::Core::GameBoy {
    class GameBoyInstance : public PlipCore {
    public:
        GameBoyInstance(PlipAudio* audio, PlipInput* input, PlipVideo* video, const PlipKeyValuePairCollection &config);
        ~GameBoyInstance() override;

        void Delta(long ns) override;
        [[nodiscard]] std::map<std::string, std::map<std::string, DebugValue>> GetDebugInfo() const override;
        bool IsPcAt(uint64_t pc) const override;
        PlipError Load(const std::string &path) override;

    private:
        static constexpr uint32_t BaseClockRate = 4194304;

        Cpu::SharpLr35902 *m_cpu;

        // Static memory addresses
        static constexpr auto BaseRomAddress = 0x0000;
        static constexpr auto BankedRomAddress = 0x4000;
        static constexpr auto VideoRamAddress = 0x8000;
        static constexpr auto WorkRamAddress = 0xC000;
        static constexpr auto CartRamAddress = 0xA000;
        static constexpr auto EchoRamAddress = 0xE000;
        static constexpr auto OamAddress = 0xFE00;
        static constexpr auto UnusableAddress = 0xFEA0;
        static constexpr auto IoRegistersAddress = 0xFF00;
        static constexpr auto HighRamAddress = 0xFF80;

        // System memory map
        uint8_t m_unusableContents[0x60] {};

        PlipMemoryRom* m_bootRom;
        PlipMemoryRam* m_videoRam = new PlipMemoryRam(0x2000);
        PlipMemoryRam* m_workRam = new PlipMemoryRam(0x2000);
        PlipMemoryRam* m_oam = new PlipMemoryRam(0x100);
        PlipMemoryRom* m_unusable = new PlipMemoryRom(m_unusableContents, 0x60);
        PlipMemoryRam* m_ioRegisters = new PlipMemoryRam(0x80);
        PlipMemoryRam* m_highRam = new PlipMemoryRam(0x80);
    };
}
