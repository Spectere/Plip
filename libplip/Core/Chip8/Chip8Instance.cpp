/* Chip8Instance.cpp
 *
 * A CHIP-8 implementation for Plip.
 *
 * (hahaha, more like PLIP-8 amirite)
 */

#include <cmath>

#include "Chip8Instance.h"

#include "../../PlipIo.h"

using Plip::Core::Chip8::Chip8Instance;

Chip8Instance::Chip8Instance(PlipAudio *audio, PlipInput *input, PlipVideo *video, const PlipKeyValuePairCollection &config)
: PlipCore(audio, input, video, config) {
    m_ram = new PlipMemoryRam(RamSize);
    m_memory->AddBlock(m_ram);

    m_input->AddInput(0x0, PlipInputDefinition(PlipInputType::Digital, "0"), { .digital = false });
    m_input->AddInput(0x1, PlipInputDefinition(PlipInputType::Digital, "1"), { .digital = false });
    m_input->AddInput(0x2, PlipInputDefinition(PlipInputType::Digital, "2"), { .digital = false });
    m_input->AddInput(0x3, PlipInputDefinition(PlipInputType::Digital, "3"), { .digital = false });
    m_input->AddInput(0x4, PlipInputDefinition(PlipInputType::Digital, "4"), { .digital = false });
    m_input->AddInput(0x5, PlipInputDefinition(PlipInputType::Digital, "5"), { .digital = false });
    m_input->AddInput(0x6, PlipInputDefinition(PlipInputType::Digital, "6"), { .digital = false });
    m_input->AddInput(0x7, PlipInputDefinition(PlipInputType::Digital, "7"), { .digital = false });
    m_input->AddInput(0x8, PlipInputDefinition(PlipInputType::Digital, "8"), { .digital = false });
    m_input->AddInput(0x9, PlipInputDefinition(PlipInputType::Digital, "9"), { .digital = false });
    m_input->AddInput(0xA, PlipInputDefinition(PlipInputType::Digital, "A"), { .digital = false });
    m_input->AddInput(0xB, PlipInputDefinition(PlipInputType::Digital, "B"), { .digital = false });
    m_input->AddInput(0xC, PlipInputDefinition(PlipInputType::Digital, "C"), { .digital = false });
    m_input->AddInput(0xD, PlipInputDefinition(PlipInputType::Digital, "D"), { .digital = false });
    m_input->AddInput(0xE, PlipInputDefinition(PlipInputType::Digital, "E"), { .digital = false });
    m_input->AddInput(0xF, PlipInputDefinition(PlipInputType::Digital, "F"), { .digital = false });

    m_video->ResizeOutput(ScreenWidth, ScreenHeight, 1.0, 1.0);
    m_videoFormat = PlipVideo::GetFormatInfo(m_video->GetFormat());
    m_videoOutput = malloc(ScreenWidth * ScreenHeight * m_videoFormat.pixelWidth);

    m_cpu = new Cpu::Chip8(ClockRate, m_memory, CharacterSet, m_input);
    m_cpu->Reset(0x200);

    m_cycleTime = m_cpu->GetCycleTime();

    m_channels = PlipAudio::Channels;
    m_sampleRate = m_audio->GetSampleRate();
    m_audioBufferLength = m_audio->GetBufferLength();
    m_audioBuffer.resize(m_audioSamplesPerGeneration * m_channels);
    m_waveform = m_config.GetValue("waveform", 0);
    m_audioBufferFillThreshold = m_audioBufferLength * m_channels;

    if(m_waveform == 1) {
        // Square waves are kinda loud.
        m_waveformVolume /= 2;
    }

    const auto cycles = WaveformHz / static_cast<double>(m_sampleRate);
    m_phaseDelta = cycles * 2.0 * M_PI;
    m_phaseAdjustment = m_audioSamplesPerGeneration / 256;  // I really don't know why this fixes the audio wonkiness.
                                                            // I have to be missing something, right?
}

Chip8Instance::~Chip8Instance() {
    free(m_videoOutput);
}

