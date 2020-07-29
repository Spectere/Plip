/* Chip8.h
 *
 * An implementation of a CHIP-8 CPU.
 */

#pragma once

#include <random>

#include "../PlipCpu.h"
#include "../../Input/PlipInput.h"
#include "../../Video/PlipVideo.h"

namespace Plip::Cpu {
    class Chip8 : public PlipCpu {
    public:
        Chip8(long hz, PlipMemoryMap* memoryMap, uint16_t charset, Plip::PlipInput *input);

        void Cycle() override;
        void DelayTimer();
        uint64_t* GetVideo() { return m_videoBuffer; }
        [[nodiscard]] bool IsAudioPlaying() const { return m_timerAudio >= 2; }
        void Reset(uint32_t pc) override;

        static const int VideoSize = 32;  // 64 x 32

    private:
        std::string DumpRegisters();

        inline uint16_t Fetch() {
            uint8_t high = m_memory->GetByte(m_pc++);
            uint8_t low = m_memory->GetByte(m_pc++);
            return (high << 8) + low;
        }

        static inline uint16_t GetAddress(uint16_t b) { return b & 0xFFF; }
        static inline uint8_t GetReg1(uint16_t b) { return (b >> 8) & 0xF; }
        static inline uint8_t GetReg2(uint16_t b) { return (b >> 4) & 0xF; }
        static inline uint8_t GetValue(uint16_t b) { return b & 0xFF; }

        void Op0NNN(uint16_t address);
        void Op00E0();
        void Op00EE();
        void Op1NNN(uint16_t address);
        void Op2NNN(uint16_t address);
        void Op3XNN(uint8_t reg, uint8_t value);
        void Op4XNN(uint8_t reg, uint8_t value);
        void Op5XY0(uint8_t left, uint8_t right);
        void Op6XNN(uint8_t reg, uint8_t value);
        void Op7XNN(uint8_t reg, uint8_t value);
        void Op8XYO(uint8_t left, uint8_t right, uint8_t op);
        void Op9XY0(uint8_t left, uint8_t right);
        void OpANNN(uint16_t address);
        void OpBNNN(uint16_t address);
        void OpCXNN(uint8_t reg, uint8_t value);
        void OpDXYN(uint8_t xReg, uint8_t yReg, uint8_t size);
        void OpEXOO(uint8_t reg, uint8_t op);
        void OpFXOO(uint8_t reg, uint8_t op);

        static const int StackSize = 12;

        std::default_random_engine m_rng;

        Plip::PlipInput *m_input;
        uint64_t *m_videoBuffer;

        uint16_t m_charsetAddress;
        uint8_t m_timerAudio = 0;
        uint8_t m_timerDelay = 0;
        uint16_t m_sp = 0;
        uint16_t m_pc = 0;
        uint16_t m_i = 0;
        uint8_t m_reg[16] {};
        uint16_t m_stack[StackSize] {};

        bool m_waitForKey = false;
        uint8_t m_keyRegister = 0;
    };
}
