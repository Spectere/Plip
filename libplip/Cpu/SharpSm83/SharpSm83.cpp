/* SharpSm83.h
 *
 * An implementation of a Sharp SM83-based CPU.
 */

#include "SharpSm83.h"

#include "PlipSupport.h"

using Plip::Cpu::SharpSm83;

SharpSm83::SharpSm83(const long hz, PlipMemoryMap *memoryMap, const bool gbcMode)
    : PlipCpu(hz, memoryMap), m_baseSpeed(hz), m_gbcMode(gbcMode) { }

unsigned long SharpSm83::GetPc() const {
    return m_registers.PC;
}

void SharpSm83::Reset(const uint32_t pc) {
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

    m_ime = false;
}

std::map<std::string, Plip::DebugValue> SharpSm83::GetDebugInfo() const {
    return {
        { "Halt", DebugValue(m_halt) },
        { "IME", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ime)) },
        { "Double Speed", DebugValue(m_doubleSpeed) },
        { "Address Bus", DebugValue(DebugValueType::Int16Le, m_addressBus) },
        { "Data Bus", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_dataBus)) },
    };
}

std::map<std::string, Plip::DebugValue> SharpSm83::GetRegisters() const {
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
        { "CF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, SharpSm83Registers::CarryFlagBit))) },
        { "HF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, SharpSm83Registers::HalfCarryFlagBit))) },
        { "NF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, SharpSm83Registers::SubtractFlagBit))) },
        { "ZF", DebugValue(static_cast<bool>(BIT_TEST(m_registers.F, SharpSm83Registers::ZeroFlagBit))) },
    };
}

long SharpSm83::Cycle() {
    if(m_changingSpeed) {
        if(--m_speedChangeTimer == 0) {
            m_changingSpeed = false;
            m_doubleSpeed = !m_doubleSpeed;
            SetHz(m_doubleSpeed ? m_baseSpeed * 2 : m_baseSpeed);
        }

        return 0;
    }

    const auto cycleCount = DecodeAndExecute();

    // Interrupt Enabling
    // We'll keep this here, since it makes testing easier (sets the IME state immediately after the EI, instead
    // of immediately before the next instruction--easier to inspect).
    if(m_state == SharpSm83State::Decode && m_imeDelay > 0 && --m_imeDelay == 0) {
        m_ime = true;
    }

    return cycleCount;
}

void SharpSm83::Step() {
    // The original PlipCpu single-stepping isn't suited for cycle-accuracy, as it
    // requires us to know exactly how many cycles the instruction is going to take
    // on the first cycle. With our current implementation, we won't know how many
    // cycles a conditional branch will take until the second cycle.
    if(++m_tCycle >= MCycleLength) {
        Cycle();
        m_tCycle = 0;
    }
}
