/* GameBoyInstance.Audio.cpp
 *
 * Handles functions related to emulating the Game Boy's APU.
 */

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::APU_Clock() {
    // Is APU enabled?
    if(!m_ioRegisters->Audio_GetEnabled()) {
        // No? Reset and hold duty steps at 0.
        m_apuCh1DutyStep = m_apuCh2DutyStep = 0;
        return;
    }
    
    // Channel 1 - Pulse with Sweep
    if(m_ioRegisters->Audio_IsChannelTriggered(1)) {
        APU_ConfigurePulse(1, m_ioRegisters->Audio_GetCh1Data());
    } else if(m_apuActiveChannels & 0b0001) {
        APU_GeneratePulse(1);
    }

    // Channel 2 - Pulse
    if(m_ioRegisters->Audio_IsChannelTriggered(2)) {
        APU_ConfigurePulse(1, m_ioRegisters->Audio_GetCh2Data());
    } else if(m_apuActiveChannels & 0b0010) {
        APU_GeneratePulse(2);
    }

    // Channel 3 - Wave
    if(m_ioRegisters->Audio_IsChannelTriggered(3)) {
        APU_ConfigureWave(m_ioRegisters->Audio_GetCh3Data());
    } else if(m_apuActiveChannels & 0b0100) {
        APU_GenerateWave();
    }

    // Channel 4 - Noise
    if(m_ioRegisters->Audio_IsChannelTriggered(4)) {
        APU_ConfigureNoise(m_ioRegisters->Audio_GetCh4Data());
    } else if(m_apuActiveChannels & 0b1000) {
        APU_GenerateNoise();
    }

    // Update channel state.
    m_ioRegisters->Audio_SetChannelState(m_apuActiveChannels);

    // Should we mix and output?
    if(const auto audioQueueFilled = m_audio->GetQueueSize(); audioQueueFilled < m_apuAudioBufferFillThreshold) {
        APU_Mix();
        APU_Output();
    }
}

void GameBoyInstance::APU_ConfigureNoise(const AudioNoiseDefinition& definition) {
    m_apuCh4 = definition;
    m_apuActiveChannels |= 0b1000;
}

void GameBoyInstance::APU_ConfigurePulse(const int channel, const AudioPulseDefinition& definition) {
    AudioPulseDefinition* def;
    if(channel == 1) {
        m_apuCh1 = definition;
        def = &m_apuCh1;
    } else {
        m_apuCh2 = definition;
        def = &m_apuCh2;
    }
    m_apuActiveChannels |= 1 << (channel - 1);
    def->CurrentPeriod = def->Period;
    def->CurrentLengthTimer = def->InitialLengthTimer;
}

void GameBoyInstance::APU_ConfigureWave(const AudioWaveDefinition& definition) {
    m_apuCh3 = definition;
    m_apuActiveChannels |= 0b0100;
}

void GameBoyInstance::APU_GenerateNoise() {
    
}

void GameBoyInstance::APU_GeneratePulse(const int channel) {
    auto stream = (channel == 1) ? m_apuCh1Buffer : m_apuCh2Buffer;
    auto definition = (channel == 1) ? m_apuCh1 : m_apuCh2;
    const auto dutyStep = (channel == 1) ? &m_apuCh1DutyStep : &m_apuCh2DutyStep;

    for(auto i = 0; i < m_apuPulseCycles; ++i) {
        // Output the waveform.
        
        
        // Advance the clock.
        if(++definition.CurrentPeriod > 0x7FF) {
            // Reset period and increment the duty step.
            definition.CurrentPeriod = definition.Period;
            ++(*dutyStep);
        }
    }
}

void GameBoyInstance::APU_GenerateWave() {
    
}

void GameBoyInstance::APU_Mix() {
    m_apuAudioBuffer.clear();
    for(auto i = 0; i < m_apuCh1Buffer.size(); i++) {
        auto result = (m_apuCh1Buffer[i] + m_apuCh2Buffer[i] + m_apuCh3Buffer[i] + m_apuCh4Buffer[i]) / 4;
        m_apuAudioBuffer.push_back(result);
    }
    
    m_apuCh1Buffer.clear();
    m_apuCh2Buffer.clear();
    m_apuCh3Buffer.clear();
    m_apuCh4Buffer.clear();
}

void GameBoyInstance::APU_Output() {
    m_audio->Enqueue(m_apuAudioBuffer);
    m_apuAudioBuffer.clear();
}
