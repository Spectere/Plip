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

#define ADDR_MODE(opcode) ((opcode) & 0b00011100)

#define CHECK_ADD_CARRY(left, right) { \
    if(((uint16_t)(left)) + ((uint16_t)(right)) + ((uint16_t)(m_registers.GetCarryFlag() ? 1 : 0)) > 0xFF) m_registers.SetCarryFlag(); \
    else m_registers.ClearCarryFlag(); \
}

#define CHECK_NEGATIVE(val) { BIT_TEST(val, 7) ? m_registers.SetNegativeFlag() : m_registers.ClearNegativeFlag(); }

#define CHECK_OVERFLOW(left, right) { \
    const uint8_t checkOverflowAddition = (left) + (right) + (m_registers.GetCarryFlag() ? 1 : 0); \
    const uint8_t checkOverflowResult = ((left) ^ checkOverflowAddition) & ((right) ^ checkOverflowAddition) & 0x80; \
    if(checkOverflowResult != 0) m_registers.SetOverflowFlag(); else m_registers.ClearOverflowFlag(); \
}

#define CHECK_ZERO(val) { (val) == 0 ? m_registers.SetZeroFlag() : m_registers.ClearZeroFlag(); }

#define STACK_POP() (m_memory->GetByte(StackLocation | ++m_registers.S))

#define STACK_PUSH(val) { m_memory->SetByte(StackLocation | m_registers.S--, (val)); }

uint8_t Mos6502::AddBinary(const uint8_t value) {
    const uint8_t result = (m_registers.A + value + (m_registers.GetCarryFlag() ? 1 : 0)) & 0xFF;

    CHECK_OVERFLOW(m_registers.A, value);
    CHECK_ADD_CARRY(m_registers.A, value);
    CHECK_ZERO(result);
    CHECK_NEGATIVE(result);

    return result;
}

uint8_t Mos6502::AddDecimal(const uint8_t value) {
    // Decimal addition. First, add the low nibble and adjust if necessary.
    uint16_t result = (m_registers.A & 0x0F) + (value & 0x0F) + (m_registers.GetCarryFlag() ? 1 : 0);
    if(result > 0x09) result += 0x06;

    // Add in the high nibble, figure out the adjustment value, check for overflow, then perform the adjustment.
    result += (m_registers.A & 0xF0) + (value & 0xF0);
    const uint8_t highAdjustment = ((result & 0xFFF0) > 0x90) ? 0x60 : 0;

    // Check for overflow, then perform the high nibble adjustment.
    if((m_registers.A ^ result) & (value ^ result) & 0x80) {
        m_registers.SetOverflowFlag();
    } else {
        m_registers.ClearOverflowFlag();
    }
        
    result += highAdjustment;

    // Set carry flag based on the high nibble adjustment.
    if(highAdjustment) m_registers.SetCarryFlag(); else m_registers.ClearCarryFlag();

    if(m_version == Mos6502Version::Wdc65C02) {
        // The WDC 65C02 sets the zero flag correctly, based on the result of BCD addition. This causes this
        // instruction to use an additional cycle.
        CHECK_ZERO(result & 0xFF);
        ++cycleCount;
    } else {
        // Buggy NMOS 6502 behavior. Set the zero flag based on the result of binary addition.
        CHECK_ZERO(m_registers.A + value + (m_registers.GetCarryFlag() ? 1 : 0));
    }

    CHECK_NEGATIVE(result);

    return result & 0xFF;
}

