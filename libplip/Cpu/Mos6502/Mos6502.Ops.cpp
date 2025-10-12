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

#define FETCH_PC_SIGNED(var) { \
    var = static_cast<int8_t>(m_memory->GetByte(m_registers.PC++)); \
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

#define STACK_PUSH_FLAGS() { STACK_PUSH(m_registers.F | (1 << m_registers.BreakCommandBit)); }

#define STACK_PUSH_16(val) { STACK_PUSH(val >> 8); STACK_PUSH(val & 0xFF); }

#define BRANCH(cond) { \
    int8_t rel; \
    FETCH_PC_SIGNED(rel); \
    if(cond) { \
        ++cycleCount; \
        JumpRelative(rel); \
    } \
}

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
    highAdjustment ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();

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

void Mos6502::CallAbsolute(const uint16_t addr) {
    // Save address to stack and jump.
    const uint16_t pc = m_registers.PC;
    STACK_PUSH_16(pc);
    cycleCount += 3;
    m_registers.PC = addr;
}

void Mos6502::CallReturn() {
    // Pops address from stack and jumps.
    const uint8_t low = STACK_POP();
    const uint8_t high = STACK_POP();
    m_registers.PC = (high << 8) | low;
}

void Mos6502::JumpRelative(const int8_t rel) {
    // Jumps to a relative address. Add a cycle if we cross into a new page.
    const uint16_t newPc = m_registers.PC + rel;
    if((m_registers.PC ^ newPc) & 0xFF00) {
        ++cycleCount;
    }
    m_registers.PC = newPc;
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

void Mos6502::OpAddWithCarry(const uint8_t value) {
    if(AluIsInDecimalMode()) {
        m_registers.A = AddDecimal(value);
    } else {
        m_registers.A = AddBinary(value);
    }
}

void Mos6502::OpBitwiseAnd(const uint8_t value) {
    m_registers.A &= value;
    CHECK_NEGATIVE(m_registers.A);
    CHECK_ZERO(m_registers.A);
}

void Mos6502::OpBitwiseOr(const uint8_t value) {
    m_registers.A |= value;
    CHECK_NEGATIVE(m_registers.A);
    CHECK_ZERO(m_registers.A);
}

void Mos6502::OpBitwiseXor(const uint8_t value) {
    m_registers.A ^= value;
    CHECK_NEGATIVE(m_registers.A);
    CHECK_ZERO(m_registers.A);
}

void Mos6502::OpCompare(const uint8_t value) {
    const uint8_t result = m_registers.A - value;

    (m_registers.A >= value) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
            
    CHECK_NEGATIVE(result);
    CHECK_ZERO(result);
}

uint8_t Mos6502::OpDecrement(const uint16_t addr) {
    uint8_t value;
    FETCH_ADDR(value, addr);
    --value;
    CHECK_ZERO(value);
    CHECK_NEGATIVE(value);
    m_memory->SetByte(addr, value);
    ++cycleCount;
    return value;
}

uint8_t Mos6502::OpIncrement(const uint16_t addr) {
    uint8_t value;
    FETCH_ADDR(value, addr);
    ++value;
    CHECK_ZERO(value);
    CHECK_NEGATIVE(value);
    m_memory->SetByte(addr, value);
    ++cycleCount;
    return value;
}

uint8_t Mos6502::OpLogicalShiftLeft(uint8_t value) {
    (value & 0x80) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
    value <<= 1;
    CHECK_ZERO(value);
    CHECK_NEGATIVE(value);
    ++cycleCount;
    return value;
}

uint8_t Mos6502::OpLogicalShiftRight(uint8_t value) {
    if(value & 0x01) m_registers.SetCarryFlag(); else m_registers.ClearCarryFlag();
    value >>= 1;
    CHECK_ZERO(value);
    CHECK_NEGATIVE(value);  // Should never be set.
    ++cycleCount;
    return value;
}

uint8_t Mos6502::OpRotateLeft(uint8_t value) {
    const uint8_t carry = m_registers.GetCarryFlag() ? 0x01 : 0x00;
    (value & 0x80) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
    value <<= 1;
    value |= carry;
    CHECK_ZERO(value);
    CHECK_NEGATIVE(value);
    ++cycleCount;
    return value;
}

uint8_t Mos6502::OpRotateRight(uint8_t value) {
    const uint8_t carry = m_registers.GetCarryFlag() ? 0x80 : 0x00;
    (value & 0x01) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
    value >>= 1;
    value |= carry;
    CHECK_ZERO(value);
    CHECK_NEGATIVE(value);
    ++cycleCount;
    return value;
}

void Mos6502::OpSubtractWithBorrow(uint8_t value) {
    if(AluIsInDecimalMode()) {
        m_registers.A = SubDecimal(value);
    } else {
        m_registers.A = AddBinary(value ^ 0xFF);
    }
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
            STACK_PUSH_FLAGS();
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
            OpBitwiseAnd(FetchFromMemory(ADDR_MODE(op)));
            break;
        }

        case 0x49: case 0x45: case 0x55: case 0x4D: case 0x5D: case 0x59: case 0x41: case 0x51: {
            // EOR
            OpBitwiseXor(FetchFromMemory(ADDR_MODE(op)));
            break;
        }

        case 0x09: case 0x05: case 0x15: case 0x0D: case 0x1D: case 0x19: case 0x01: case 0x11: {
            // ORA
            OpBitwiseOr(FetchFromMemory(ADDR_MODE(op)));
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
            OpAddWithCarry(FetchFromMemory(ADDR_MODE(op)));
            break;
        }

        case 0xE9: case 0xE5: case 0xF5: case 0xED: case 0xFD: case 0xF9: case 0xE1: case 0xF1: {
            // SBC
            OpSubtractWithBorrow(FetchFromMemory(ADDR_MODE(op)));
            break;
        }

        case 0xC9: case 0xC5: case 0xD5: case 0xCD: case 0xDD: case 0xD9: case 0xC1: case 0xD1: {
            // CMP
            OpCompare(FetchFromMemory(ADDR_MODE(op)));
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

            (m_registers.X >= value) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
            
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

            (m_registers.Y >= value) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
            
            CHECK_NEGATIVE(result);
            CHECK_ZERO(result);            
            break;
        }

        //
        // Increments and Decrements
        //
        case 0xE6: case 0xF6: case 0xEE: case 0xFE: {
            // INC
            OpIncrement(FetchAddress(ADDR_MODE(op), false, true));
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
            OpDecrement(FetchAddress(ADDR_MODE(op), false, true));
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
                addr = FetchAddress(ADDR_MODE(op), false, true);
                FETCH_ADDR(value, addr);
            }

            value = OpLogicalShiftLeft(value);
            
            if(op == 0x0A) {
                m_registers.A = value;
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
                addr = FetchAddress(ADDR_MODE(op), false, true);
                FETCH_ADDR(value, addr);
            }

            value = OpLogicalShiftRight(value);

            if(op == 0x4A) {
                m_registers.A = value;
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
                addr = FetchAddress(ADDR_MODE(op), false, true);
                FETCH_ADDR(value, addr);
            }

            value = OpRotateLeft(value);

            if(op == 0x2A) {
                m_registers.A = value;
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
                addr = FetchAddress(ADDR_MODE(op), false, true);
                FETCH_ADDR(value, addr);
            }

            value = OpRotateRight(value);

            if(op == 0x6A) {
                m_registers.A = value;
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
            uint8_t lowDest, highDest, lowSrc, highSrc;
            FETCH_PC(lowSrc);
            FETCH_PC(highSrc);
            uint16_t addr = (highSrc << 8) | lowSrc;
            
            FETCH_ADDR(lowDest, addr);
            addr = (addr & 0xFF00) | static_cast<uint8_t>(lowSrc + 1);  // Emulate page wraparound.
            FETCH_ADDR(highDest, addr);
            m_registers.PC = (highDest << 8) | lowDest;
            break;
        }

        case 0x20: {
            // JSR
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            CallAbsolute((high << 8) | low);
            break;
        }

        case 0x60: {
            // RTS
            CallReturn();
            cycleCount += 5;
            break;
        }

        //
        // Branches
        //
        case 0x90: {
            // BCC
            BRANCH(!m_registers.GetCarryFlag());
            break;
        }

        case 0xB0: {
            // BCS
            BRANCH(m_registers.GetCarryFlag());
            break;
        }

        case 0xF0: {
            // BEQ
            BRANCH(m_registers.GetZeroFlag());
            break;
        }

        case 0x30: {
            // BMI
            BRANCH(m_registers.GetNegativeFlag());
            break;
        }

        case 0xD0: {
            // BNE
            BRANCH(!m_registers.GetZeroFlag());
            break;
        }

        case 0x10: {
            // BPL
            BRANCH(!m_registers.GetNegativeFlag());
            break;
        }

        case 0x50: {
            // BVC
            BRANCH(!m_registers.GetOverflowFlag());
            break;
        }

        case 0x70: {
            // BVS
            BRANCH(m_registers.GetOverflowFlag());
            break;
        }

        //
        // Status Flag
        //
        case 0x18: {
            // CLC
            m_registers.ClearCarryFlag();
            ++cycleCount;
            break;
        }

        case 0xD8: {
            // CLD
            m_registers.ClearDecimalMode();
            ++cycleCount;
            break;
        }

        case 0x58: {
            // CLI
            m_registers.ClearInterruptDisable();
            ++cycleCount;
            break;
        }

        case 0xB8: {
            // CLV
            m_registers.ClearOverflowFlag();
            ++cycleCount;
            break;
        }

        case 0x38: {
            // SEC
            m_registers.SetCarryFlag();
            ++cycleCount;
            break;
        }

        case 0xF8: {
            // SED
            m_registers.SetDecimalMode();
            ++cycleCount;
            break;
        }

        case 0x78: {
            // SEI
            m_registers.SetInterruptDisable();
            ++cycleCount;
            break;
        }

        //
        // System Instructions
        //
        case 0x00: {
            // BRK
            const uint16_t addr = (m_memory->GetByte(0xFFFF) << 8) | m_memory->GetByte(0xFFFE);
            CallAbsolute(addr);
            STACK_PUSH_FLAGS();
            m_registers.SetBreakCommand();
            cycleCount += 3;
            break;
        }

        case 0xEA: {
            // NOP
            ++cycleCount;
            break;
        }

        case 0x40: {
            // RTI
            m_registers.F = STACK_POP() | 0b00100000;
            CallReturn();
            cycleCount += 5;
            break;
        }

        default: {
            if(m_version == Mos6502Version::Wdc65C02) {
                DecodeAndExecuteWdc65C02Extended();
            } else {
                DecodeAndExecuteNmosUnofficial();
            }
        }
    }

    return cycleCount;
}

