/* NesPpuRegisters.h
 *
 * Implements the NES's PPU registers.
 */

#pragma once

#include "../../Memory/PlipMemory.h"

namespace Plip::Core::Nes {
    enum class PpuRegister {
        PpuControl = 0x00,
        PpuMask = 0x01,
        PpuStatus = 0x02,
        OamAddress = 0x03,
        OamData = 0x04,
        PpuScroll = 0x05,
        PpuAddress = 0x06,
        PpuData = 0x07,
    };

    class NesPpuRegisters final : public PlipMemory {
    public:
        [[nodiscard]] uint8_t GetByte(const uint32_t address, const bool privileged) override {
            return GetByte(static_cast<PpuRegister>(address), privileged);
        }

        void SetByte(const uint32_t address, const uint8_t value, const bool privileged) override {
            SetByte(static_cast<PpuRegister>(address), value, privileged);
        }

        [[nodiscard]] uint8_t GetByte(PpuRegister ppuRegister, bool privileged) const;
        [[nodiscard]] uint32_t GetLength() override;
        void SetByte(PpuRegister ppuRegister, uint8_t value, bool privileged);

    private:
        uint8_t m_ppuControl {};
        uint8_t m_ppuMask {};
        uint8_t m_ppuStatus {};
        uint8_t m_oamAddress {};
        uint8_t m_oamData {};
        uint8_t m_ppuScrollX {};
        uint8_t m_ppuScrollY {};
        uint16_t m_ppuAddress {};
        uint8_t m_ppuData {};

        bool m_ppuScrollWriteY {};
        bool m_ppuAddressWriteLsb {};
    };
}
