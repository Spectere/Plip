/* SharpLr35902.Ops.cpp
 *
 * An implementation of a MOS 6502 CPU.
 * This file implements the decode and execution steps.
 */

#include "Mos6502.h"
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

#define FETCH_ADDR16(var, addr) { \
    uint8_t low, high; \
    low = m_memory->GetByte(addr); \
    high = m_memory->GetByte(addr + 1); \
    cycleCount += 2; \
}

#define FETCH_ZERO_PAGE(var) { \
    uint8_t offset; \
    FETCH_PC(offset); \
    var = m_memory->GetByte(offset); \
    ++cycleCount; \
}

#define STORE_ZERO_PAGE(val) { \
    uint8_t offset; \
    FETCH_PC(offset); \
    m_memory->SetByte(offset, val); \
    ++cycleCount; \
}

#define FETCH_ZERO_PAGE_X(var) { \
    uint8_t offset; \
    FETCH_PC(offset); \
    offset += m_registers.X; \
    var = m_memory->GetByte(offset); \
    cycleCount += 2; \
}

#define STORE_ZERO_PAGE_X(val) { \
    uint8_t offset; \
    FETCH_PC(offset); \
    offset += m_registers.X; \
    m_memory->SetByte(offset, val); \
    cycleCount += 2; \
}

#define FETCH_ZERO_PAGE_Y(var) { \
    uint8_t offset; \
    FETCH_PC(offset); \
    offset += m_registers.Y; \
    var = m_memory->GetByte(offset); \
    cycleCount += 2; \
}

#define STORE_ZERO_PAGE_Y(val) { \
    uint8_t offset; \
    FETCH_PC(offset); \
    offset += m_registers.Y; \
    m_memory->SetByte(offset, val); \
    cycleCount += 2; \
}

#define FETCH_ABSOLUTE(var) { \
    uint8_t low, high; \
    FETCH_PC(low); \
    FETCH_PC(high); \
    const uint16_t addr = (high << 8) | low; \
    var = m_memory->GetByte(addr); \
    cycleCount++; \
}

#define STORE_ABSOLUTE(val) { \
    uint8_t low, high; \
    FETCH_PC(low); \
    FETCH_PC(high); \
    const uint16_t addr = (high << 8) | low; \
    m_memory->SetByte(addr, val); \
    cycleCount++; \
}

#define FETCH_ABSOLUTE_X(var) { \
    uint8_t low, high; \
    FETCH_PC(low); \
    FETCH_PC(high); \
    const uint16_t addr = ((high << 8) | low) + m_registers.X; \
    var = m_memory->GetByte(addr); \
    cycleCount += ((addr >> 8) != high) ? 2 : 1; \
}

#define STORE_ABSOLUTE_X(val) { \
    uint8_t low, high; \
    FETCH_PC(low); \
    FETCH_PC(high); \
    const uint16_t addr = ((high << 8) | low) + m_registers.X; \
    m_memory->SetByte(addr, val); \
    cycleCount += 2; \
}

#define FETCH_ABSOLUTE_Y(var) { \
    uint8_t low, high; \
    FETCH_PC(low); \
    FETCH_PC(high); \
    const uint16_t addr = ((high << 8) | low) + m_registers.Y; \
    var = m_memory->GetByte(addr); \
    cycleCount += ((addr >> 8) != high) ? 2 : 1; \
}

#define STORE_ABSOLUTE_Y(val) { \
    uint8_t low, high; \
    FETCH_PC(low); \
    FETCH_PC(high); \
    const uint16_t addr = ((high << 8) | low) + m_registers.Y; \
    m_memory->SetByte(addr, val); \
    cycleCount += 2; \
}

#define FETCH_INDIRECT_X(var) { \
    uint8_t index, low, high; \
    FETCH_PC(index); \
    index += m_registers.X; \
    /* This MUST be done like this to ensure that zero page wraparound occurs. */ \
    FETCH_ADDR(low, index); \
    FETCH_ADDR(high, ++index); \
    const uint16_t addr = (high << 8) | low; \
    var = m_memory->GetByte(addr); \
    cycleCount += 2; \
}

