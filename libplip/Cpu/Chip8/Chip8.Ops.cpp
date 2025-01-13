/* Chip8.Ops.cpp
 *
 * CHIP-8 opcodes.
 */

#include <iomanip>
#include <sstream>

#include "../../PlipEmulationException.h"

#include "Chip8.h"

namespace Plip::Cpu {
    void Chip8::Op0NNN(uint16_t address) {
        // RCA 1802 routine. Not implemented.
    }

    void Chip8::Op00E0() const {
        // Clears the screen.
        for(auto i = 0; i < VideoSize; i++)
            m_videoBuffer[i] = 0;
    }

    void Chip8::Op00EE() {
        // Returns from a subroutine.
        if(m_sp == 0) {
            std::stringstream ex;
            ex << "stack underflow\n\n" << DumpRegisters();
            throw PlipEmulationException(ex.str().c_str());
        }

        m_pc = m_stack[--m_sp];
    }

    void Chip8::Op1NNN(const uint16_t address) {
        // Jumps to an address.
        m_pc = address;
    }

    void Chip8::Op2NNN(const uint16_t address) {
        // Calls a subroutine.
        if(m_sp >= StackSize) {
            std::stringstream ex;
            ex << "stack overflow\n\n" << DumpRegisters();
            throw PlipEmulationException(ex.str().c_str());
        }

        m_stack[m_sp++] = m_pc;
        m_pc = address;
    }

    void Chip8::Op3XNN(const uint8_t reg, const uint8_t value) {
        // Skips the next instruction if reg == value.
        if(m_reg[reg] == value) m_pc += 2;
    }

    void Chip8::Op4XNN(const uint8_t reg, const uint8_t value) {
        // Skips the next instruction if reg != value.
        if(m_reg[reg] != value) m_pc += 2;
    }

    void Chip8::Op5XY0(const uint8_t left, const uint8_t right) {
        // Skips the next instruction if left == right.
        if(m_reg[left] == m_reg[right]) m_pc += 2;
    }

    void Chip8::Op6XNN(const uint8_t reg, const uint8_t value) {
        // Sets reg to value.
        m_reg[reg] = value;
    }

    void Chip8::Op7XNN(const uint8_t reg, const uint8_t value) {
        // Adds value to reg.
        m_reg[reg] += value;
    }

    void Chip8::Op8XYO(const uint8_t left, const uint8_t right, const uint8_t op) {
        uint8_t old;

        // Various bitwise and math operations.
        switch(op) {
            case 0x0:
                // left = right
                m_reg[left] = m_reg[right];
                break;

            case 0x1:
                // left |= right
                m_reg[left] |= m_reg[right];
                break;

            case 0x2:
                // left &= right
                m_reg[left] &= m_reg[right];
                break;

            case 0x3:
                // left ^= right
                m_reg[left] ^= m_reg[right];
                break;

            case 0x4:
                // left += right, VF = 1 on carry, otherwise 0
                old = m_reg[left];
                m_reg[left] += m_reg[right];
                m_reg[0xF] = old > m_reg[left] ? 1 : 0;
                break;

            case 0x5:
                // left -= right, VF = 1 on borrow, otherwise 0
                old = m_reg[left];
                m_reg[left] -= m_reg[right];
                m_reg[0xF] = old < m_reg[left] ? 0 : 1;
                break;

            case 0x6:
                // left >>= 1, VF = LSB of right before shift
                m_reg[0xF] = m_reg[left] & 0x1;
                m_reg[left] >>= 1;
                break;

            case 0x7:
                // left = right - left, VF = 1 on borrow, otherwise 0
                m_reg[left] = m_reg[right] - m_reg[left];
                m_reg[0xF] = m_reg[right] < m_reg[left] ? 0 : 1;
                break;

            case 0xE:
                // left <<= 1, VF = MSB of right before shift
                m_reg[0xF] = (m_reg[left] & 0x80) > 0 ? 1 : 0;
                m_reg[left] <<= 1;
                break;

            default: break;
        }
    }

