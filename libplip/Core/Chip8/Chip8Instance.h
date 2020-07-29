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
#include "../../Input/PlipInput.h"
#include "../../Memory/PlipMemoryRam.h"

namespace Plip::Core::Chip8 {
    class Chip8Instance : public PlipCore {
    public:
        Chip8Instance(PlipAudio *audio, PlipInput *input, PlipVideo *video);

        void Delta(long ns) override;
        PlipError Load(const std::string &path) override;

        static const uint32_t ClockRate = 500;
        static constexpr long ClockTime = (1000 / ClockRate) * 1000000;
        static const uint32_t CharacterSet = 0x000;
        static const uint32_t ProgramOffset = 0x200;
        static const uint32_t RamSize = 0x1000;

    private:
        PlipMemoryRam *m_ram;
        std::unordered_map<int, PlipInputDefinition> m_inputList;

        void WriteCharacterSet(uint32_t address);

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
