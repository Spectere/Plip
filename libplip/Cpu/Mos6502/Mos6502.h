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
        Ricoh2A03,

        /** The CMOS version by Western Design Center. This is a newer revision that
         *  supports some additional instructions, but does not support the unofficial
         *  opcodes from the NMOS versions. */
        Wdc65C02,
    };

    class Mos6502 : public PlipCpu {
    public:
        Mos6502(long hz, PlipMemoryMap* memoryMap, Mos6502Version version);
        virtual ~Mos6502() = default;

        [[nodiscard]] unsigned long GetPc() const override;
        long Step() override;
        [[nodiscard]] std::map<std::string, DebugValue> GetRegisters() const override;
        void Reset(uint32_t pc) override;

        bool CpuHasCrashed() const { return OpKillExecuted; }

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
        static constexpr auto ModeIndexedIndirect = 0b000 << 2;
        static constexpr auto ModeZeroPage = 0b001 << 2;
        static constexpr auto ModeImmediate = 0b010 << 2;
        static constexpr auto ModeAbsolute = 0b011 << 2;
        static constexpr auto ModeIndirectIndexed = 0b100 << 2;
        static constexpr auto ModeZeroPageReg = 0b101 << 2;
        static constexpr auto ModeAbsoluteY = 0b110 << 2;
        static constexpr auto ModeAbsoluteX = 0b111 << 2;

        bool OpKillExecuted {};

        uint8_t AddBinary(uint8_t value);
        uint8_t AddDecimal(uint8_t value);
        uint8_t SubDecimal(uint8_t value);
        [[nodiscard]] bool AluIsInDecimalMode() const { return m_version != Mos6502Version::Ricoh2A03 && m_registers.GetDecimalMode(); }
        void CallAbsolute(uint16_t addr);
        void CallReturn();
        long DecodeAndExecute();
        void DecodeAndExecuteNmosUnofficial();
        void DecodeAndExecuteWdc65C02Extended();
        [[nodiscard]] uint16_t FetchAddress(int addressingMode, const bool alwaysUseY = false, const bool forcePenalty = false);
        uint8_t FetchFromMemory(int addressingMode, bool alwaysUseY = false, bool useAccumulator = false, const bool forcePenalty = false);
        void JumpRelative(int8_t rel);
        void OpAddWithCarry(uint8_t value);
        void OpBitwiseAnd(uint8_t value);
        void OpBitwiseOr(uint8_t value);
        void OpBitwiseXor(uint8_t value);
        void OpCompare(uint8_t value);
        uint8_t OpDecrement(uint16_t addr);
        uint8_t OpIncrement(uint16_t addr);
        [[nodiscard]] uint8_t OpLogicalShiftLeft(uint8_t value);
        [[nodiscard]] uint8_t OpLogicalShiftRight(uint8_t value);
        uint8_t OpRotateLeft(uint8_t value);
        uint8_t OpRotateRight(uint8_t value);
        void OpSubtractWithBorrow(uint8_t value);
        void StoreToMemory(int addressingMode, uint8_t value, bool swapXY = false);
    };
}
