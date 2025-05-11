/* SharpLr35902.Ops.cpp
 *
 * An implementation of a MOS 6502 CPU.
 * This file implements the decode and execution steps.
 */

#include "Mos6502.h"
#include "../../PlipEmulationException.h"
#include "../../PlipInvalidOpcodeException.h"

using Plip::Cpu::Mos6502;

static int cycleCount;
static uint8_t op;

#define FETCH_PC(var) { \
    var = m_memory->GetByte(m_registers.PC++); \
    ++cycleCount; \
}

#define FETCH_ADDR(var, addr) { \
    var = m_memory->GetByte(addr); \
    ++cycleCount; \
}

#define ADDR_MODE(opcode) (opcode & 0b00011100)

#define CHECK_NEGATIVE(val) { BIT_TEST(val, 7) ? m_registers.SetNegativeFlag() : m_registers.ClearNegativeFlag(); }

#define CHECK_ZERO(val) { val == 0 ? m_registers.SetZeroFlag() : m_registers.ClearZeroFlag(); }

long Mos6502::DecodeAndExecute() {
    cycleCount = 0;

    FETCH_PC(op);
    switch(op) {
        //
        // Load/Store Operations
        //
        case 0xA9: case 0xA5: case 0xB5: case 0xAD: case 0xBD: case 0xB9: case 0xA1: case 0xB1: {
            // LDA
            m_registers.A = FetchFromMemory(ADDR_MODE(op));
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0xA2: case 0xA6: case 0xB6: case 0xAE: case 0xBE: {
            // LDX
            if(op == 0xA2) {
                // LDX/LDY use 0b000 for the immediate addressing mode index for some reason.
                FETCH_PC(m_registers.X);
            } else {
                m_registers.X = FetchFromMemory(ADDR_MODE(op), true);
            }
            CHECK_NEGATIVE(m_registers.X);
            CHECK_ZERO(m_registers.X);
            break;
        }

        case 0xA0: case 0xA4: case 0xB4: case 0xAC: case 0xBC: {
            // LDY
            if(op == 0xA0) {
                // LDX/LDY use 0b000 for the immediate addressing mode index for some reason.
                FETCH_PC(m_registers.Y);
            } else {
                m_registers.Y = FetchFromMemory(ADDR_MODE(op));
            }
            CHECK_NEGATIVE(m_registers.Y);
            CHECK_ZERO(m_registers.Y);
            break;
        }

        case 0x85: case 0x95: case 0x8D: case 0x9D: case 0x99: case 0x81: case 0x91: {
            // STA
            StoreToMemory(ADDR_MODE(op), m_registers.A);
            break;
        }

        case 0x86: case 0x96: case 0x8E: {
            // STX
            StoreToMemory(ADDR_MODE(op), m_registers.X, true);
            break;
        }

        case 0x84: case 0x94: case 0x8C: {
            // STY
            StoreToMemory(ADDR_MODE(op), m_registers.Y);
            break;
        }

        //
        // Register Transfers
        //
        case 0xAA: {
            // TAX
            m_registers.X = m_registers.A;
            CHECK_NEGATIVE(m_registers.X);
            CHECK_ZERO(m_registers.X);
            cycleCount++;
            break;
        }

        case 0xA8: {
            // TAY
            m_registers.Y = m_registers.A;
            CHECK_NEGATIVE(m_registers.Y);
            CHECK_ZERO(m_registers.Y);
            cycleCount++;
            break;
        }

        case 0x8A: {
            // TXA
            m_registers.A = m_registers.X;
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            cycleCount++;
            break;
        }

        case 0x98: {
            // TYA
            m_registers.A = m_registers.Y;
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            cycleCount++;
            break;
        }

        //
        // Stack Operations
        //
        case 0xBA: {
            // TSX
            m_registers.X = m_registers.S;
            CHECK_NEGATIVE(m_registers.X);
            CHECK_ZERO(m_registers.X);
            cycleCount++;
            break;
        }

        case 0x9A: {
            // TXS
            m_registers.S = m_registers.X;
            CHECK_NEGATIVE(m_registers.S);
            CHECK_ZERO(m_registers.S);
            cycleCount++;
            break;
        }

        case 0x48: {
            // PHA
            m_memory->SetByte(StackLocation | m_registers.S--, m_registers.A);
            cycleCount += 2;
            break;
        }

        case 0x08: {
            // PHP
            m_memory->SetByte(StackLocation | m_registers.S--, m_registers.F);
            cycleCount += 2;
            break;
        }

        case 0x68: {
            // PLA
            m_registers.A = m_memory->GetByte(StackLocation | ++m_registers.S);
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            cycleCount += 3;
            break;
        }

        case 0x28: {
            // PLP
            m_registers.F = m_memory->GetByte(StackLocation | ++m_registers.S) | 0b00100000;
            cycleCount += 3;
            break;
        }

        //
        // Logical
        //
        case 0x29: case 0x25: case 0x35: case 0x2D: case 0x3D: case 0x39: case 0x21: case 0x31: {
            // AND
            const uint8_t value = FetchFromMemory(ADDR_MODE(op));
            m_registers.A &= value;
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0x49: case 0x45: case 0x55: case 0x4D: case 0x5D: case 0x59: case 0x41: case 0x51: {
            // EOR
            const uint8_t value = FetchFromMemory(ADDR_MODE(op));
            m_registers.A ^= value;
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0x09: case 0x05: case 0x15: case 0x0D: case 0x1D: case 0x19: case 0x01: case 0x11: {
            // ORA
            const uint8_t value = FetchFromMemory(ADDR_MODE(op));
            m_registers.A |= value;
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0x24: case 0x2C: {
            // BIT
            const uint8_t value = FetchFromMemory(ADDR_MODE(op));
            const uint8_t result = value & m_registers.A;
            CHECK_ZERO(result);

            // Adjust the cycle count.
            --cycleCount;

            // Copy the high bits into the flag register (N/V).
            m_registers.F = (m_registers.F & 0b00111111) | (result & 0b11000000);
            break;
        }

        default: {
            throw PlipInvalidOpcodeException(op);
        }
    }

    return cycleCount;
}

uint8_t Mos6502::FetchFromMemory(int addressingMode, const bool alwaysUseY, const bool useAccumulator) {
    if(alwaysUseY && (addressingMode == ModeAbsoluteX)) {
        addressingMode = ModeAbsoluteY;
    }

    switch(addressingMode) {
        case ModeIndexedIndirect: {
            uint8_t index, low, high;
            FETCH_PC(index);
            index += m_registers.X;
            FETCH_ADDR(low, index);
            FETCH_ADDR(high, ++index);  // Abuse integer overflows to simulate zero page wraparound.
            const uint16_t addr = (high << 8) | low;
            cycleCount += 2;
            return m_memory->GetByte(addr);
        }
        case ModeZeroPage: {
            uint8_t offset;
            FETCH_PC(offset);
            ++cycleCount;
            return m_memory->GetByte(offset);
        }
        case ModeImmediate: {
            if(useAccumulator) {
                return m_registers.A;
            }

            uint8_t value;
            FETCH_PC(value);
            return value;
        }
        case ModeAbsolute: {
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = (high << 8) | low;
            ++cycleCount;
            return m_memory->GetByte(addr);
        }
        case ModeIndirectIndexed: {
            uint8_t index, low, high;
            FETCH_PC(index);
            FETCH_ADDR(low, index);
            FETCH_ADDR(high, ++index);  // Abuse integer overflows to simulate zero page wraparound.
            const uint16_t addr = ((high << 8) | low) + m_registers.Y;
            cycleCount += ((addr >> 8) != high) ? 2 : 1;
            return m_memory->GetByte(addr);
        }
        case ModeZeroPageReg: {
            uint8_t offset;
            FETCH_PC(offset);
            offset += alwaysUseY ? m_registers.Y : m_registers.X;
            cycleCount += 2;
            return m_memory->GetByte(offset);
        }
        case ModeAbsoluteY: {
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = ((high << 8) | low) + m_registers.Y;
            cycleCount += ((addr >> 8) != high) ? 2 : 1;
            return m_memory->GetByte(addr);
        }
        case ModeAbsoluteX: {
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = ((high << 8) | low) + m_registers.X;
            cycleCount += ((addr >> 8) != high) ? 2 : 1;
            return m_memory->GetByte(addr);
        }
        default:
            throw PlipEmulationException("6502: Unknown addressing mode");
    }
}

void Mos6502::StoreToMemory(const int addressingMode, const uint8_t value, const bool swapXY) {
    switch(addressingMode) {
        case ModeIndexedIndirect: {
            uint8_t index, low, high;
            FETCH_PC(index);
            index += m_registers.X;
            FETCH_ADDR(low, index);
            FETCH_ADDR(high, ++index);  // Abuse integer overflows to simulate zero page wraparound.
            const uint16_t addr = (high << 8) | low;
            cycleCount += 2;
            m_memory->SetByte(addr, value);
            break;
        }
        case ModeZeroPage: {
            uint8_t offset;
            FETCH_PC(offset);
            ++cycleCount;
            m_memory->SetByte(offset, value);
            break;
        }
        case ModeImmediate: {
            throw PlipEmulationException("6502: Immediate addressing is not valid on stores");
        }
        case ModeAbsolute: {
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = (high << 8) | low;
            ++cycleCount;
            m_memory->SetByte(addr, value);
            break;
        }
        case ModeIndirectIndexed: {
            uint8_t index, low, high;
            FETCH_PC(index);
            FETCH_ADDR(low, index);
            FETCH_ADDR(high, ++index);  // Abuse integer overflows to simulate zero page wraparound.
            const uint16_t addr = ((high << 8) | low) + m_registers.Y;
            cycleCount += 2;
            m_memory->SetByte(addr, value);
            break;
        }
        case ModeZeroPageReg: {
            uint8_t offset;
            FETCH_PC(offset);
            offset += swapXY ? m_registers.Y : m_registers.X;
            cycleCount += 2;
            m_memory->SetByte(offset, value);
            break;
        }
        case ModeAbsoluteY: {
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = ((high << 8) | low) + m_registers.Y;
            cycleCount += 2;
            m_memory->SetByte(addr, value);
            break;
        }
        case ModeAbsoluteX: {
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = ((high << 8) | low) + m_registers.X;
            cycleCount += 2;
            m_memory->SetByte(addr, value);
            break;
        }
        default:
            throw PlipEmulationException("6502: Unknown addressing mode");
    }
}
