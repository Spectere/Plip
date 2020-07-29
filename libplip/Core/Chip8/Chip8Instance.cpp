/* Chip8Instance.cpp
 *
 * A CHIP-8 implementation for Plip.
 *
 * (hahaha, more like PLIP-8 amirite)
 */

#include <cmath>

#include "Chip8Instance.h"
#include "../../PlipIo.h"

namespace Plip::Core::Chip8 {
    Chip8Instance::Chip8Instance(PlipAudio *audio, PlipInput *input, PlipVideo *video)
    : Plip::PlipCore(audio, input, video) {
        using pa = Plip::PlipAudio;

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

        m_video->Resize(ScreenWidth, ScreenHeight);
        m_videoFormat = Plip::PlipVideo::GetFormatInfo(m_video->GetFormat());
        m_videoOutput = malloc(ScreenWidth * ScreenHeight * m_videoFormat.pixelWidth);

        m_cpu = new Cpu::Chip8(ClockRate, m_memory, CharacterSet, m_input);
        m_cpu->Reset(0x200);

        m_cycleTime = m_cpu->GetCycleTime();

        m_channels = pa::Channels;
        m_sampleRate = pa::SampleRate;
        m_sampleCount = m_audio->GetBufferSize();

        auto cycles = (double)SineHz / (double)m_sampleRate;
        m_delta = cycles * 2.0 * M_PI;
    }

    Chip8Instance::~Chip8Instance() {
        free(m_videoOutput);
    }

    void Chip8Instance::Delta(long ns) {
        m_cycleRemaining += ns;

        do {
            m_cpu->Cycle();
            m_cycleRemaining -= m_cycleTime;

            if(m_audio->GetQueueSize() < 1024) {
                // This is a pretty lame implementation, but it should be good
                // enough for this.
                if(m_cpu->IsAudioPlaying()) {
                    m_audio->Enqueue(GenerateSine());
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

    void Chip8Instance::Draw() {
        auto buffer = m_cpu->GetVideo();

        for(auto y = 0; y < ScreenHeight; y++) {
            auto row = buffer[y];
            for(auto x = 0; x < ScreenWidth; x++) {
                auto bit = row >> (63 - x) & 0x1;
                m_videoFormat.plot(m_videoOutput, y * ScreenWidth + x,
                        bit * 255, bit * 255, bit * 255);
            }
        }

        m_video->BeginDraw();
        m_video->EndDraw();
        m_video->Draw(m_videoOutput);
        m_video->Render();
    }

    std::vector<float> Chip8Instance::GenerateSilence() {
        using pa = Plip::PlipAudio;

        std::vector<float> res;
        for(auto i = 0; i < m_sampleCount; i++) {
            for(auto c = 0; c < m_channels; c++)
                res.push_back(0);
            m_angle += m_delta;  // Keep the wave generator going to prevent clicks.
        }

        return res;
    }

    std::vector<float> Chip8Instance::GenerateSine() {
        using pa = Plip::PlipAudio;

        std::vector<float> res;
        for(auto i = 0; i < m_sampleCount; i++) {
            for(auto c = 0; c < m_channels; c++)
                res.push_back(SineVolume * std::sin(m_angle));
            m_angle += m_delta;
        }

        return res;
    }

    PlipError Chip8Instance::Load(const std::string &path) {
        using io = Plip::PlipIo;
        if(!io::FileExists(path)) return PlipError::FileNotFound;

        auto size = io::GetSize(path);
        auto data = io::ReadFile(path, size);

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

    void Chip8Instance::WriteCharacterSet(uint32_t address) {
        for(auto i = 0; i < m_charsetLength; i++)
            m_memory->SetByte(address + i, m_charset[i]);
    }
}
