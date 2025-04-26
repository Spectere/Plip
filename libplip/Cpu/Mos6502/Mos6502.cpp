/* Mos6502.cpp
 *
 * An implementation of a MOS 6502 CPU (and its derivatives).
 */

#include "Mos6502.h"

using Plip::Cpu::Mos6502;

Mos6502::Mos6502(const long hz, PlipMemoryMap* memoryMap, const Mos6502Version version)
    : PlipCpu(hz, memoryMap), m_version(version) {
    Mos6502::Reset(0);  // Ensure that the PC is set properly.
}

unsigned long Mos6502::GetPc() const {
    return m_registers.PC;
}

std::map<std::string, Plip::DebugValue> Mos6502::GetRegisters() const {
    return {
        { "A", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.A)) },
        { "X", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.X)) },
        { "Y", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.Y)) },
        { "SP", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.SP)) },
        { "PC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.PC)) },
        { "CF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::CarryFlagBit))) },
        { "ZF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::ZeroFlagBit))) },
        { "ID", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::InterruptDisableBit))) },
        { "DM", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::DecimalModeBit))) },
        { "BF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::BreakCommandBit))) },
        { "OF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::OverflowFlagBit))) },
        { "NF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, Mos6502Registers::NegativeFlagBit))) },
    };
}

void Mos6502::Reset([[maybe_unused]] const uint32_t pc) {
    m_registers.A = 0;
    m_registers.F = 0b00100000;
    m_registers.X = 0;
    m_registers.Y = 0;

    m_registers.SP = 0;

    // The 6502 gets its reset vector from memory. Ignore the function parameter.
    m_registers.PC = GetResetVector();
}

long Mos6502::Step() {
    return DecodeAndExecute();
}
