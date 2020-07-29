/* SharpLr35902.Macros.h
 *
 * Macros to assist with Sharp LR35902 emulation.
 */

#pragma once

#define REG_B   0b000
#define REG_C   0b001
#define REG_D   0b010
#define REG_E   0b011
#define REG_H   0b100
#define REG_L   0b101
#define REG_HL  0b110
#define REG_A   0b111

#define ZERO       7
#define SUBTRACT   6
#define HALFCARRY  5
#define CARRY      4

#define MEM_READ(addr) m_memory->GetByte(addr)
#define MEM_WRITE(addr, val) m_memory->SetByte(addr, val)

#define FETCH m_instr.push_back(MEM_READ(m_reg.pc++))

#define FLAG_CLEAR(bit) m_reg.f &= ~(1 << bit)
#define FLAG_SET(bit) m_reg.f |= (1 << bit)

// Retrieves a slice of a byte.
// val = value (uint8_t)
// pos = starting bit (0 == LSB, 7 == MSB)
// size = size in bits, including the selected bit
//    ex. SLICE(0b11010011, 5, 2) == 0b00000010
//        SLICE(0b11010011, 0, 5) == 0b00010011
//        SLICE(0b11010011, 4, 4) == 0b00001101
#define SLICE(val, pos, size) ((val << (8 - pos - size)) >> (pos + (8 - pos - size)))

// Retrieves (size) of the most significant bytes from val.
#define MSB(val, size) (val >> (8 - size));

// Retrieves (size) of the least significant bytes from val.
#define LSB(val, size) ((val << (8 - size)) >> (8 - size))
