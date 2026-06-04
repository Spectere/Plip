/* GameBoyInstance.Video.cpp
*
 * Handles functions related to emulating the Game Boy's APU.
 */

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::APU_Init() {
    m_audioSampleRate = m_audio->GetSampleRate();
    const auto plipBufferLength = m_audio->GetBufferLength();

    m_audioCyclesPerSample = (static_cast<float>(BaseClockRate) / 2.0f) / static_cast<float>(m_audioSampleRate);
    m_apuOutputSendThreshold = plipBufferLength * PlipAudio::Channels;

    m_audioBuffer.reserve(m_apuOutputSendThreshold);

    // Store the pointers to the channels.
    m_channel1 = m_ioRegisters->Audio_GetChannelPulse1();
    m_channel2 = m_ioRegisters->Audio_GetChannelPulse2();
    m_channel3 = m_ioRegisters->Audio_GetChannelWave();
    m_channel4 = m_ioRegisters->Audio_GetChannelNoise();

    APU_Reset();
}

void GameBoyInstance::APU_Reset() {
    // TODO
}

void GameBoyInstance::APU_Cycle() {
    // BaseClock / 4 == Pulse
    if(m_totalCpuCycles % (m_apuClockDivisor * 2) == 0) {
        m_channel1Last = APU_Clock_Channel(m_channel1);
        m_channel2Last = APU_Clock_Channel(m_channel2);
    }

    // BaseClock / 2 == Wave
    m_channel3Last = APU_Clock_Channel(m_channel3);

    // BaseClock / 16 == Noise
    if(m_totalCpuCycles % (m_apuClockDivisor * 8) == 0) {
        m_channel4Last = APU_Clock_Channel(m_channel4);
    }

    if(m_audio->IsActive() && ++m_audioAccumulator >= m_audioCyclesPerSample) {
        m_audioAccumulator -= m_audioCyclesPerSample;

        // TODO: This is ugly, and probably inefficient. Bitwise math on m_ioRegisters->Audio_GetChannelPanning might be better.
        const auto mixLeft  = (m_channel1->PanLeft  ? m_channel1Last : 0)
                            + (m_channel2->PanLeft  ? m_channel2Last : 0)
                            + (m_channel3->PanLeft  ? m_channel3Last : 0)
                            + (m_channel4->PanLeft  ? m_channel4Last : 0);

        const auto mixRight = (m_channel1->PanRight ? m_channel1Last : 0)
                            + (m_channel2->PanRight ? m_channel2Last : 0)
                            + (m_channel3->PanRight ? m_channel3Last : 0)
                            + (m_channel4->PanRight ? m_channel4Last : 0);

        // We still want to generate audio even if the audio engine is disabled. Just don't push it anywhere.
        m_audioBuffer.push_back(mixLeft / ApuMixDivisor);
        m_audioBuffer.push_back(mixRight / ApuMixDivisor);
    }
}

void GameBoyInstance::APU_Send() {
    m_audio->Enqueue(m_audioBuffer);
    m_audioBuffer.clear();
}

// Channels 1 and 2
float GameBoyInstance::APU_Clock_Channel(PulseChannel* channel) const {
    channel->ClockChannel(m_ioRegisters->Audio_GetDivApuCounter());

    if(!channel->Enabled) return 0.0f;

    const auto waveform = PulseChannel::Waveforms[channel->DutyCycle];
    return static_cast<float>(waveform >> (7 - channel->DutyPosition) & 0b1) * static_cast<float>(channel->Volume);
}

// Channel 3
float GameBoyInstance::APU_Clock_Channel(WaveChannel* channel) const {
    channel->ClockChannel(m_ioRegisters->Audio_GetDivApuCounter());

    return 0.0f;
}

// Channel 4
float GameBoyInstance::APU_Clock_Channel(NoiseChannel* channel) const {
    channel->ClockChannel(m_ioRegisters->Audio_GetDivApuCounter());

    return 0.0f;
}
