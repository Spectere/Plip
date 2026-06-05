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
        const auto mixLeft  = (m_channel1->PanLeft && m_channelEnable[0]  ? m_channel1Last : 0)
                            + (m_channel2->PanLeft && m_channelEnable[1]  ? m_channel2Last : 0)
                            + (m_channel3->PanLeft && m_channelEnable[2]  ? m_channel3Last : 0)
                            + (m_channel4->PanLeft && m_channelEnable[3]  ? m_channel4Last : 0);

        const auto mixRight = (m_channel1->PanRight && m_channelEnable[0] ? m_channel1Last : 0)
                            + (m_channel2->PanRight && m_channelEnable[1] ? m_channel2Last : 0)
                            + (m_channel3->PanRight && m_channelEnable[2] ? m_channel3Last : 0)
                            + (m_channel4->PanRight && m_channelEnable[3] ? m_channel4Last : 0);

        m_audioBuffer.push_back((mixLeft / ApuMixDivisor) * ApuFinalMixMultiplier);
        m_audioBuffer.push_back((mixRight / ApuMixDivisor) * ApuFinalMixMultiplier);
    }
}

void GameBoyInstance::APU_Send() {
    m_audio->Enqueue(m_audioBuffer);
    m_audioBuffer.clear();
}

// Channels 1 and 2
float GameBoyInstance::APU_Clock_Channel(PulseChannel* channel) {
    channel->ClockChannel();

    if(!channel->Enabled) return 0.0f;

    const auto waveform = PulseChannel::Waveforms[channel->DutyCycle] >> (7 - channel->DutyPosition) & 0b1;
    return (waveform ? 1.0f : -1.0f) * static_cast<float>(channel->Volume);
}

// Channel 3
float GameBoyInstance::APU_Clock_Channel(WaveChannel* channel) {
    constexpr auto DacAmplify = 3.0f;

    channel->ClockChannel();

    if(!channel->Enabled) return 0.0f;

    auto sample = channel->GetCurrentSample();
    if(channel->OutputLevel == 0) {
        // Set to digital zero.
        sample = 0;
    } else {
        // Shift toward digital zero.
        sample >>= channel->OutputLevel - 1;
    }

    // Convert to a float, shift, and amplify.
    const auto value = (static_cast<float>(sample) / 2.0f) - 1.0f;
    return value * DacAmplify;
}

// Channel 4
float GameBoyInstance::APU_Clock_Channel(NoiseChannel* channel) {
    channel->ClockChannel();

    if(!channel->Enabled) return 0.0f;

    const auto value = channel->LFSRBits & 0b1;
    return (value ? 1.0f : -1.0f) * static_cast<float>(channel->Volume);
}
