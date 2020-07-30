/* SharpLr35902.Macros.h
 *
 * Macros to assist with Sharp LR35902 emulation.
 */

#pragma once

#define IDX_B    0b000
#define IDX_C    0b001
#define IDX_D    0b010
#define IDX_E    0b011
#define IDX_H    0b100
#define IDX_L    0b101
#define IDX_HL   0b110
#define IDX_A    0b111

#define ADDR_BC  0b00
#define ADDR_DE  0b01
#define ADDR_HLP 0b10  // HL+
#define ADDR_HLM 0b11  // HL-

#define IDX_PAIR_BC  0b00
#define IDX_PAIR_DE  0b01
#define IDX_PAIR_HL  0b10
#define IDX_PAIR_SP  0b11

#define ZERO       7
#define SUBTRACT   6
#define HALFCARRY  5
#define CARRY      4

#define MEM_READ(addr) m_memory->GetByte(addr)
#define MEM_WRITE(addr, val) m_memory->SetByte(addr, val)

#define FETCH m_instr.push_back(MEM_READ(m_reg.pc++))
#define FETCH_ADDR(addr) m_instr.push_back(MEM_READ(addr))

#define BEGIN_EXECUTE m_allowFetch = false
#define END_EXECUTE m_instr.clear(); m_mcycle = 2; m_allowFetch = true
#define NUM_MCYCLES(val) if(++m_mcycle > (val)) { END_EXECUTE; }

#define OP(code) m_instr[0] == (code)
#define OP_MASK(mask, code) (m_instr[0] & (mask)) == (code)
#define OP_CB_MASK(code, mask) (m_instr[1] & (mask)) == (code)

#define OP_REG_X(idx) ((m_instr[(idx)] >> 3) & 0b00000111)
#define OP_REG_Y(idx) (m_instr[(idx)] & 0b00000111)

#define REG_COMBINE(high, low) (((high) << 8) + (low))
#define REG_AF REG_COMBINE(m_reg.a, m_reg.f)
#define REG_BC REG_COMBINE(m_reg.b, m_reg.c)
#define REG_DE REG_COMBINE(m_reg.d, m_reg.e)
#define REG_HL REG_COMBINE(m_reg.h, m_reg.l)

#define FLAG_CLEAR(bit) m_reg.f &= ~(1 << (bit))
#define FLAG_SET(bit) m_reg.f |= (1 << (bit))
