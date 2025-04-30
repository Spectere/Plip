/* Mos6502.h
 *
 * An implementation of a MOS 6502 CPU (and its derivatives).
 */

#pragma once

#include "Mos6502Registers.h"
#include "../PlipCpu.h"

namespace Plip::Cpu {
    enum class Mos6502Version {
        /** The original NMOS version by MOS Technologies. */
        Mos6502,

        /** The Ricoh 2A03. This version lacks decimal mode. */
        //Ricoh2A03,

        /** The CMOS version by Western Design Center. This is a newer revision that
         *  supports some additional instructions, but does not support the unofficial
         *  opcodes from the NMOS versions. */
        //Wdc65C02,
    };

    class Mos6502 : public PlipCpu {
    public:
        Mos6502(long hz, PlipMemoryMap* memoryMap, Mos6502Version version);
        virtual ~Mos6502() = default;

        [[nodiscard]] unsigned long GetPc() const override;
        long Step() override;
        [[nodiscard]] std::map<std::string, DebugValue> GetRegisters() const override;
        void Reset(uint32_t pc) override;

    protected:
        static constexpr uint16_t StackLocation = 0x0100;
        static constexpr uint16_t NmiVectorHigh = 0xFFFB;
        static constexpr uint16_t NmiVectorLow = 0xFFFA;
        static constexpr uint16_t ResetVectorHigh = 0xFFFD;
        static constexpr uint16_t ResetVectorLow = 0xFFFC;
        static constexpr uint16_t InterruptVectorHigh = 0xFFFF;
        static constexpr uint16_t InterruptVectorLow = 0xFFFE;

        [[nodiscard]] uint16_t GetNmiVector() const { return (m_memory->GetByte(NmiVectorHigh) << 8) | m_memory->GetByte(NmiVectorLow); }
        [[nodiscard]] uint16_t GetResetVector() const { return (m_memory->GetByte(ResetVectorHigh) << 8) | m_memory->GetByte(ResetVectorLow); }
        [[nodiscard]] uint16_t GetInterruptVector() const { return (m_memory->GetByte(InterruptVectorHigh) << 8) | m_memory->GetByte(InterruptVectorLow); }

        Mos6502Registers m_registers {};
        Mos6502Version m_version {};

    private:
        long DecodeAndExecute();
    };
}
