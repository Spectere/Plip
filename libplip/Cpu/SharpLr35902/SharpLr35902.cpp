/* SharpLr35902.h
 *
 * An implementation of a Sharp LR35902 CPU (SM83 core).
 */

#include "SharpLr35902.h"
#include "../../PlipSupport.h"

using Plip::Cpu::SharpLr35902;

SharpLr35902::SharpLr35902(const long hz, PlipMemoryMap *memoryMap, const bool gbcMode)
    : PlipCpu(hz, memoryMap), m_baseSpeed(hz), m_gbcMode(gbcMode) { }

unsigned long SharpLr35902::GetPc() const {
    return m_registers.PC;
}

void SharpLr35902::Reset(const uint32_t pc) {
    m_registers.A = 0;
    m_registers.F = 0;
    m_registers.B = 0;
    m_registers.C = 0;
    m_registers.D = 0;
    m_registers.E = 0;
    m_registers.H = 0;
    m_registers.L = 0;

    m_registers.SP = 0;
    m_registers.PC = pc;

    m_ime = SharpLr35902ImeState::Disabled;
}

std::map<std::string, Plip::DebugValue> SharpLr35902::GetDebugInfo() const {
    return {
        { "Halt", DebugValue(m_halt) },
        { "IME", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ime)) },
        { "Double Speed", DebugValue(m_doubleSpeed) },
    };
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
        { "SP", DebugValue(DebugValueType::Int16Le, static_cast<uint64_t>(m_registers.SP)) },
        { "PC", DebugValue(DebugValueType::Int16Le, static_cast<uint64_t>(m_registers.PC)) },
        { "CF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, SharpLr35902Registers::CarryFlagBit))) },
        { "HF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, SharpLr35902Registers::HalfCarryFlagBit))) },
        { "NF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, SharpLr35902Registers::SubtractFlagBit))) },
        { "ZF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, SharpLr35902Registers::ZeroFlagBit))) },
    };
}

long SharpLr35902::Step() {
    if(m_changingSpeed) {
        if(--m_speedChangeTimer == 0) {
            m_changingSpeed = false;
            m_doubleSpeed = !m_doubleSpeed;
            SetHz(m_doubleSpeed ? m_baseSpeed * 2 : m_baseSpeed);
        }
        
        return 0;
    }
    
    const auto cycleCount = DecodeAndExecute();

    if(m_enableInterrupts && m_ime == SharpLr35902ImeState::Disabled) {
        m_ime = SharpLr35902ImeState::PendingEnable;
        m_enableInterrupts = false;
    } else if(m_ime == SharpLr35902ImeState::PendingEnable) {
        m_ime = SharpLr35902ImeState::Enabled;
    }

    return cycleCount;
}