void Chip8Instance::Delta(const long ns) {
    m_cycleRemaining += ns;

    do {
        m_cycleRemaining -= m_cpu->Cycle();

        if(const auto audioQueueFilled = m_audio->GetQueueSize(); audioQueueFilled < m_audioBufferFillThreshold) {
            if(m_cpu->IsAudioPlaying()) {
                m_audio->Enqueue(m_waveform == 0 ? GenerateSine() : GenerateSquare());
            } else {
                m_audio->Enqueue(GenerateSilence());
            }
        }

        m_delayRemaining -= m_cycleTime;
        if(m_delayRemaining <= 0) {
            m_cpu->DelayTimer();
            m_delayRemaining += DelayTimerTick;
        }
    } while(m_cycleTime < m_cycleRemaining);

    Draw();
}

void Chip8Instance::Draw() const {
    const auto buffer = m_cpu->GetVideo();

    for(auto y = 0; y < ScreenHeight; y++) {
        const auto row = buffer[y];
        for(auto x = 0; x < ScreenWidth; x++) {
            const auto bit = row >> (63 - x) & 0x1;
            m_videoFormat.plot(m_videoOutput, y * ScreenWidth + x,
                    bit * 255, bit * 255, bit * 255);
        }
    }

    m_video->BeginDraw();
    m_video->Draw(m_videoOutput);
    m_video->EndDraw();
}

std::vector<float> Chip8Instance::GenerateSilence() {
    for(auto i = 0; i < m_audioSamplesPerGeneration; i++) {
        for(auto c = 0; c < m_channels; c++) {
            m_audioBuffer[i * m_channels + c] = 0;
        }
        m_phase += m_phaseDelta;  // Keep the wave generator going to prevent clicks.
    }

    return m_audioBuffer;
}

std::vector<float> Chip8Instance::GenerateSine() {
    for(auto i = 0; i < m_audioSamplesPerGeneration; i++) {
        for(auto c = 0; c < m_channels; c++) {
            m_audioBuffer[i * m_channels + c] = static_cast<float>(m_waveformVolume * std::sin(m_phase));
        }
        m_phase += m_phaseDelta;
    }
    m_phase -= m_phaseDelta * m_phaseAdjustment;
    return m_audioBuffer;
}

std::vector<float> Chip8Instance::GenerateSquare() {
    for(auto i = 0; i < m_audioSamplesPerGeneration; i++) {
        for(auto c = 0; c < m_channels; c++) {
            m_audioBuffer[i * m_channels + c] = static_cast<float>(WaveformVolume * std::sin(m_phase)) > 0 ? m_waveformVolume : -m_waveformVolume;
        }
        m_phase += m_phaseDelta;
    }
    m_phase -= m_phaseDelta * m_phaseAdjustment;
    return m_audioBuffer;
}

std::map<std::string, std::map<std::string, Plip::DebugValue>> Chip8Instance::GetDebugInfo() const {
    return {
        { "CPU", m_cpu->GetRegisters() }
    };
}

std::vector<uint64_t> Chip8Instance::GetPcs() const {
    return { m_cpu->GetPc() };
}

bool Chip8Instance::IsPcAt(const uint64_t pc) const {
    return m_cpu->GetPc() == pc;
}

Plip::PlipError Chip8Instance::Load(const std::string &path) {
    using io = PlipIo;
    if(!io::FileExists(path)) return PlipError::FileNotFound;

    const auto size = io::GetSize(path);
    const auto data = io::ReadFile(path, size);

    // Update titlebar.
    m_video->SetTitle("CHIP-8: " + io::GetFilename(path));

    // Zero RAM.
    for(auto i = 0; i < RamSize; i++)
        m_memory->SetByte(i, 0x00);

    // Load program.
    auto ramByte = ProgramOffset;
    auto dataByte = 0;
    while(dataByte < size && ramByte < RamSize)
        m_memory->SetByte(ramByte++, data[dataByte++]);

    // Write character set.
    WriteCharacterSet(CharacterSet);

    return PlipError::Success;
}

// ReSharper disable once CppDFAConstantParameter
void Chip8Instance::WriteCharacterSet(const uint32_t address) const {
    for(auto i = 0; i < m_charsetLength; i++)
        m_memory->SetByte(address + i, m_charset[i]);
}