#define STORE_INDIRECT_X(val) { \
    uint8_t index, low, high; \
    FETCH_PC(index); \
    index += m_registers.X; \
    /* This MUST be done like this to ensure that zero page wraparound occurs. */ \
    FETCH_ADDR(low, index); \
    FETCH_ADDR(high, ++index); \
    const uint16_t addr = (high << 8) | low; \
    m_memory->SetByte(addr, val); \
    cycleCount += 2; \
}

#define FETCH_INDIRECT_Y(var) { \
    uint8_t index, low, high; \
    FETCH_PC(index); \
    /* This MUST be done like this to ensure that zero page wraparound occurs. */ \
    FETCH_ADDR(low, index); \
    FETCH_ADDR(high, ++index); \
    const uint16_t addr = ((high << 8) | low) + m_registers.Y; \
    var = m_memory->GetByte(addr); \
    cycleCount += ((addr >> 8) != high) ? 2 : 1; \
}

#define STORE_INDIRECT_Y(val) { \
    uint8_t index, low, high; \
    FETCH_PC(index); \
    /* This MUST be done like this to ensure that zero page wraparound occurs. */ \
    FETCH_ADDR(low, index); \
    FETCH_ADDR(high, ++index); \
    const uint16_t addr = ((high << 8) | low) + m_registers.Y; \
    m_memory->SetByte(addr, val); \
    cycleCount += 2; \
}

#define CHECK_NEGATIVE(val) { BIT_TEST(val, 7) ? m_registers.SetNegativeFlag() : m_registers.ClearNegativeFlag(); }

#define CHECK_ZERO(val) { val == 0 ? m_registers.SetZeroFlag() : m_registers.ClearZeroFlag(); }

