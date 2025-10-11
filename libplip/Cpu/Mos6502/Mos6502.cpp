/* Mos6502.cpp
 *
 * An implementation of a MOS 6502 CPU (and its derivatives).
 */

#include "Mos6502.h"

using Plip::Cpu::Mos6502;

Mos6502::Mos6502(const long hz, PlipMemoryMap* memoryMap, const Mos6502Version version)
    : PlipCpu(hz, memoryMap), m_version(version) {
    // These are only set on a cold start.
    m_registers.A = 0;
    m_registers.X = 0;
    m_registers.Y = 0;
    m_registers.F = 0b00110100;

    // Ensure that the PC is set properly.
    Mos6502::Reset(0);
}

unsigned long Mos6502::GetPc() const {
    return m_registers.PC;
}

std::map<std::string, Plip::DebugValue> Mos6502::GetRegisters() const {
    return {
        { "A", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.A)) },
        { "X", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.X)) },
        { "Y", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.Y)) },
        { "S", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.S)) },
        { "PC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.PC)) },
        { "F(C)", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::CarryFlagBit))) },
        { "F(Z)", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::ZeroFlagBit))) },
        { "F(I)", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::InterruptDisableBit))) },
        { "F(D)", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::DecimalModeBit))) },
        { "F(B)", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::BreakCommandBit))) },
        { "F(O)", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::OverflowFlagBit))) },
        { "F(N)", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::NegativeFlagBit))) },
    };
}

void Mos6502::Reset([[maybe_unused]] const uint32_t pc) {
    // Reset the stack pointer.
    if(m_version == Mos6502Version::Ricoh2A03) {
        m_registers.S = 0xFD;
    } else {
        m_registers.S = 0xFF;
    }

    // Set interrupt disable.
    m_registers.F |= 0b00000100;

    // The 6502 gets its reset vector from memory. Ignore the function parameter.
    m_registers.PC = GetResetVector();
}

long Mos6502::Step() {
    if(OpKillExecuted) return __LONG_MAX__;
    return DecodeAndExecute();
}
