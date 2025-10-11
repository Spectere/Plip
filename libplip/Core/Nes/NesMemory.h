/* NesMemory.h
 *
 * Manages the NES's memory layout.
 */

#pragma once

#include "NesApuRegisters.h"
#include "NesMapper.h"
#include "NesPpuRegisters.h"
#include "../../Memory/PlipMemoryMap.h"

namespace Plip::Core::Nes {
    class NesMemory final : public PlipMemoryMap {
    public:
        static constexpr auto WorkRamAddress = 0x0000;
        static constexpr auto WorkRamLength = 0x0800;
        static constexpr auto PpuRegisterAddress = 0x2000;
        static constexpr auto ApuRegisterAddress = 0x4000;
        static constexpr auto CartridgeSpace = 0x4020;

        NesMemory(PlipMemory* workRam, NesPpuRegisters* ppuRegisters, NesApuRegisters* apuRegisters, NesMapper* mapper, PlipMemory* ppuRam);

        [[nodiscard]] uint8_t GetByte(uint32_t address, bool privileged) const override;
        uint32_t GetLength() override { return 1 << 16; }
        void SetByte(uint32_t address, uint8_t value, bool privileged) override;

    private:
        PlipMemory* m_workRam;
        NesPpuRegisters* m_ppuRegisters;
        NesApuRegisters* m_apuRegisters;
        NesMapper* m_mapper;

        PlipMemory* m_ppuRam;
    };
}