long Mos6502::DecodeAndExecute() {
    cycleCount = 0;

    FETCH_PC(op);
    switch(op) {
        //
        // Load/Store Operations
        //
        case 0xA9: {
            // LDA imm8
            // 2 cycles, ZN
            FETCH_PC(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0xA5: {
            // LDA zp
            // 3 cycles, ZN
            FETCH_ZERO_PAGE(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0xB5: {
            // LDA zp, X
            // 4 cycles, ZN
            FETCH_ZERO_PAGE_X(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0xAD: {
            // LDA abs16
            // 4 cycles, ZN
            FETCH_ABSOLUTE(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0xBD: {
            // LDA abs16, X
            // 4(+1) cycles, ZN
            FETCH_ABSOLUTE_X(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0xB9: {
            // LDA abs16, Y
            // 4(+1) cycles, ZN
            FETCH_ABSOLUTE_Y(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0xA1: {
            // LDA (imm8, X)
            // 6 cycles, ZN
            FETCH_INDIRECT_X(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0xB1: {
            // LDA (imm8), Y
            // 5(+1) cycles, ZN
            FETCH_INDIRECT_Y(m_registers.A);
            CHECK_NEGATIVE(m_registers.A);
            CHECK_ZERO(m_registers.A);
            break;
        }

        case 0xA2: {
            // LDX imm8
            // 2 cycles, ZN
            FETCH_PC(m_registers.X);
            CHECK_NEGATIVE(m_registers.X);
            CHECK_ZERO(m_registers.X);
            break;
        }

        case 0xA6: {
            // LDX zp
            // 3 cycles, ZN
            FETCH_ZERO_PAGE(m_registers.X);
            CHECK_NEGATIVE(m_registers.X);
            CHECK_ZERO(m_registers.X);
            break;
        }

        case 0xB6: {
            // LDX zp, Y
            // 4 cycles, ZN
            FETCH_ZERO_PAGE_Y(m_registers.X);
            CHECK_NEGATIVE(m_registers.X);
            CHECK_ZERO(m_registers.X);
            break;
        }

        case 0xAE: {
            // LDX abs16
            // 4 cycles, ZN
            FETCH_ABSOLUTE(m_registers.X);
            CHECK_NEGATIVE(m_registers.X);
            CHECK_ZERO(m_registers.X);
            break;
        }

        case 0xBE: {
            // LDX abs16, Y
            // 4(+1) cycles, ZN
            FETCH_ABSOLUTE_Y(m_registers.X);
            CHECK_NEGATIVE(m_registers.X);
            CHECK_ZERO(m_registers.X);
            break;
        }

        case 0xA0: {
            // LDY imm8
            // 2 cycles, ZN
            FETCH_PC(m_registers.Y);
            CHECK_NEGATIVE(m_registers.Y);
            CHECK_ZERO(m_registers.Y);
            break;
        }

        case 0xA4: {
            // LDY zp
            // 3 cycles, ZN
            FETCH_ZERO_PAGE(m_registers.Y);
            CHECK_NEGATIVE(m_registers.Y);
            CHECK_ZERO(m_registers.Y);
            break;
        }

        case 0xB4: {
            // LDY zp, X
            // 4 cycles, ZN
            FETCH_ZERO_PAGE_X(m_registers.Y);
            CHECK_NEGATIVE(m_registers.Y);
            CHECK_ZERO(m_registers.Y);
            break;
        }

        case 0xAC: {
            // LDY abs16
            // 4 cycles, ZN
            FETCH_ABSOLUTE(m_registers.Y);
            CHECK_NEGATIVE(m_registers.Y);
            CHECK_ZERO(m_registers.Y);
            break;
        }

        case 0xBC: {
            // LDY abs16, X
            // 4(+1) cycles, ZN
            FETCH_ABSOLUTE_X(m_registers.Y);
            CHECK_NEGATIVE(m_registers.Y);
            CHECK_ZERO(m_registers.Y);
            break;
        }

        case 0x85: {
            // STA zp
            // 3 cycles
            STORE_ZERO_PAGE(m_registers.A);
            break;
        }

        case 0x95: {
            // STA zp, X
            // 4 cycles
            STORE_ZERO_PAGE_X(m_registers.A);
            break;
        }

        case 0x8D: {
            // STA abs16
            // 4 cycles
            STORE_ABSOLUTE(m_registers.A);
            break;
        }

        case 0x9D: {
            // STA abs16, X
            // 5 cycles
            STORE_ABSOLUTE_X(m_registers.A);
            break;
        }

        case 0x99: {
            // STA abs16, Y
            // 5 cycles
            STORE_ABSOLUTE_Y(m_registers.A);
            break;
        }

        case 0x81: {
            // STA (imm8, X)
            // 6 cycles
            STORE_INDIRECT_X(m_registers.A);
            break;
        }

        case 0x91: {
            // STA (imm8), Y
            // 6 cycles
            STORE_INDIRECT_Y(m_registers.A);
            break;
        }

        case 0x86: {
            // STX zp
            // 3 cycles
            STORE_ZERO_PAGE(m_registers.X);
            break;
        }

        case 0x96: {
            // STX zp, Y
            // 4 cycles
            STORE_ZERO_PAGE_Y(m_registers.X);
            break;
        }

        case 0x8E: {
            // STX abs16
            // 4 cycles
            STORE_ABSOLUTE(m_registers.X);
            break;
        }

        case 0x84: {
            // STY zp
            // 3 cycles
            STORE_ZERO_PAGE(m_registers.Y);
            break;
        }

        case 0x94: {
            // STY zp, X
            // 4 cycles
            STORE_ZERO_PAGE_X(m_registers.Y);
            break;
        }

        case 0x8C: {
            // STY abs16
            // 4 cycles
            STORE_ABSOLUTE(m_registers.Y);
            break;
        }

        default: {
            throw PlipInvalidOpcodeException(op);
        }
    }

    return cycleCount;
}
