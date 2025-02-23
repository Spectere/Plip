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

#define CHECK_ADD_CARRY(left, right) { \
    if(((uint16_t)(left)) + ((uint16_t)(right)) > 0xFF) m_registers.SetCarryFlag(); \
    else m_registers.ClearCarryFlag(); \
}

#define CHECK_ADD_HALF_CARRY(left, right) { \
    if(((left) & 0x0F) + ((right) & 0x0F) > 0x0F) m_registers.SetHalfCarryFlag(); \
    else m_registers.ClearHalfCarryFlag(); \
}

#define CHECK_SUB_BORROW(left, right) { \
    ((right) > (left)) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag(); \
}

#define CHECK_SUB_HALF_BORROW(left, right) { \
    if(((right) & 0x0F) > ((left) & 0x0F)) m_registers.SetHalfCarryFlag(); \
    else m_registers.ClearHalfCarryFlag(); \
}

#define CHECK_ZERO(val) { \
    if((val) == 0) m_registers.SetZeroFlag(); \
    else m_registers.ClearZeroFlag(); \
}

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

uint16_t SharpLr35902::GetPointerAddress(const int pointerIndex) {
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

void SharpLr35902::OpAddToRegisterA(int value, const bool addWithCarry) {
    value += ((addWithCarry && m_registers.GetCarryFlag()) ? 1 : 0);

    CHECK_ADD_HALF_CARRY(m_registers.A, value);
    CHECK_ADD_CARRY(m_registers.A, value);
    m_registers.A += value;
    m_registers.ClearSubtractFlag();
    CHECK_ZERO(m_registers.A);
}

void SharpLr35902::OpBitwiseAndRegisterA(const uint8_t value) {
    m_registers.A &= value;
    m_registers.ClearCarryFlag();
    m_registers.SetHalfCarryFlag();
    m_registers.ClearSubtractFlag();
    CHECK_ZERO(m_registers.A);
}

void SharpLr35902::OpBitwiseOrRegisterA(const uint8_t value) {
    m_registers.A |= value;
    m_registers.ClearCarryFlag();
    m_registers.ClearHalfCarryFlag();
    m_registers.ClearSubtractFlag();
    CHECK_ZERO(m_registers.A);
}

void SharpLr35902::OpBitwiseXorRegisterA(const uint8_t value) {
    m_registers.A ^= value;
    m_registers.ClearCarryFlag();
    m_registers.ClearHalfCarryFlag();
    m_registers.ClearSubtractFlag();
    CHECK_ZERO(m_registers.A);
}

void SharpLr35902::OpSubtractFromRegisterA(int value, const bool subtractWithBorrow, const bool discardResult) {
    value += ((subtractWithBorrow && m_registers.GetCarryFlag()) ? 1 : 0);
    const uint8_t result = m_registers.A - value;

    CHECK_SUB_HALF_BORROW(m_registers.A, value);
    CHECK_SUB_BORROW(m_registers.A, value);
    m_registers.SetSubtractFlag();
    CHECK_ZERO(result);

    if(!discardResult) m_registers.A = result;
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

        //
        // 16-bit Load Instructions
        //
        case 0x01: case 0x11: case 0x21: case 0x31: {
            // LD xx, imm16
            // 3 cycles, - - - -
            const auto destRegIdx = OP_REG16;
            uint8_t valLow;
            uint8_t valHigh;
            FETCH_PC(valLow);
            FETCH_PC(valHigh);
            m_registers.Set16ByIndex(destRegIdx, (valHigh << 8) | valLow);
            break;
        }

        case 0x08: {
            // LD [imm16], SP
            // 5 cycles, - - - - -
            uint8_t addrLow;
            uint8_t addrHigh;
            FETCH_PC(addrLow);
            FETCH_PC(addrHigh);

            const uint16_t addr = (addrHigh << 8) | addrLow;
            STORE_ADDR(addr, m_registers.SP);
            STORE_ADDR(addr + 1, m_registers.SP >> 8);
            break;
        }

        case 0xC1: case 0xD1: case 0xE1: case 0xF1: {
            // POP zz
            // 3 cycles, BC/DE/HL: - - - -, AF: Z N H C
            const auto destReg16Idx = OP_REG16;
            uint8_t valLow;
            uint8_t valHigh;
            FETCH_ADDR(valLow, m_registers.SP++);
            FETCH_ADDR(valHigh, m_registers.SP++);
            if(destReg16Idx == m_registers.RegIndex16Af) {
                // AF shares an index with SP, but it must be handled differently.
                m_registers.A = valHigh;
                m_registers.F = valLow & 0xF0;  // lower 4 bits are discarded
            } else {
                m_registers.Set16ByIndex(destReg16Idx, (valHigh << 8) | valLow);
            }
            break;
        }

        case 0xC5: case 0xD5: case 0xE5: case 0xF5: {
            // PUSH zz
            // 3 cycles, - - - -
            const auto srcReg16Idx = OP_REG16;
            uint8_t valLow;
            uint8_t valHigh;
            if(srcReg16Idx == m_registers.RegIndex16Af) {
                // AF shares an index with SP, but it must be handled differently.
                valLow = m_registers.F & 0xF0;
                valHigh = m_registers.A;
            } else {
                const auto val = m_registers.Get16ByIndex(srcReg16Idx);
                valLow = val;
                valHigh = val >> 8;
            }
            STORE_ADDR(--m_registers.SP, valHigh);
            STORE_ADDR(--m_registers.SP, valLow);
            break;
        }

        case 0xF8: {
            // LD HL, SP + imm8s
            // 3 cycles, 0 0 H C
            uint8_t val;
            FETCH_PC(val);
            m_registers.SetHl(m_registers.SP);

            m_registers.ClearSubtractFlag();
            m_registers.ClearZeroFlag();
            CHECK_ADD_HALF_CARRY(m_registers.L, val);
            CHECK_ADD_CARRY(m_registers.L, val);

            m_registers.L += val;

            // Adjust H if necessary.
            if(m_registers.GetCarryFlag() && (val & 0b1000000) == 0) {
                m_registers.H++;
            } else if(!m_registers.GetCarryFlag() && (val & 0b1000000) != 0) {
                m_registers.H--;
            }
            cycleCount++;
            break;
        }

        case 0xF9: {
            // LD SP, HL
            // 2 cycles, - - - -
            m_registers.SP = m_registers.GetHl();
            cycleCount++;
            break;
        }

        //
        // 8-bit Arithmetic / Logical Instructions
        //
        case 0x04: case 0x0C: case 0x14: case 0x1C: case 0x24: case 0x2C: case 0x3C: {
            // INC x
            // 1 cycle, Z 0 H -
            const auto destRegIdx = OP_REG_X;
            uint8_t regValue = m_registers.Get8ByIndex(destRegIdx);

            CHECK_ADD_HALF_CARRY(regValue, 1);
            m_registers.ClearSubtractFlag();

            m_registers.Set8ByIndex(destRegIdx, ++regValue);
            CHECK_ZERO(regValue);

            break;
        }

        case 0x05: case 0x0D: case 0x15: case 0x1D: case 0x25: case 0x2D: case 0x3D: {
            // DEC x
            // 1 cycle, Z 1 H -
            const auto destRegIdx = OP_REG_X;
            uint8_t regValue = m_registers.Get8ByIndex(destRegIdx);

            CHECK_SUB_HALF_BORROW(regValue, 1);
            m_registers.SetSubtractFlag();

            m_registers.Set8ByIndex(destRegIdx, --regValue);
            CHECK_ZERO(regValue);

            break;
        }

        case 0x34: {
            // INC [HL]
            // 3 cycles, Z 0 H -
            uint8_t memValue;
            FETCH_ADDR(memValue, m_registers.GetHl());

            CHECK_ADD_HALF_CARRY(memValue, 1);
            m_registers.ClearSubtractFlag();

            STORE_ADDR(m_registers.GetHl(), ++memValue);
            CHECK_ZERO(memValue);

            break;
        }

        case 0x35: {
            // DEC [HL]
            // 3 cycles, Z 0 H -
            uint8_t memValue;
            FETCH_ADDR(memValue, m_registers.GetHl());

            CHECK_SUB_HALF_BORROW(memValue, 1);
            m_registers.SetSubtractFlag();

            STORE_ADDR(m_registers.GetHl(), --memValue);
            CHECK_ZERO(memValue);

            break;
        }

        case 0x27: {
            // DAA
            // 1 cycle, Z - 0 C
            uint8_t adjustment = 0;

            if(m_registers.GetSubtractFlag()) {
                adjustment += m_registers.GetHalfCarryFlag() ? 0x06 : 0;
                adjustment += m_registers.GetCarryFlag() ? 0x60 : 0;
                m_registers.A -= adjustment;
                m_registers.ClearCarryFlag();
            } else {
                adjustment += (m_registers.GetHalfCarryFlag() || ((m_registers.A & 0x0F) > 0x09)) ? 0x06 : 0;
                if(m_registers.GetCarryFlag() || m_registers.A > 0x99) {
                    adjustment |= 0x60;
                    m_registers.SetCarryFlag();
                } else {
                    m_registers.ClearCarryFlag();
                }
                m_registers.A += adjustment;
            }

            CHECK_ZERO(m_registers.A);
            m_registers.ClearHalfCarryFlag();

            break;
        }

        case 0x2F: {
            // CPL
            // 1 cycle, - 1 1 -
            m_registers.SetSubtractFlag();
            m_registers.SetHalfCarryFlag();

            m_registers.A ^= 0xFF;
            break;
        }

        case 0x37: {
            // SCF
            // 1 cycle, - 0 0 1
            m_registers.ClearSubtractFlag();
            m_registers.ClearHalfCarryFlag();
            m_registers.SetCarryFlag();

            break;
        }

        case 0x3F: {
            // CCF
            // 1 cycle, - 0 0 C

            m_registers.ClearSubtractFlag();
            m_registers.ClearHalfCarryFlag();
            m_registers.GetCarryFlag() ? m_registers.ClearCarryFlag() : m_registers.SetCarryFlag();

            break;
        }

        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x87:
        case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8F: {
            // ADD A, y (0x80-0x87)
            // ADC A, y (0x88-0x8F)
            // 1 cycle, Z 0 H C
            const auto regValue = m_registers.Get8ByIndex(OP_REG_Y);
            OpAddToRegisterA(regValue, op >= 0x88);

            break;
        }

        case 0x86: case 0x8E: {
            // ADD A, [HL] (0x86)
            // ADC A, [HL] (0x8E)
            // 2 cycles, Z 0 H C
            int memValue;
            FETCH_ADDR(memValue, m_registers.GetHl());
            OpAddToRegisterA(memValue, op == 0x8E);

            break;
        }

        case 0xC6: case 0xCE: {
            // ADD A, imm8 (0xC6)
            // ADC A, imm8 (0xCE)
            // 2 cycles, Z 0 H C
            int immValue;
            FETCH_PC(immValue);
            OpAddToRegisterA(immValue, op == 0xCE);

            break;
        }

        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x97:
        case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9F: {
            // SUB A, y (0x90-0x97)
            // SBC A, y (0x98-0x9F)
            // 1 cycle, Z 1 H C
            const auto regValue = m_registers.Get8ByIndex(OP_REG_Y);
            OpSubtractFromRegisterA(regValue, op >= 0x98, false);

            break;
        }

        case 0x96: case 0x9E: {
            // SUB A, [HL] (0x96)
            // SBC A, [HL] (0x9E)
            // 2 cycles, Z 1 H C
            int memValue;
            FETCH_ADDR(memValue, m_registers.GetHl());
            OpSubtractFromRegisterA(memValue, op == 0x9E, false);

            break;
        }

        case 0xD6: case 0xDE: {
            // SUB A, imm8 (0xD6)
            // SBC A, imm8 (0xDE)
            // 2 cycles, Z 1 H C
            int immValue;
            FETCH_PC(immValue);
            OpSubtractFromRegisterA(immValue, op == 0xDE, false);

            break;
        }

        case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA7: {
            // AND A, y
            // 1 cycle, Z 0 1 0
            const auto regValue = m_registers.Get8ByIndex(OP_REG_Y);
            OpBitwiseAndRegisterA(regValue);

            break;
        }

        case 0xA6: {
            // AND A, [HL]
            // 2 cycles, Z 0 1 0
            uint8_t memValue;
            FETCH_ADDR(memValue, m_registers.GetHl());
            OpBitwiseAndRegisterA(memValue);

            break;
        }

        case 0xE6: {
            // AND A, imm8
            // 2 cycles, Z 0 1 0
            uint8_t immValue;
            FETCH_PC(immValue);
            OpBitwiseAndRegisterA(immValue);

            break;
        }

        case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAF: {
            // XOR A, y
            // 1 cycle, Z 0 0 0
            const auto regValue = m_registers.Get8ByIndex(OP_REG_Y);
            OpBitwiseXorRegisterA(regValue);

            break;
        }

        case 0xAE: {
            // XOR A, [HL]
            // 2 cycles, Z 0 0 0
            uint8_t memValue;
            FETCH_ADDR(memValue, m_registers.GetHl());
            OpBitwiseXorRegisterA(memValue);

            break;
        }

        case 0xEE: {
            // XOR A, imm8
            // 2 cycles, Z 0 0 0
            uint8_t immValue;
            FETCH_PC(immValue);
            OpBitwiseXorRegisterA(immValue);

            break;
        }

        case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB7: {
            // OR A, y
            // 1 cycle, Z 0 0 0
            const auto regValue = m_registers.Get8ByIndex(OP_REG_Y);
            OpBitwiseOrRegisterA(regValue);

            break;
        }

        case 0xB6: {
            // OR A, [HL]
            // 2 cycles, Z 0 0 0
            uint8_t memValue;
            FETCH_ADDR(memValue, m_registers.GetHl());
            OpBitwiseOrRegisterA(memValue);

            break;
        }

        case 0xF6: {
            // OR A, imm8
            // 2 cycles, Z 0 0 0
            uint8_t immValue;
            FETCH_PC(immValue);
            OpBitwiseOrRegisterA(immValue);

            break;
        }

        case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBF: {
            // CP A, y
            // 1 cycle, Z 1 H C
            const auto regValue = m_registers.Get8ByIndex(OP_REG_Y);
            OpSubtractFromRegisterA(regValue, false, true);

            break;
        }

        case 0xBE: {
            // CP A, [HL]
            // 2 cycles, Z 1 H C
            uint8_t memValue;
            FETCH_ADDR(memValue, m_registers.GetHl());
            OpSubtractFromRegisterA(memValue, false, true);

            break;
        }

        case 0xFE: {
            // CP A, imm8
            // 2 cycles, Z 1 H C
            uint8_t immValue;
            FETCH_PC(immValue);
            OpSubtractFromRegisterA(immValue, false, true);

            break;
        }

        default:
            throw PlipEmulationException("Instruction not implemented");
    }

    return cycleCount;
}
