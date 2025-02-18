/* SharpLr35902.Ops.cpp
 *
 * An implementation of a Sharp LR35902 CPU (SM83 core).
 * This file implements the decode and execution steps.
 */

#include "SharpLr35902.h"
#include "../../PlipEmulationException.h"

using Plip::Cpu::SharpLr35902;

static int cycleCount = 0;
static uint8_t op;
static uint8_t op2;

#define FETCH_PC(var) { \
    var = m_memory->GetByte(m_registers.PC++); \
    ++cycleCount; \
}

#define FETCH_ADDR(var, addr) { \
    var = m_memory->GetByte(addr); \
    ++cycleCount; \
}

#define STORE_ADDR(addr, val) { \
    m_memory->SetByte(addr, val); \
    ++cycleCount; \
}

#define OP_REG_X ((op >> 3) & 0b111)
#define OP_REG_Y (op & 0b111)
#define OP_REG16 ((op >> 4) & 0b11)
#define OP_PTR OP_REG16
#define OP_COND ((op >> 3) & 0b11)

static constexpr int AddrBc  = 0b00;
static constexpr int AddrDe  = 0b01;
static constexpr int AddrHlI = 0b10;
static constexpr int AddrHlD = 0b11;

void DecodeAndExecuteCb();

uint16_t SharpLr35902::GetPointerAddress(int pointerIndex) {
    switch(pointerIndex) {
        case AddrBc: return m_registers.GetBc();
        case AddrDe: return m_registers.GetDe();
        case AddrHlI: {
            const auto addr = m_registers.GetHl();
            m_registers.SetHl(addr + 1);
            return addr;
        }
        case AddrHlD: {
            const auto addr = m_registers.GetHl();
            m_registers.SetHl(addr - 1);
            return addr;
        }
        default:
            throw new PlipEmulationException("BUG: Attempted to resolve a pointer using an out of range index.");
    }
}

long SharpLr35902::DecodeAndExecute() {
    cycleCount = 0;

    FETCH_PC(op);
    switch(op) {
        //
        // 8-bit Load Instructions
        //
        case 0x02: case 0x12: case 0x22: case 0x32: {
            // LD [xx], A
            // 2 cycles, - - - -
            const auto addr = GetPointerAddress(OP_PTR);
            STORE_ADDR(addr, m_registers.A);
            break;
        }

        case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E: case 0x3E: {
            // LD x, imm8
            // 2 cycles, - - - -
            const auto destRegIdx = OP_REG_X;
            uint8_t imm8;
            FETCH_PC(imm8);
            m_registers.Set8ByIndex(destRegIdx, imm8);
            break;
        }

        case 0x0A: case 0x1A: case 0x2A: case 0x3A: {
            // LD A, [xx]
            // 2 cycles, - - - -
            const auto addr = GetPointerAddress(OP_PTR);
            uint8_t val;
            FETCH_ADDR(val, addr);
            m_registers.A = val;
            break;
        }

        case 0x36: {
            // LD [HL], imm8
            // 3 cycles, - - - -
            uint8_t imm8;
            FETCH_PC(imm8);
            STORE_ADDR(m_registers.GetHl(), imm8);
            break;
        }

        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47:
        case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4F:
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57:
        case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5F:
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67:
        case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6F:
        case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7F: {
            // LD x, y
            // 1 cycle, - - - -
            const auto destRegIdx = OP_REG_X;
            const auto srcRegIdx = OP_REG_Y;
            m_registers.Set8ByIndex(destRegIdx, m_registers.Get8ByIndex(srcRegIdx));
            break;
        }

        case 0x46: case 0x4E: case 0x56: case 0x5E: case 0x66: case 0x6E: case 0x7E: {
            // LD x, [HL]
            // 2 cycles, - - - -
            const auto destRegIdx = OP_REG_X;
            uint8_t val;
            FETCH_ADDR(val, m_registers.GetHl());
            m_registers.Set8ByIndex(destRegIdx, val);
            break;
        }

        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77: {
            // LD [HL], y
            // 2 cycles, - - - -
            STORE_ADDR(m_registers.GetHl(), m_registers.Get8ByIndex(OP_REG_Y));
            break;
        }

        case 0xE0: {
            // LD [imm8], A
            // 3 cycles, - - - -
            uint8_t offset;
            FETCH_PC(offset);
            STORE_ADDR(0xFF00 | offset, m_registers.A);
            break;
        }

        case 0xE2: {
            // LDH [C], A
            // 2 cycles, - - - -
            STORE_ADDR(0xFF00 | m_registers.C, m_registers.A);
            break;
        }

        case 0xEA: {
            // LD [imm16], A
            // 4 cycles, - - - -
            uint8_t memLow;
            uint8_t memHigh;
            FETCH_PC(memLow);
            FETCH_PC(memHigh);
            STORE_ADDR((memHigh << 8) | memLow, m_registers.A);
            break;
        }

        case 0xF0: {
            // LDH A, [imm8]
            // 3 cycles, - - - -
            uint8_t offset;
            FETCH_PC(offset);
            FETCH_ADDR(m_registers.A, 0xFF00 | offset);
            break;
        }

        case 0xF2: {
            // LDH A, [C]
            // 2 cycles, - - - -
            FETCH_ADDR(m_registers.A, 0xFF00 | m_registers.C);
            break;
        }

        case 0xFA: {
            // LD A, [imm16]
            // 4 cycles, - - - -
            uint8_t memLow;
            uint8_t memHigh;
            FETCH_PC(memLow);
            FETCH_PC(memHigh);
            FETCH_ADDR(m_registers.A, (memHigh << 8) | memLow);
            break;
        }

        default:
            throw PlipEmulationException("Instruction not implemented");
    }

    return cycleCount;
}

void DecodeAndExecuteCb() {
}