uint8_t Mos6502::SubDecimal(const uint8_t value) {
    const int carry = m_registers.GetCarryFlag() ? 0 : 1;
    const int result = m_registers.A - value - carry;
    
    // Set the carry and overflow flags based on the binary result.
    if(result & 0xFF00) {
        m_registers.ClearCarryFlag();
    } else {
        m_registers.SetCarryFlag();
    }

    if((m_registers.A ^ value) & (m_registers.A ^ result) & 0x80) {
        m_registers.SetOverflowFlag();
    } else {
        m_registers.ClearOverflowFlag();
    }
    
    // Perform arithmetic on each nibble and perform adjustments as necessary.
    int lowNibble = (m_registers.A & 0x0F) - (value & 0x0F) - carry;
    int highNibble = (m_registers.A & 0xF0) - (value & 0xF0);

    // If we carry into bit 4, adjust the low nibble. Borrow from the high nibble if necessary. 
    if(lowNibble & 0xF0) lowNibble -= 0x06;
    if(lowNibble & 0x80) highNibble -= 0x10;
    if(highNibble & 0x0F00) highNibble -= 0x60;

    // Put the final result together, check for negative and zero.
    const uint8_t final = (lowNibble & 0x0F) | (highNibble & 0xF0);
    CHECK_NEGATIVE(final);
    CHECK_ZERO(final);

    // If we're emulating a WDC 65C02, add a cycle.
    // TODO: Apparently the 65C02 will also perform additional adjustments in some circumstances. Find out more and implement them.
    if(m_version == Mos6502Version::Wdc65C02) {
        ++cycleCount;
    }

    // Whew!
    return final;
}

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
            STACK_PUSH(m_registers.A);
            cycleCount += 2;
            break;
        }

        case 0x08: {
            // PHP
            STACK_PUSH(m_registers.F);
            cycleCount += 2;
            break;
        }

        case 0x68: {
            // PLA
            m_registers.A = STACK_POP();
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            cycleCount += 3;
            break;
        }

        case 0x28: {
            // PLP
            m_registers.F = STACK_POP() | 0b00100000;
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

        //
        // Arithmetic
        //
        case 0x69: case 0x65: case 0x75: case 0x6D: case 0x7D: case 0x79: case 0x61: case 0x71: {
            // ADC
            const uint8_t value = FetchFromMemory(ADDR_MODE(op));
            if(AluIsInDecimalMode()) {
                m_registers.A = AddDecimal(value);
            } else {
                m_registers.A = AddBinary(value);
            }
            break;
        }

        case 0xE9: case 0xE5: case 0xF5: case 0xED: case 0xFD: case 0xF9: case 0xE1: case 0xF1: {
            // SBC
            uint8_t value = FetchFromMemory(ADDR_MODE(op));
            if(AluIsInDecimalMode()) {
                m_registers.A = SubDecimal(value);
            } else {
                value ^= 0xFF;
                m_registers.A = AddBinary(value);
            }
            break;
        }

        case 0xC9: case 0xC5: case 0xD5: case 0xCD: case 0xDD: case 0xD9: case 0xC1: case 0xD1: {
            // CMP
            const uint8_t value = FetchFromMemory(ADDR_MODE(op));
            const uint8_t result = m_registers.A - value;

            if(m_registers.A >= value) {
                m_registers.SetCarryFlag();
            } else {
                m_registers.ClearCarryFlag();
            }
            
            CHECK_NEGATIVE(result);
            CHECK_ZERO(result);
            break;
        }

        case 0xE0: case 0xE4: case 0xEC: {
            // CPX
            uint8_t value;
            if(op == 0xE0) {  // CPX/CPY uses 0b000 as the immediate addressing mode
                FETCH_PC(value);
            } else {
                value = FetchFromMemory(ADDR_MODE(op));
            }
            const uint8_t result = m_registers.X - value;

            if(m_registers.X >= value) {
                m_registers.SetCarryFlag();
            } else {
                m_registers.ClearCarryFlag();
            }
            
            CHECK_NEGATIVE(result);
            CHECK_ZERO(result);
            break;
        }

        case 0xC0: case 0xC4: case 0xCC: {
            // CPY
            uint8_t value;
            if(op == 0xC0) {  // CPX/CPY uses 0b000 as the immediate addressing mode
                FETCH_PC(value);
            } else {
                value = FetchFromMemory(ADDR_MODE(op));
            }
            const uint8_t result = m_registers.Y - value;

            if(m_registers.Y >= value) {
                m_registers.SetCarryFlag();
            } else {
                m_registers.ClearCarryFlag();
            }
            
            CHECK_NEGATIVE(result);
            CHECK_ZERO(result);            
            break;
        }

        //
        // Increments and Decrements
        //
        case 0xE6: case 0xF6: case 0xEE: case 0xFE: {
            // INC
            const uint16_t addr = FetchAddress(ADDR_MODE(op));
            uint8_t value;
            FETCH_ADDR(value, addr);
            ++value;
            CHECK_ZERO(value);
            CHECK_NEGATIVE(value);
            m_memory->SetByte(addr, value);
            break;
        }

        case 0xE8: {
            // INX
            ++m_registers.X;
            CHECK_ZERO(m_registers.X);
            CHECK_NEGATIVE(m_registers.X);
            ++cycleCount;
            break;
        }

        case 0xC8: {
            // INY
            ++m_registers.Y;
            CHECK_ZERO(m_registers.Y);
            CHECK_NEGATIVE(m_registers.Y);
            ++cycleCount;
            break;
        }

        case 0xC6: case 0xD6: case 0xCE: case 0xDE: {
            // DEC
            const uint16_t addr = FetchAddress(ADDR_MODE(op));
            uint8_t value;
            FETCH_ADDR(value, addr);
            --value;
            CHECK_ZERO(value);
            CHECK_NEGATIVE(value);
            m_memory->SetByte(addr, value);
            break;
        }

        case 0xCA: {
            // DEX
            --m_registers.X;
            CHECK_ZERO(m_registers.X);
            CHECK_NEGATIVE(m_registers.X);
            ++cycleCount;
            break;
        }

        case 0x88: {
            // DEY
            --m_registers.Y;
            CHECK_ZERO(m_registers.Y);
            CHECK_NEGATIVE(m_registers.Y);
            ++cycleCount;
            break;
        }

        //
        // Shifts and Rotates
        //
        case 0x0A: case 0x06: case 0x16: case 0x0E: case 0x1E: {
            // ASL
            uint8_t value;
            uint16_t addr = 0;
            if(op == 0x0A) {
                value = m_registers.A;
            } else {
                addr = FetchAddress(ADDR_MODE(op));
                FETCH_ADDR(value, addr);
            }

            if(value & 0x80) m_registers.SetCarryFlag(); else m_registers.ClearCarryFlag();
            value <<= 1;
            CHECK_ZERO(value);
            CHECK_NEGATIVE(value);

            if(op == 0x0A) {
                m_registers.A = value;
                ++cycleCount;
            } else {
                m_memory->SetByte(addr, value);
            }
            break;
        }

        case 0x4A: case 0x46: case 0x56: case 0x4E: case 0x5E: {
            // LSR
            uint8_t value;
            uint16_t addr = 0;
            if(op == 0x4A) {
                value = m_registers.A;
            } else {
                addr = FetchAddress(ADDR_MODE(op));
                FETCH_ADDR(value, addr);
            }

            if(value & 0x01) m_registers.SetCarryFlag(); else m_registers.ClearCarryFlag();
            value >>= 1;
            CHECK_ZERO(value);
            CHECK_NEGATIVE(value);  // Should never be set.

            if(op == 0x4A) {
                m_registers.A = value;
                ++cycleCount;
            } else {
                m_memory->SetByte(addr, value);
            }
            break;
        }

        case 0x2A: case 0x26: case 0x36: case 0x2E: case 0x3E: {
            // ROL
            uint8_t value;
            uint16_t addr = 0;
            if(op == 0x2A) {
                value = m_registers.A;
            } else {
                addr = FetchAddress(ADDR_MODE(op));
                FETCH_ADDR(value, addr);
            }

            const uint8_t carry = m_registers.GetCarryFlag() ? 0x01 : 0x00;
            if(value & 0x80) m_registers.SetCarryFlag(); else m_registers.ClearCarryFlag();
            value <<= 1;
            value |= carry;
            CHECK_ZERO(value);
            CHECK_NEGATIVE(value);

            if(op == 0x2A) {
                m_registers.A = value;
                ++cycleCount;
            } else {
                m_memory->SetByte(addr, value);
            }
            break;
        }

        case 0x6A: case 0x66: case 0x76: case 0x6E: case 0x7E: {
            // ROR
            uint8_t value;
            uint16_t addr = 0;
            if(op == 0x6A) {
                value = m_registers.A;
            } else {
                addr = FetchAddress(ADDR_MODE(op));
                FETCH_ADDR(value, addr);
            }

            const uint8_t carry = m_registers.GetCarryFlag() ? 0x80 : 0x00;
            if(value & 0x01) m_registers.SetCarryFlag(); else m_registers.ClearCarryFlag();
            value >>= 1;
            value |= carry;
            CHECK_ZERO(value);
            CHECK_NEGATIVE(value);

            if(op == 0x6A) {
                m_registers.A = value;
                ++cycleCount;
            } else {
                m_memory->SetByte(addr, value);
            }
            break;
        }

        //
        // Jumps and Calls
        //
        case 0x4C: {
            // JMP (absolute)
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            m_registers.PC = (high << 8) | low;
            break;
        }

        case 0x6C: {
            // JMP (indirect)
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = (high << 8) | low;
            FETCH_ADDR(low, addr);
            FETCH_ADDR(high, addr + 1);
            m_registers.PC = (high << 8) | low;
            break;
        }

        case 0x20: {
            // JSR
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t pc = m_registers.PC - 1;
            STACK_PUSH(pc >> 8);
            STACK_PUSH(pc & 0xFF);
            m_registers.PC = (high << 8) | low;
            break;
        }

        case 0x60: {
            // RTS
            const uint8_t low = STACK_POP();
            const uint8_t high = STACK_POP();
            m_registers.PC = (high << 8) | low;
            cycleCount += 5;
            break;
        }

        default: {
            throw PlipInvalidOpcodeException(op);
        }
    }

    return cycleCount;
}

uint16_t Mos6502::FetchAddress(int addressingMode) {
    switch(addressingMode) {
        case ModeZeroPage: {
            uint8_t offset, low, high;
            FETCH_PC(offset);
            FETCH_ADDR(low, offset);
            FETCH_ADDR(high, ++offset);  // Abuse integer overflows to simulate zero page wraparound.
            return (high << 8) | low;
        }

        case ModeZeroPageReg: {
            uint8_t offset, low, high;
            FETCH_PC(offset);
            offset += m_registers.X;
            FETCH_ADDR(low, offset);
            FETCH_ADDR(high, ++offset);  // Abuse integer overflows to simulate zero page wraparound.
            ++cycleCount;
            return (high << 8) | low;
        }

        case ModeAbsolute: {
            uint16_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            cycleCount += 2;
            return (high << 8) | low;
        }

        case ModeAbsoluteX: {
            uint16_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            uint16_t addr = (high << 8) | low;
            addr += m_registers.X;
            cycleCount += 3;
            return addr;
        }
        
        default:
            throw PlipEmulationException("6502: Invalid addressing mode in this context");
    }
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
