/* SharpLr35902.h
 *
 * An implementation of a Sharp LR35902 CPU (SM83 core).
 */

#include "SharpLr35902.h"

using Plip::Cpu::SharpLr35902;

SharpLr35902::SharpLr35902(const long hz, PlipMemoryMap *memoryMap) : PlipCpu(hz, memoryMap) {
}

void SharpLr35902::Cycle() {
}

unsigned long SharpLr35902::GetPc() const {
    return m_registers.PC;
}

void SharpLr35902::Reset(uint32_t pc) {
    m_registers.A = 0;
    m_registers.F = 0;
    m_registers.B = 0;
    m_registers.C = 0;
    m_registers.D = 0;
    m_registers.E = 0;
    m_registers.H = 0;
    m_registers.L = 0;

    m_registers.SP = 0;
    m_registers.PC = 0;
}

std::map<std::string, Plip::DebugValue> SharpLr35902::GetRegisters() const {
    return {
        { "A", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.A)) },
        { "B", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.B)) },
        { "C", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.C)) },
        { "D", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.D)) },
        { "E", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.E)) },
        { "H", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.H)) },
        { "L", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_registers.L)) },
        { "SP", DebugValue(DebugValueType::Int16Be, static_cast<uint64_t>(m_registers.SP)) },
        { "PC", DebugValue(DebugValueType::Int16Be, static_cast<uint64_t>(m_registers.PC)) },
        { "ZF", DebugValue(static_cast<bool>(m_registers.F & 0x80)) },
        { "NF", DebugValue(static_cast<bool>(m_registers.F & 0x40)) },
        { "HF", DebugValue(static_cast<bool>(m_registers.F & 0x20)) },
        { "CF", DebugValue(static_cast<bool>(m_registers.F & 0x10)) },
    };
}
