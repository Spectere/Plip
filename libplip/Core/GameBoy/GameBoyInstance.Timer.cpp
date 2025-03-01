/* GameBoyInstance.Timer.cpp
 *
 * A GameBoy emulation core.
 */

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::Timer_Cycle() {
    ++m_timerSystem;

    const auto lastWrittenValue = m_memory->LastWrittenValue;
    switch(m_memory->LastWrittenAddress) {
        case 0xFF04: {
            m_timerSystem = 0;
            break;
        }

        case 0xFF07: {
            m_ioRegisters->SetByte(IOReg_TimerControl, 0b11111 | lastWrittenValue);
            break;
        }
    }

    // Set DIV.
    m_ioRegisters->SetByte(IOReg_Divider, m_timerSystem >> 6);
}

void GameBoyInstance::Timer_Init() {
    m_timerSystem = 0;
}

std::map<std::string, Plip::DebugValue> GameBoyInstance::Timer_GetDebugInfo() const {
    return {
        { "DIV", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IOReg_Divider))) },
    };
}