void Mos6502::DecodeAndExecuteNmosUnofficial() {
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch(op) { // NOLINT(*-multiway-paths-covered)
        case 0x02: case 0x12: case 0x22: case 0x32: case 0x42: case 0x52: case 0x62: case 0x72: case 0x92: case 0xB2: case 0xD2: case 0xF2: {
            // KIL
            // Crashes the CPU.
            OpKillExecuted = true;
            break;
        }
        
        case 0x1A: case 0x3A: case 0x5A: case 0x7A: case 0xDA: case 0xFA: {
            // NOP (1 byte, 2 cycles)
            ++cycleCount;
            break;
        }

        case 0x80: case 0x82: case 0x89: case 0xC2: case 0xE2: {
            // SKB (2 bytes, 2 cycles)
            // Effectively 'NOP imm8'
            ++m_registers.PC;
            ++cycleCount;
            break;
        }

        case 0x0C:                                                         /* IGN abs16    */
        case 0x1C: case 0x3C: case 0x5C: case 0x7C: case 0xDC: case 0xFC:  /* IGN abs16, X */
        case 0x04: case 0x44: case 0x64:                                   /* IGN zp       */
        case 0x14: case 0x34: case 0x54: case 0x74: case 0xD4: case 0xF4:  /* IGN zp, X    */ {
            // IGN (2-3 bytes, 3-5 cycles)
            // Effectively NOPs with a variety of addressing modes.
            FetchFromMemory(ADDR_MODE(op));  // This should take care of the cycle count.
            break;
        }

        case 0xEB: {
            // SBC imm8 (alternate opcode)
            OpSubtractWithBorrow(FetchFromMemory(ADDR_MODE(op)));
            break;
        }

        case 0x4B: {
            // ALR/ASR imm8 (2 cycles)
            // Equivalent to "AND imm8; LSR A"
            uint8_t value;
            FETCH_PC(value);
            m_registers.A &= value;
            m_registers.A = OpLogicalShiftRight(m_registers.A);
            --cycleCount;
            break;
        }

        case 0x0B: case 0x2B: {
            // ANC imm8 (2 cycles)
            // "AND imm8", then copies N to C.
            uint8_t value;
            FETCH_PC(value);
            m_registers.A &= value;
            CHECK_ZERO(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            m_registers.GetNegativeFlag() ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
            break;
        }

        case 0x6B: {
            // ARR imm8 (2 cycles)
            // Similar to "AND imm8; ROR A", with different flag setting logic.
            // N and Z are set normally.
            // C is bit 6
            // V is bit 6 xor bit 5.
            uint8_t value;
            FETCH_PC(value);
            m_registers.A &= value;
            m_registers.A >>= 1;
            m_registers.A |= (m_registers.GetCarryFlag() ? 0x80 : 0);
            CHECK_ZERO(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            (m_registers.A & 0b01000000) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
            (((m_registers.A & 0b01000000) >> 1) ^ (m_registers.A & 0b00100000)) ? m_registers.SetOverflowFlag() : m_registers.ClearOverflowFlag();
            break;
        }

        case 0xCB: {
            // AXS imm8 (2 cycles)
            // Sets X to "(A && X) - imm8" and updates NZC.
            uint8_t value;
            FETCH_PC(value);
            const uint8_t andResult = m_registers.A & m_registers.X;
            int subResult = andResult - value;
            (subResult < 0) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
            CHECK_ZERO(subResult);
            CHECK_NEGATIVE(subResult);
            m_registers.X = subResult & 0xFF;
            break;
        }

        case 0xA3: case 0xA7: case 0xAF: case 0xB3: case 0xB7: case 0xBF: {
            // LAX
            // "LDA imm8; TAX"
            const uint8_t value = FetchFromMemory(ADDR_MODE(op), true);
            m_registers.X = m_registers.A = value;
            CHECK_ZERO(value);
            CHECK_NEGATIVE(value);
            break;
        }

        case 0x83: case 0x87: case 0x8F: case 0x97: {
            // SAX
            // M = A & X, no flags are affected
            StoreToMemory(ADDR_MODE(op), m_registers.A & m_registers.X, true);
            break;
        }

        case 0xC3: case 0xC7: case 0xCF: case 0xD3: case 0xD7: case 0xDB: case 0xDF: {
            // DCP
            // DEC value; CMP value
            const uint16_t addr = FetchAddress(ADDR_MODE(op), false, true);
            const uint8_t value = OpDecrement(addr);
            OpCompare(value);
            break;
        }

        case 0xE3: case 0xE7: case 0xEF: case 0xF3: case 0xF7: case 0xFB: case 0xFF: {
            // ISC
            // INC value; SBC value
            const uint16_t addr = FetchAddress(ADDR_MODE(op), false, true);
            const uint8_t value = OpIncrement(addr);
            OpSubtractWithBorrow(value);
            break;
        }

        case 0x23: case 0x27: case 0x2F: case 0x33: case 0x37: case 0x3B: case 0x3F: {
            // RLA
            // ROL value; AND value
            const uint16_t addr = FetchAddress(ADDR_MODE(op), false, true);
            uint8_t value;
            FETCH_ADDR(value, addr);
            value = OpRotateLeft(value);
            m_memory->SetByte(addr, value);

            OpBitwiseAnd(value);
            break;
        }

        case 0x63: case 0x67: case 0x6F: case 0x73: case 0x77: case 0x7B: case 0x7F: {
            // RRA
            // ROR value; ADC value
            const uint16_t addr = FetchAddress(ADDR_MODE(op), false, true);
            uint8_t value;
            FETCH_ADDR(value, addr);
            value = OpRotateRight(value);
            m_memory->SetByte(addr, value);

            OpAddWithCarry(value);
            break;
        }

        case 0x03: case 0x07: case 0x0F: case 0x13: case 0x17: case 0x1B: case 0x1F: {
            // SLO
            // ASL value; ORA value
            const uint16_t addr = FetchAddress(ADDR_MODE(op), false, true);
            uint8_t value;
            FETCH_ADDR(value, addr);
            value = OpLogicalShiftLeft(value);
            m_memory->SetByte(addr, value);

            OpBitwiseOr(value);
            break;
        }

        case 0x43: case 0x47: case 0x4F: case 0x53: case 0x57: case 0x5B: case 0x5F: {
            // SRE
            // LSR value; EOR value
            const uint16_t addr = FetchAddress(ADDR_MODE(op), false, true);
            uint8_t value;
            FETCH_ADDR(value, addr);
            value = OpLogicalShiftRight(value);
            m_memory->SetByte(addr, value);

            OpBitwiseXor(value);
            break;
        }

        case 0xBB: {
            // LAS abs16, Y
            // S &= mem; A = X = S
            // No page boundary penalty; fixed 4 cycle instruction.
            const uint8_t value = FetchFromMemory(ADDR_MODE(op), false, false, true);
            --cycleCount;  // Force penalty, then adjust cycle count.
            m_registers.A = m_registers.X = m_registers.S &= value;
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0x9B: {
            // TAS abs16, Y
            // S = A & X; (*mem + y) = S & ((address >> 8) + 1;  /* WTF?! */
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = (high << 8) | low;
            m_registers.S = m_registers.A & m_registers.X;
            m_memory->SetByte(addr + m_registers.Y, m_registers.S & ((addr >> 8) + 1));
            cycleCount += 2;
            break;
        }

        case 0x9C: {
            // SHY abs16, Y
            // (*mem + X) = Y & ((addr >> 8) + 1)
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = (high << 8) | low;
            m_memory->SetByte(addr + m_registers.X, m_registers.Y & ((addr >> 8) + 1));
            cycleCount += 2;
            break;
        }

        case 0x9E: {
            // SHX abs16, Y
            // (*mem + Y) = X & ((addr >> 8) + 1)
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = (high << 8) | low;
            m_memory->SetByte(addr + m_registers.Y, m_registers.X & ((addr >> 8) + 1));
            cycleCount += 2;
            break;
        }

        case 0x93: {
            // AHX (zp), Y
            // *addr = zp; (addr + Y) = A & X & (&addr) 
            uint8_t zp, low, high;
            FETCH_PC(zp);
            FETCH_ADDR(low, zp)
            FETCH_ADDR(high, (zp + 1) & 0xFF);
            const uint16_t addr = (high << 8) | low;
            const uint8_t value = m_memory->GetByte(addr);
            m_memory->SetByte(addr + m_registers.Y, m_registers.A & m_registers.X & value);
            cycleCount += 2;
            break;
        }

        case 0x9F: {
            // AHX abs16, Y
            // (*mem + Y) = A & X & ((addr >> 8) + 1
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = (high << 8) | low;
            m_memory->SetByte(addr + m_registers.Y, m_registers.A & m_registers.X & ((addr >> 8) + 1));
            cycleCount += 2;
            break;
        }

        case 0xAB: {
            // ANX imm8
            // X = A &= imm8
            // Set N and Z based on A
            uint8_t imm;
            FETCH_PC(imm);
            m_registers.X = m_registers.A &= imm;
            CHECK_ZERO(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            break;
        }

        case 0x8B: {
            // XAA imm8
            // E = (A & X & imm8)
            // A = (A | magic) X & imm8
            // Set N and Z based on E. E is discarded. Results are unstable on real hardware.
            uint8_t imm;
            FETCH_PC(imm);
            const uint8_t result = m_registers.A & m_registers.X & imm;
            CHECK_ZERO(result);
            CHECK_NEGATIVE(result);
            m_registers.A = (m_registers.A | XaaMagic) & m_registers.X & imm;
            break;
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void Mos6502::DecodeAndExecuteWdc65C02Extended() { // NOLINT(*-convert-member-functions-to-static)
    // TODO: The 65C02 has additional instructions that we don't currently handle.
    throw PlipInvalidOpcodeException(op);
}

uint16_t Mos6502::FetchAddress(const int addressingMode, const bool alwaysUseY, const bool forcePenalty) {
    switch(addressingMode) {
        case ModeIndexedIndirect: {
            uint8_t index, low, high;
            FETCH_PC(index);
            index += m_registers.X;
            FETCH_ADDR(low, index);
            FETCH_ADDR(high, ++index);  // Abuse integer overflows to simulate zero page wraparound.
            cycleCount += 2;
            return (high << 8) | low;
        }
            
        case ModeZeroPage: {
            uint8_t offset;
            FETCH_PC(offset);
            ++cycleCount;
            return offset;
        }
            
        case ModeAbsolute: {
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            ++cycleCount;
            return (high << 8) | low;
        }
            
        case ModeIndirectIndexed: {
            uint8_t index, low, high;
            FETCH_PC(index);
            FETCH_ADDR(low, index);
            FETCH_ADDR(high, ++index);  // Abuse integer overflows to simulate zero page wraparound.
            const uint16_t addr = ((high << 8) | low) + m_registers.Y;
            cycleCount += (forcePenalty || ((addr >> 8) != high)) ? 2 : 1;
            return addr;
        }
            
        case ModeZeroPageReg: {
            uint8_t offset;
            FETCH_PC(offset);
            offset += alwaysUseY ? m_registers.Y : m_registers.X;
            cycleCount += 2;
            return offset;
        }
            
        case ModeAbsoluteY: {
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = ((high << 8) | low) + m_registers.Y;
            cycleCount += (forcePenalty || ((addr >> 8) != high)) ? 2 : 1;
            return addr;
        }
            
        case ModeAbsoluteX: {
            uint8_t low, high;
            FETCH_PC(low);
            FETCH_PC(high);
            const uint16_t addr = ((high << 8) | low) + m_registers.X;
            cycleCount += (forcePenalty || ((addr >> 8) != high)) ? 2 : 1;
            return addr;
        }
        
        default:
            throw PlipEmulationException("6502: Invalid addressing mode in this context");
    }
}

uint8_t Mos6502::FetchFromMemory(int addressingMode, const bool alwaysUseY, const bool useAccumulator, const bool forcePenalty) {
    if(alwaysUseY && (addressingMode == ModeAbsoluteX)) {
        addressingMode = ModeAbsoluteY;
    }

    if(addressingMode == ModeImmediate) {
        if(useAccumulator) {
            return m_registers.A;
        }

        uint8_t value;
        FETCH_PC(value);
        return value;
    }
    
    return m_memory->GetByte(FetchAddress(addressingMode, alwaysUseY, forcePenalty));
}

long Mos6502::ServiceInterrupt(const uint16_t vector) {
    STACK_PUSH_16(m_registers.PC);
    STACK_PUSH_FLAGS();
    m_registers.PC = vector;
    m_registers.ClearBreakCommand();
    m_registers.SetInterruptDisable();
    return InterruptCycles;
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
