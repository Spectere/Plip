/* Chip8Instance.h
 *
 * A CHIP-8 implementation for Plip.
 *
 * (hahaha, more like PLIP-8 amirite)
 */

#pragma once

#include <string>

#include "../PlipCore.h"
#include "../../PlipError.h"
#include "../../PlipInstance.h"
#include "../../Cpu/Chip8/Chip8.h"
#include "../../Input/PlipInput.h"
#include "../../Memory/PlipMemoryRam.h"

namespace Plip::Core::Chip8 {
    class Chip8Instance : public PlipCore {
    public:
        Chip8Instance(PlipAudio *audio, PlipInput *input, PlipVideo *video);
        ~Chip8Instance();

        void Delta(long ns) override;
        PlipError Load(const std::string &path) override;

        static const uint32_t ClockRate = 500;
        static const uint32_t CharacterSet = 0x100;
        static const long DelayTimerTick = 16666666;
        static const uint32_t ProgramOffset = 0x200;
        static const uint32_t RamSize = 0x1000;
        static const int SineHz = 440;
        static constexpr double SineVolume = 0.25;

    private:
        double m_angle = 0.0;
        double m_delta;
        int m_channels;
        int m_sampleCount;
        int m_sampleRate;

        Cpu::Chip8 *m_cpu;
        long m_cycleRemaining = 0;
        long m_delayRemaining = DelayTimerTick;
        long m_cycleTime = 0;
        PlipMemoryRam *m_ram;
        std::unordered_map<int, PlipInputDefinition> m_inputList;
        Plip::PlipVideoFormatInfo m_videoFormat {};
        void *m_videoOutput;

        void Draw();
        std::vector<float> GenerateSilence();
        std::vector<float> GenerateSine();
        void WriteCharacterSet(uint32_t address);

        static const int ScreenWidth = 64;
        static const int ScreenHeight = 32;

        static constexpr uint8_t m_charset[] {
                0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                0x20, 0x60, 0x20, 0x20, 0x70, // 1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
        static constexpr uint32_t m_charsetLength = 5 * 16;
    };
}
