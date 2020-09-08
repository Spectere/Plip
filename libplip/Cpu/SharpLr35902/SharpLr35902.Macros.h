/* SharpLr35902.Macros.h
 *
 * Macros to assist with Sharp LR35902 emulation.
 */

#pragma once

#include "../../PlipSupport.h"

#define IDX_B        0b000
#define IDX_C        0b001
#define IDX_D        0b010
#define IDX_E        0b011
#define IDX_H        0b100
#define IDX_L        0b101
#define IDX_HL       0b110
#define IDX_A        0b111

#define ADDR_BC      0b00
#define ADDR_DE      0b01
#define ADDR_HLP     0b10  // HL+
#define ADDR_HLM     0b11  // HL-

#define IDX_16_BC    0b00
#define IDX_16_DE    0b01
#define IDX_16_HL    0b10
#define IDX_16_SP    0b11

#define COND_NZ      0b00
#define COND_Z       0b01
#define COND_NC      0b10
#define COND_C       0b11

#define ZERO         7
#define SUBTRACT     6
#define HALFCARRY    5
#define CARRY        4

#define MEM_READ(addr) m_memory->GetByte(addr)
#define MEM_WRITE(addr, val) m_memory->SetByte(addr, val)

#define FETCH m_instr.push_back(MEM_READ(m_reg.pc++))
#define FETCH_CYCLE(cycle) do { if(m_mcycle == (cycle)) { FETCH; } } while(0)
#define FETCH_ADDR(addr) m_instr.push_back(MEM_READ(addr))
#define FETCH_ADDR_CYCLE(cycle, addr) do { if(m_mcycle == (cycle)) { FETCH_ADDR(addr); } } while(0)
#define FETCH_IMM_CYCLE(cycle) do { if(m_mcycle == (cycle)) { FETCH; } } while(0)

#define CYCLE(cycle) if(m_mcycle == (cycle))

#define BEGIN_EXECUTE m_allowFetch = false
#define END_EXECUTE m_instr.clear(); m_mcycle = 2; m_allowFetch = true
#define NUM_MCYCLES(val) do { if(++m_mcycle > (val)) { END_EXECUTE; } } while(0)

#define OP_COND ((m_instr[0] >> 3) & 0b00000011)
#define OP_REG16(idx) ((m_instr[(idx)] >> 4) & 0b00000011)
#define OP_IDX(idx) ((m_instr[(idx)] >> 3) & 0b00000111)
#define OP_REG_X(idx) OP_IDX(idx)
#define OP_REG_Y(idx) (m_instr[(idx)] & 0b00000111)

#define REG_HL COMBINE16LE(m_reg.h, m_reg.l)

#define SET_PC_IMM m_reg.pc = m_instr[2] << 8 | m_instr[1]
#define SET_PC_STACK(cycle) \
    do { \
        CYCLE(cycle)     { m_reg.pc &= 0xFF00; m_reg.pc |= STACK_POP;      } \
        CYCLE(cycle + 1) { m_reg.pc &= 0x00FF; m_reg.pc |= STACK_POP << 8; } \
    } while(0)

#define STACK_PUSH(val) m_memory->SetByte(--m_reg.sp, (val))
#define STACK_PUSH_PC(cycle) \
    do { \
        CYCLE(cycle)     { STACK_PUSH(m_reg.pc >> 8);   } \
        CYCLE(cycle + 1) { STACK_PUSH(m_reg.pc & 0xFF); } \
    } while(0)
#define STACK_POP m_memory->GetByte(m_reg.sp++)

#define FLAG_CLEAR(bit) BIT_CLEAR(m_reg.f, (bit))
#define FLAG_FLIP(bit) BIT_FLIP(m_reg.f, (bit))
#define FLAG_SET(bit) BIT_SET(m_reg.f, (bit))
#define FLAG_TEST(bit) BIT_TEST(m_reg.f, (bit))

#define CHECK_BIT_CARRY(val) do { if(val) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY); } while(0)
#define CHECK_CARRY(val) do { if((val) & 0xFF00) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY); } while(0)
#define CHECK_CARRY16(val) do { if((val) & 0xFFFF0000) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY); } while(0)
#define CHECK_ADD_HALFCARRY(left, right) do { \
    if(((left) & 0x0F) + ((right) & 0x0F) > 0x0F) FLAG_SET(HALFCARRY); \
    else FLAG_CLEAR(HALFCARRY); } while(0)
#define CHECK_SUB_HALFCARRY(left, right) do { \
    if(((left) & 0x0F) - ((right) & 0x0F) < 0x00) FLAG_SET(HALFCARRY); \
    else FLAG_CLEAR(HALFCARRY); } while(0)
#define CHECK_ADD_HALFCARRY16(left, right) do { \
    if(((left) & 0x0FFF) + ((right) & 0x0FFF) > 0x0FFF) FLAG_SET(HALFCARRY); \
    else FLAG_CLEAR(HALFCARRY); } while(0)
#define CHECK_ZERO(val) do { if((val)) FLAG_CLEAR(ZERO); else FLAG_SET(ZERO); } while(0)