    void Chip8::Op9XY0(const uint8_t left, const uint8_t right) {
        // Skips the next instruction if left != right.
        if(m_reg[left] != m_reg[right]) m_pc += 2;
    }

    void Chip8::OpANNN(const uint16_t address) {
        // Sets i to address.
        m_i = address;
    }

    void Chip8::OpBNNN(const uint16_t address) {
        // Jumps to address + V0.
        m_pc = address + m_reg[0];
    }

    void Chip8::OpCXNN(const uint8_t reg, const uint8_t value) {
        // Sets reg to random(0-255) & value.
        std::uniform_int_distribution num(0, 255);
        m_reg[reg] = num(m_rng) & value;
    }

    void Chip8::OpDXYN(const uint8_t xReg, const uint8_t yReg, const uint8_t size) {
        // Draws an (8 x size) sprite at address I to (xReg, yReg).
        const auto sprite = new uint64_t[size];
        const auto x = m_reg[xReg];
        const auto y = m_reg[yReg];

        for(auto i = 0; i < size; i++) {
            // Push sprites all the way to the highest bits, then
            // bump them to the right as much as necessary.
            sprite[i] = (static_cast<uint64_t>(m_memory->GetByte(m_i + i)) << 56) >> x;
        }

        m_reg[0xF] = 0;
        for(auto i = 0; i < size; i++) {
            if(y + i >= VideoSize) return; // Don't attempt to write past maxY.

            uint64_t oldRow = m_videoBuffer[y + i];
            m_videoBuffer[y + i] ^= sprite[i];

            // Invert the old row, OR it with the new row, and invert the result.
            // If it's greater than 0, a bit has been turned off.
            oldRow = ~oldRow;
            if(~(oldRow | m_videoBuffer[y + i]) > 0)
                m_reg[0xF] = 1;
        }
    }

    void Chip8::OpEXOO(const uint8_t reg, const uint8_t op) {
        // Input functions.
        switch(op) {
            case 0x9E:
                // Skips the next instruction if the key in reg is pressed.
                if(m_input->GetInput(m_reg[reg]).digital) m_pc += 2;
                break;

            case 0xA1:
                // Skips the next instruction if the key in reg is released.
                if(!m_input->GetInput(m_reg[reg]).digital) m_pc += 2;
                break;

            default: break;
        }
    }

    void Chip8::OpFXOO(const uint8_t reg, const uint8_t op) {
        std::stringstream ss;
        std::string str;

        // Various other instructions.
        switch(op) {
            case 0x07:
                // Store the value of the delay timer in reg.
                m_reg[reg] = m_timerDelay;
                break;

            case 0x0A:
                // Wait for a keypress and store the result in reg.
                m_waitForKey = true;
                m_keyRegister = reg;
                break;

            case 0x15:
                // Set the delay timer to reg.
                m_timerDelay = m_reg[reg];
                break;

            case 0x18:
                // Set the audio timer to reg.
                m_timerAudio = m_reg[reg];
                break;

            case 0x1E:
                // Add reg to I.
                m_i += m_reg[reg];
                break;

            case 0x29:
                // Set I to the address of the charset value in reg.
                m_i = m_charsetAddress + (m_reg[reg] * 5);
                break;

            case 0x33:
                // Stores reg as a binary-coded decimal, starting at I.
                ss << std::setfill('0') << std::setw(3) << std::to_string(m_reg[reg]);
                str = ss.str();
                for(auto i = 0; i < 3; i++)
                    m_memory->SetByte(m_i + i, str[i] - '0');
                break;

            case 0x55:
                // Stores the values of V0 to reg, inclusive, in I and up.
                for(auto i = 0; i < reg + 1; i++)
                    m_memory->SetByte(m_i + i, m_reg[i]);
                break;

            case 0x65:
                // Fills registers V0 to reg, inclusive, with the values in
                // memory starting with I.
                for(auto i = 0; i < reg + 1; i++)
                    m_reg[i] = m_memory->GetByte(m_i + i);
                break;

            default: break;
        }
    }
}