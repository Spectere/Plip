/* GameBoyInstance.Timer.cpp
 *
 * A Game Boy emulation core.
 */

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::Timer_Cycle() {
    // Read TAC
    const auto timerControl = m_ioRegisters->GetByte(IoRegister::TimerControl);
    const auto timaEnabled = timerControl & 0b100;
    const auto timaClock = timerControl & 0b11;

    // TIMA Reload
    if(m_timaQueueReload && m_memory->LastWrittenAddress == IoRegistersAddress + static_cast<uint32_t>(IoRegister::TimerCounter)) {
        // Pretend that nothing happened, including the upcoming reload and interrupt.
        m_timaQueueReload = false;
        m_ioRegisters->SetByte(IoRegister::TimerControl, 0);
        RegisterWriteServiced();
    } else if(m_timaQueueReload) {
        m_ioRegisters->SetByte(IoRegister::TimerControl, m_ioRegisters->GetByte(IoRegister::TimerModulo));
        RaiseInterrupt(Cpu::SharpLr35902Interrupt::Timer);
        m_timaQueueReload = false;
    }
    
    // DIV
    m_timerSystem += 4;

    if(m_memory->LastWrittenAddress == IoRegistersAddress + static_cast<uint32_t>(IoRegister::Divider)) {
        m_timerSystem = 0;
        RegisterWriteServiced();
    }
    m_ioRegisters->SetTimerDivider(m_timerSystem >> 8);
    
    // TIMA Increment
    const auto frequencyBit = Timer_GetFrequencyBit(timaClock);
    const auto thisBitResult = ((m_timerSystem >> frequencyBit) & 0b1) && timaEnabled;
    if(m_timerBitLast && !thisBitResult) {
        // Falling edge detected.
        auto tima = m_ioRegisters->GetByte(IoRegister::TimerCounter);
        if(++tima == 0) m_timaQueueReload = true;
        m_ioRegisters->SetByte(IoRegister::TimerCounter, tima);
    }
    m_timerBitLast = thisBitResult;
}

int GameBoyInstance::Timer_GetFrequencyBit(const int clockSelect) {
    switch(clockSelect) {
        case 0b00: return 9;
        case 0b01: return 3;
        case 0b10: return 5;
        case 0b11: return 7;
        default: throw PlipEmulationException("Invalid timer clock.");
    }
}

void GameBoyInstance::Timer_Init() {
    m_timerSystem = 0;
}

std::map<std::string, Plip::DebugValue> GameBoyInstance::Timer_GetDebugInfo() const {
    return {
        { "DIV", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::Divider))) },
        { "TIMA", DebugValue(DebugValueType::Int16Le, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::TimerCounter))) },
        { "TMA", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::TimerModulo))) },
        { "TAC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::TimerControl))) },
        { "TIMA Reload", DebugValue(m_timaQueueReload) },
    };
}
