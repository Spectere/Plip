/* SharpSm83.Ops.cpp
 *
 * An implementation of a Sharp SM83-based CPU.
 * This file implements the decode and execution steps.
 */

#include "SharpSm83.h"
#include "PlipEmulationException.h"
#include "PlipInvalidOpcodeException.h"
#include "PlipSupport.h"

using Plip::Cpu::SharpSm83;

const std::array<SharpSm83::OpHandler, 256> SharpSm83::OpTable = []{
    std::array<OpHandler, 256> op{};
    op.fill(&SharpSm83::Op_Invalid);

    // Bit Operations
    op[0xCB] = &SharpSm83::Op_CB;

    // Miscellaneous / Control Instructions
    op[0x00] = &SharpSm83::Op_NOP;
    op[0x10] = &SharpSm83::Op_STOP;
    op[0x76] = &SharpSm83::Op_HALT;
    op[0xF3] = &SharpSm83::Op_DI;
    op[0xFB] = &SharpSm83::Op_EI;

    // Jumps / Calls
    op[0x18] = &SharpSm83::Op_JR_imm8s;
    op[0x20] = op[0x28] = op[0x30] = op[0x38] = &SharpSm83::Op_JR_c_imm8s;
    op[0xC9] = op[0xD9] = &SharpSm83::Op_RET;
    op[0xC0] = op[0xC8] = op[0xD0] = op[0xD8] = &SharpSm83::Op_RET_c;
    op[0xC3] = &SharpSm83::Op_JP_imm16;
    op[0xC2] = op[0xCA] = op[0xD2] = op[0xDA] = &SharpSm83::Op_JP_c_imm16;
    op[0xC7] = op[0xCF] = op[0xD7] = op[0xDF] = op[0xE7] = op[0xEF] = op[0xF7] = op[0xFF] = &SharpSm83::Op_RST_vec;
    op[0xE9] = &SharpSm83::Op_JP_HL;
    op[0xCD] = &SharpSm83::Op_CALL_imm16;
    op[0xC4] = op[0xCC] = op[0xD4] = op[0xDC] = &SharpSm83::Op_CALL_c_imm16;

    // 8-bit Load Instructions
    op[0x02] = op[0x12] = op[0x22] = op[0x32] = &SharpSm83::Op_LD_mem_A;
    op[0x06] = op[0x0E] = op[0x16] = op[0x1E] = op[0x26] = op[0x2E] = op[0x3E] = &SharpSm83::Op_LD_reg_imm8;
    op[0x0A] = op[0x1A] = op[0x2A] = op[0x3A] = &SharpSm83::Op_LD_A_mem;
    op[0x36] = &SharpSm83::Op_LD_memHL_imm8;
    for(auto i = 0x40; i < 0x80; ++i) {
        if(i == 0x76) continue;  // HALT

        if((i & 0x7) == 0x6)        op[i] = &SharpSm83::Op_LD_reg_memHL;
        else if((i & 0x78) == 0x70) op[i] = &SharpSm83::Op_LD_memHL_reg;
        else                        op[i] = &SharpSm83::Op_LD_reg_reg;
    }
    op[0xE0] = &SharpSm83::Op_LDH_memImm8_A;
    op[0xE2] = &SharpSm83::Op_LDH_memC_A;
    op[0xEA] = &SharpSm83::Op_LD_memImm16_A;
    op[0xF0] = &SharpSm83::Op_LDH_A_imm8;
    op[0xF2] = &SharpSm83::Op_LDH_A_memC;
    op[0xFA] = &SharpSm83::Op_LD_A_memImm16;

    // 16-bit Load Instructions
    op[0x01] = op[0x11] = op[0x21] = op[0x31] = &SharpSm83::Op_LD_reg16_imm16;
    op[0x08] = &SharpSm83::Op_LD_memImm16_SP;
    op[0xC1] = op[0xD1] = op[0xE1] = op[0xF1] = &SharpSm83::Op_POP_reg16;
    op[0xC5] = op[0xD5] = op[0xE5] = op[0xF5] = &SharpSm83::Op_PUSH_reg16;
    op[0xF8] = &SharpSm83::Op_LD_HL_SPimm8s;
    op[0xF9] = &SharpSm83::Op_LD_SP_HL;

    // 8-bit Arithmetic / Logical Instructions
    op[0x04] = op[0x0C] = op[0x14] = op[0x1C] = op[0x24] = op[0x2C] = op[0x3C] = &SharpSm83::Op_INC_reg;
    op[0x05] = op[0x0D] = op[0x15] = op[0x1D] = op[0x25] = op[0x2D] = op[0x3D] = &SharpSm83::Op_DEC_reg;
    op[0x34] = &SharpSm83::Op_INC_memHL;
    op[0x35] = &SharpSm83::Op_DEC_memHL;
    op[0x27] = &SharpSm83::Op_DAA;
    op[0x2F] = &SharpSm83::Op_CPL;
    op[0x37] = &SharpSm83::Op_SCF;
    op[0x3F] = &SharpSm83::Op_CCF;

    for(auto i = 0x80; i < 0x90; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_ADD_ADC_A_memHL;
        else                 op[i] = &SharpSm83::Op_ADD_ADC_A_reg;
    }
    op[0xC6] = op[0xCE] = &SharpSm83::Op_ADD_ADC_A_imm8;

    for(auto i = 0x90; i < 0xA0; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_SUB_SBC_A_memHL;
        else                 op[i] = &SharpSm83::Op_SUB_SBC_A_reg;
    }
    op[0xD6] = op[0xDE] = &SharpSm83::Op_SUB_SBC_A_imm8;

    for(auto i = 0xA0; i < 0xA8; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_AND_A_memHL;
        else                 op[i] = &SharpSm83::Op_AND_A_reg;
    }
    op[0xE6] = &SharpSm83::Op_AND_A_imm8;

    for(auto i = 0xA8; i < 0xB0; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_XOR_A_memHL;
        else                 op[i] = &SharpSm83::Op_XOR_A_reg;
    }
    op[0xEE] = &SharpSm83::Op_XOR_A_imm8;

    for(auto i = 0xB0; i < 0xB8; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_OR_A_memHL;
        else                 op[i] = &SharpSm83::Op_OR_A_reg;
    }
    op[0xF6] = &SharpSm83::Op_OR_A_imm8;

    for(auto i = 0xB8; i < 0xC0; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_CP_A_memHL;
        else                 op[i] = &SharpSm83::Op_CP_A_reg;
    }
    op[0xFE] = &SharpSm83::Op_CP_A_imm8;

    // 16-bit Arithmetic / Logical Instructions
    op[0x03] = op[0x13] = op[0x23] = op[0x33] = &SharpSm83::Op_INC_reg16;
    op[0x0B] = op[0x1B] = op[0x2B] = op[0x3B] = &SharpSm83::Op_DEC_reg16;
    op[0x09] = op[0x19] = op[0x29] = op[0x39] = &SharpSm83::Op_ADD_HL_reg16;
    op[0xE8] = &SharpSm83::Op_ADD_SP_imm8s;

    // 8-bit Shift, Rotate, and Bit Instructions
    op[0x07] = &SharpSm83::Op_RLCA;
    op[0x0F] = &SharpSm83::Op_RRCA;
    op[0x17] = &SharpSm83::Op_RLA;
    op[0x1F] = &SharpSm83::Op_RRA;

    return op;
}();

const std::array<SharpSm83::OpHandler, 256> SharpSm83::OpTableCb = []{
    std::array<OpHandler, 256> op{};
    op.fill(&SharpSm83::Op_InvalidCB);

    for(auto i = 0x00; i < 0x20; ++i) {
        if((i & 0x7) == 0x6) continue;  // <op> reg, [HL]

        if((i & 0x8) == 0) op[i] = &SharpSm83::Op_RLC_RL_reg;  // 0x00-0x07, 0x10-0x17
        else               op[i] = &SharpSm83::Op_RRC_RR_reg;  // 0x08-0x0F, 0x18-0x1F
    }
    op[0x06] = op[0x16] = &SharpSm83::Op_RLC_RL_memHL;
    op[0x0E] = op[0x1E] = &SharpSm83::Op_RRC_RR_memHL;

    for(auto i = 0x20; i < 0x30; ++i) {
        if((i & 0x7) == 0x6) continue;  // <op> reg, [HL]

        if((i & 0x8) == 0) op[i] = &SharpSm83::Op_SLA_reg;  // 0x20-0x27
        else               op[i] = &SharpSm83::Op_SRA_reg;  // 0x28-0x2F
    }
    op[0x26] = &SharpSm83::Op_SLA_memHL;
    op[0x2E] = &SharpSm83::Op_SRA_memHL;

    for(auto i = 0x30; i < 0x38; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_SWAP_memHL;
        else                 op[i] = &SharpSm83::Op_SWAP_reg;
    }

    for(auto i = 0x38; i < 0x40; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_SRL_memHL;
        else                 op[i] = &SharpSm83::Op_SRL_reg;
    }

    for(auto i = 0x40; i < 0x80; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_BIT_memHL;
        else                 op[i] = &SharpSm83::Op_BIT_reg;
    }

    for(auto i = 0x80; i < 0xC0; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_RES_memHL;
        else                 op[i] = &SharpSm83::Op_RES_reg;
    }

    for(auto i = 0xC0; i < 0x100; ++i) {
        if((i & 0x7) == 0x6) op[i] = &SharpSm83::Op_SET_memHL;
        else                 op[i] = &SharpSm83::Op_SET_reg;
    }

    return op;
}();

void SharpSm83::ServiceInterrupt(const int activeInterrupts) {
    if(m_mCycle == 1) { m_ime = false; }  // Disable interrupts.
    if(m_mCycle == 3) { PushByte(m_registers.PC >> 8); }
    if(m_mCycle == 4) { PushByte(m_registers.PC); }
    if(m_mCycle == 5) {
        // Jump to the interrupt vector.
        uint16_t destAddr = 0x40;
        for(auto i = 0; i < 5; i++) {
            // Interrupts are serviced in order of LSB to MSB.
            if((activeInterrupts & (1 << i)) > 0) {
                // Found one! Set the destination address, clear the flag, and jump.
                destAddr += i * 8;
                m_memory->SetByte(0xFF0F, GetInterruptFlag() ^ (1 << i));
                break;
            }
        }

        m_registers.PC = destAddr;
        m_state = SharpSm83State::Decode;
    }
}

// ReSharper disable once CppDFAConstantFunctionResult
long SharpSm83::DecodeAndExecute() {
    const auto activeInterrupts = GetInterruptEnable() & GetInterruptFlag() & 0b11111;

    // HALT Handler
    if(m_halt) {
        m_memoryBusState = MemoryBusState::None;

        if(activeInterrupts == 0) {
            // No pending interrupts. Do nothing.
            return MCycleLength;
        }

        // Interrupts are disabled. Wake up.
        m_halt = false;
    }

    // Interrupt Servicing
    if(m_ime && activeInterrupts && m_state == SharpSm83State::Decode) {
        m_state = SharpSm83State::Interrupt;
        m_mCycle = 0;
    }

    if(m_state == SharpSm83State::Interrupt) {
        ++m_mCycle;
        ServiceInterrupt(activeInterrupts);
        return MCycleLength;
    }

    // Opcode decoding/executing.
    if(m_state == SharpSm83State::Decode) {
        m_lastOp = m_op = FetchAtPc();
        m_state = SharpSm83State::Execute;
        m_mCycle = 0;  // Keep this above the increment. Most documentation refers to cycles as M1, M2, etc.
    } else {
        m_memoryBusState = MemoryBusState::None;
    }
    ++m_mCycle;
    (this->*OpTable[m_op])();

    return MCycleLength;
}

// Helper Methods
void SharpSm83::OpAddToRegisterA(const int value, const bool addWithCarry) {
    const auto carry = (addWithCarry && m_registers.GetCarryFlag()) ? 1 : 0;

    CheckAddHalfCarry(m_registers.A, value, carry);
    CheckAddCarry(m_registers.A, value + carry);
    m_registers.A += value + carry;
    m_registers.ClearSubtractFlag();
    m_registers.SetZeroFlagTo(!m_registers.A);
}

void SharpSm83::OpBitwiseAndRegisterA(const uint8_t value) {
    m_registers.A &= value;
    m_registers.ClearCarryFlag();
    m_registers.SetHalfCarryFlag();
    m_registers.ClearSubtractFlag();
    m_registers.SetZeroFlagTo(!m_registers.A);
}

void SharpSm83::OpBitwiseOrRegisterA(const uint8_t value) {
    m_registers.A |= value;
    m_registers.ClearCarryFlag();
    m_registers.ClearHalfCarryFlag();
    m_registers.ClearSubtractFlag();
    m_registers.SetZeroFlagTo(!m_registers.A);
}

void SharpSm83::OpBitwiseXorRegisterA(const uint8_t value) {
    m_registers.A ^= value;
    m_registers.ClearCarryFlag();
    m_registers.ClearHalfCarryFlag();
    m_registers.ClearSubtractFlag();
    m_registers.SetZeroFlagTo(!m_registers.A);
}

void SharpSm83::OpSubtractFromRegisterA(const int value, const bool subtractWithBorrow, const bool discardResult) {
    const auto borrow = ((subtractWithBorrow && m_registers.GetCarryFlag()) ? 1 : 0);
    const uint8_t result = m_registers.A - value - borrow;

    CheckSubHalfBorrow(m_registers.A, value, borrow);
    CheckSubBorrow(m_registers.A, value, borrow);
    m_registers.SetSubtractFlag();
    m_registers.SetZeroFlagTo(!result);

    if(!discardResult) m_registers.A = result;
}

uint8_t SharpSm83::OpRotateLeft(uint8_t value, const bool throughCarry, const bool checkZeroFlag) {
    m_registers.ClearSubtractFlag();
    m_registers.ClearHalfCarryFlag();

    const auto newLeastSignificantBit = throughCarry
        ? (m_registers.GetCarryFlag() ? 1 : 0)
        : (value & 0b10000000) >> 7;

    (value & 0b10000000) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
    value <<= 1;
    value |= newLeastSignificantBit;

    (checkZeroFlag && value == 0) ? m_registers.SetZeroFlag() : m_registers.ClearZeroFlag();

    return value;
}

uint8_t SharpSm83::OpRotateRight(uint8_t value, const bool throughCarry, const bool checkZeroFlag) {
    m_registers.ClearSubtractFlag();
    m_registers.ClearHalfCarryFlag();

    const auto newMostSignificantBit = throughCarry
        ? (m_registers.GetCarryFlag() ? 1 : 0)
        : (value & 0b1);

    (value & 0b1) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
    value >>= 1;
    value |= (newMostSignificantBit << 7);

    (checkZeroFlag && value == 0) ? m_registers.SetZeroFlag() : m_registers.ClearZeroFlag();

    return value;
}

uint8_t SharpSm83::OpShiftLeft(uint8_t value) {
    m_registers.ClearSubtractFlag();
    m_registers.ClearHalfCarryFlag();

    (value & 0b10000000) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
    value <<= 1;
    m_registers.SetZeroFlagTo(!value);

    return value;
}

uint8_t SharpSm83::OpShiftRight(uint8_t value, const bool arithmetic) {
    m_registers.ClearSubtractFlag();
    m_registers.ClearHalfCarryFlag();

    (value & 0b1) ? m_registers.SetCarryFlag() : m_registers.ClearCarryFlag();
    value >>= 1;
    if(arithmetic && (value & 0b01000000)) value |= 0b10000000;
    m_registers.SetZeroFlagTo(!value);

    return value;
}

uint8_t SharpSm83::OpSwapNibbles(uint8_t value) {
    m_registers.ClearSubtractFlag();
    m_registers.ClearHalfCarryFlag();
    m_registers.ClearCarryFlag();

    value = (value << 4) | (value >> 4);
    m_registers.SetZeroFlagTo(!value);

    return value;
}

// NOTE: Methods called via the optables *must* *not* be static/const. Prevent clang-tidy and CLion from
// getting excited.

// NOLINTBEGIN(*-make-member-function-const)
// ReSharper disable CppMemberFunctionMayBeConst
// ReSharper disable CppMemberFunctionMayBeStatic

void SharpSm83::Op_CB() {
    if(m_mCycle == 2) {
        m_cb = FetchAtPc();
        m_lastOp = 0xCB00 | m_cb;
    }

    if(m_mCycle >= 2) {
        (this->*OpTableCb[m_cb])();
    }
}


//
// Exceptional Exception Executors
//
void SharpSm83::Op_Invalid() { throw PlipInvalidOpcodeException(m_op); }
void SharpSm83::Op_InvalidCB() { throw PlipInvalidOpcodeException(0xCB00 | m_op); }


//
// Miscellaneous / Control Instructions
//
void SharpSm83::Op_NOP() {
    // NOP
    // 1 cycle, - - - -
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_STOP() {
    // STOP imm8
    // 2 cycles, - - - -
    if(!m_gbcMode) {
        // According to PanDocs, no licensed DMG ROM uses STOP, so we're gonna leave
        // it like this for now. :)
        throw PlipEmulationException("STOP not yet implemented for the DMG CPU", true);
    }

    // In CGB mode, STOP is used to put the CPU into double speed mode.
    if(!BIT_TEST(m_memory->GetByte(0xFF4D), 0)) {
        // STOP called, but the switch has not been armed. Decode again on the second M-cycle.
        if(m_mCycle == 2) { m_state = SharpSm83State::Decode; }
        return;
    }

    // Switch speed.
    m_changingSpeed = true;
    m_speedChangeTimer = SpeedSwitchDelay;
}

void SharpSm83::Op_HALT() {
    // HALT
    // 1 cycle, - - - -
    if(const auto activeInterrupts = GetInterruptEnable() & GetInterruptFlag() & 0b11111; !m_ime && activeInterrupts != 0) {
        // HALT bug triggered. The CPU will not be halted, interrupts will
        // not be serviced, and the PC will be NOT be incremented during the
        // next fetch.
        m_holdPc = true;
    } else {
        m_halt = true;
    }

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_DI() {
    // DI
    // 1 cycle, - - - -
    m_ime = false;
    m_imeDelay = 0;
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_EI() {
    // EI
    // 1 cycle, - - - -
    if(m_imeDelay == 0) m_imeDelay = InitialImeDelay;
    m_state = SharpSm83State::Decode;
}


//
// Jumps / Calls
//
void SharpSm83::Op_JR_imm8s() {
    // JR imm8s
    // 3 cycles, - - - -
    if(m_mCycle == 2) { FetchAtPc(); }
    if(m_mCycle == 3) {
        m_registers.PC += std::bit_cast<int8_t>(m_dataBus);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_JR_c_imm8s() {
    // JR c, imm8s
    // 3/2 cycles, - - - -
    if(m_mCycle == 2) {
        FetchAtPc();

        if(!TestConditional(GetOpConditional())) {
            m_state = SharpSm83State::Decode;
        }
    }
    if(m_mCycle == 3) {
        m_registers.PC += std::bit_cast<int8_t>(m_dataBus);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_RET() {
    // RET/RETI
    // 4 cycles, - - - -
    if(m_mCycle == 2) { m_z = PopByte(); }
    if(m_mCycle == 3) { m_w = PopByte(); }
    if(m_mCycle == 4) {
        if(m_op == 0xD9) m_ime = true;  // RETI

        m_registers.PC = (m_w << 8) | m_z;
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_RET_c() {
    // RET c
    // 5/2 cycles, - - - -
    if(m_mCycle == 2) {
        if(!TestConditional(GetOpConditional())) {
            m_state = SharpSm83State::Decode;
        }
    }
    if(m_mCycle == 3) { m_z = PopByte(); }
    if(m_mCycle == 4) { m_w = PopByte(); }
    if(m_mCycle == 5) {
        m_registers.PC = (m_w << 8) | m_z;
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_JP_imm16() {
    // JP imm16
    // 4 cycles, - - - -
    if(m_mCycle == 2) { m_z = FetchAtPc(); }
    if(m_mCycle == 3) { FetchAtPc(); }
    if(m_mCycle == 4) {
        m_registers.PC = (m_dataBus << 8) | m_z;
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_JP_c_imm16() {
    // JP c, imm16
    // 4/3 cycles, - - - -
    if(m_mCycle == 2) { m_z = FetchAtPc(); }
    if(m_mCycle == 3) {
        FetchAtPc();
        if(!TestConditional(GetOpConditional())) {
            m_state = SharpSm83State::Decode;
        }
    }
    if(m_mCycle == 4) {
        m_registers.PC = (m_dataBus << 8) | m_z;
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_RST_vec() {
    // RST vec
    // 4 cycles, - - - -
    if(m_mCycle == 3) { PushByte(m_registers.PC >> 8); }
    if(m_mCycle == 4) {
        PushByte(m_registers.PC);

        m_registers.PC = GetOpParamX() * 8;
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_JP_HL() {
    // JP HL
    // 1 cycle, - - - -
    m_registers.PC = m_registers.GetHl();
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_CALL_imm16() {
    // CALL imm16
    // 6 cycles, - - - -
    if(m_mCycle == 2) { m_z = FetchAtPc(); }
    if(m_mCycle == 3) { m_w = FetchAtPc(); }
    if(m_mCycle == 5) { PushByte(m_registers.PC >> 8); }
    if(m_mCycle == 6) {
        PushByte(m_registers.PC);
        m_registers.PC = (m_w << 8) | m_z;

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_CALL_c_imm16() {
    // CALL c, imm16
    // 6/3 cycles, - - - -
    if(m_mCycle == 2) { m_z = FetchAtPc(); }
    if(m_mCycle == 3) {
        m_w = FetchAtPc();

        if(!TestConditional(GetOpConditional())) {
            m_state = SharpSm83State::Decode;
        }
    }
    if(m_mCycle == 5) { PushByte(m_registers.PC >> 8); }
    if(m_mCycle == 6) {
        PushByte(m_registers.PC);
        m_registers.PC = (m_w << 8) | m_z;

        m_state = SharpSm83State::Decode;
    }
}


//
// 8-bit Load Instructions
//
void SharpSm83::Op_LD_mem_A() {
    // LD [xx], A
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        WriteMemory(GetPointerAddress(GetOpParam16()), m_registers.A);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_reg_imm8() {
    // LD x, imm8
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        m_registers.Set8ByIndex(GetOpParamX(), FetchAtPc());
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_A_mem() {
    // LD A, [xx]
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        m_registers.A = ReadMemory(GetPointerAddress(GetOpParam16()));
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_memHL_imm8() {
    // LD [HL], imm8
    // 3 cycles, - - - -
    if(m_mCycle == 2) { FetchAtPc(); }
    if(m_mCycle == 3) {
        WriteMemory(m_registers.GetHl(), m_dataBus);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_reg_reg() {
    // LD x, y
    // 1 cycle, - - - -
    m_registers.Set8ByIndex(GetOpParamX(), m_registers.Get8ByIndex(GetOpParamY()));
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_LD_reg_memHL() {
    // LD x, [HL]
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        ReadMemory(m_registers.GetHl());
        m_registers.Set8ByIndex(GetOpParamX(), m_dataBus);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_memHL_reg() {
    // LD [HL], y
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        WriteMemory(m_registers.GetHl(), m_registers.Get8ByIndex(GetOpParamY()));
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LDH_memImm8_A() {
    // LD [imm8], A
    // 3 cycles, - - - -
    if(m_mCycle == 2) { FetchAtPc(); }
    if(m_mCycle == 3) {
        WriteMemory(0xFF00 | m_dataBus, m_registers.A);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LDH_memC_A() {
    // LDH [C], A
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        WriteMemory(0xFF00 | m_registers.C, m_registers.A);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_memImm16_A() {
    // LD [imm16], A
    // 4 cycles, - - - -
    if(m_mCycle == 2) { m_z = FetchAtPc(); }
    if(m_mCycle == 3) { FetchAtPc(); }
    if(m_mCycle == 4) {
        WriteMemory((m_dataBus << 8) | m_z, m_registers.A);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LDH_A_imm8() {
    // LDH A, [imm8]
    // 3 cycles, - - - -
    if(m_mCycle == 2) { FetchAtPc(); }
    if(m_mCycle == 3) {
        ReadMemory(0xFF00 | m_dataBus);
        m_registers.A = m_dataBus;
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LDH_A_memC() {
    // LDH A, [C]
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        ReadMemory(0xFF00 | m_registers.C);
        m_registers.A = m_dataBus;
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_A_memImm16() {
    // LD A, [imm16]
    // 4 cycles, - - - -
    if(m_mCycle == 2) {
        FetchAtPc();
        m_z = m_dataBus;
    }
    if(m_mCycle == 3) { FetchAtPc(); }
    if(m_mCycle == 4) {
        ReadMemory((m_dataBus << 8) | m_z);
        m_registers.A = m_dataBus;
        m_state = SharpSm83State::Decode;
    }
}


//
// 16-bit Load Instructions
//
void SharpSm83::Op_LD_reg16_imm16() {
    // LD xx, imm16
    // 3 cycles, - - - -
    if(m_mCycle == 2) { m_z = FetchAtPc(); }
    if(m_mCycle == 3) {
        FetchAtPc();
        m_registers.Set16ByIndex(GetOpParam16(), (m_dataBus << 8) | m_z);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_memImm16_SP() {
    // LD [imm16], SP
    // 5 cycles, - - - - -
    if(m_mCycle == 2) { m_z = FetchAtPc(); }
    if(m_mCycle == 3) { m_w = FetchAtPc(); }
    if(m_mCycle == 4) { WriteMemory((m_w << 8) | m_z, m_registers.SP); }
    if(m_mCycle == 5) {
        WriteMemory(((m_w << 8) | m_z) + 1, m_registers.SP >> 8);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_POP_reg16() {
    // POP zz
    // 3 cycles, BC/DE/HL: - - - -, AF: Z N H C
    if(m_mCycle == 2) { m_z = PopByte(); }
    if(m_mCycle == 3) {
        m_w = PopByte();

        if(const auto regIndex = GetOpParam16(); regIndex == SharpSm83Registers::RegIndex16Af) {
            // AF shares an index with SP, but it must be handled differently.
            m_registers.A = m_w;
            m_registers.F = m_z & 0xF0;
        } else {
            m_registers.Set16ByIndex(regIndex, (m_w << 8) | m_z);
        }

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_PUSH_reg16() {
    // PUSH zz
    // 4 cycles, - - - -
    if(m_mCycle == 2) {
        if(const auto regIndex = GetOpParam16(); regIndex == SharpSm83Registers::RegIndex16Af) {
            // AF shares an index with SP, but it must be handled differently.
            m_z = m_registers.F & 0xF0;
            m_w = m_registers.A;
        } else {
            const auto val = m_registers.Get16ByIndex(regIndex);
            m_z = val;
            m_w = val >> 8;
        }
    }
    if(m_mCycle == 3) { PushByte(m_w); }
    if(m_mCycle == 4) {
        PushByte(m_z);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_HL_SPimm8s() {
    // LD HL, SP + imm8s
    // 3 cycles, 0 0 H C
    if(m_mCycle == 2) { FetchAtPc(); }
    if(m_mCycle == 3) {
        const auto val = std::bit_cast<int8_t>(m_dataBus);

        m_registers.SetHl(m_registers.SP + val);

        m_registers.ClearSubtractFlag();
        m_registers.ClearZeroFlag();

        ((m_registers.SP & 0xFF) + static_cast<uint8_t>(val) > 0xFF)
            ? m_registers.SetCarryFlag()
            : m_registers.ClearCarryFlag();

        ((m_registers.SP & 0x0F) + (static_cast<uint8_t>(val) & 0x0F) > 0x0F)
            ? m_registers.SetHalfCarryFlag()
            : m_registers.ClearHalfCarryFlag();

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_LD_SP_HL() {
    // LD SP, HL
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        m_registers.SP = m_registers.GetHl();
        m_state = SharpSm83State::Decode;
    }
}


//
// 8-bit Arithmetic / Logical Instructions
//
void SharpSm83::Op_INC_reg() {
    // INC x
    // 1 cycle, Z 0 H -
    const auto regIndex = GetOpParamX();
    uint8_t regValue = m_registers.Get8ByIndex(regIndex);

    CheckAddHalfCarry(regValue, 1);
    m_registers.ClearSubtractFlag();

    m_registers.Set8ByIndex(regIndex, ++regValue);
    m_registers.SetZeroFlagTo(!regValue);

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_INC_memHL() {
    // INC [HL]
    // 3 cycles, Z 0 H -
    if(m_mCycle == 2) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 3) {
        CheckAddHalfCarry(m_dataBus, 1);
        m_registers.ClearSubtractFlag();

        WriteMemory(m_registers.GetHl(), m_dataBus + 1);
        m_registers.SetZeroFlagTo(!m_dataBus);

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_DEC_reg() {
    // DEC x
    // 1 cycle, Z 1 H -
    const auto regIndex = GetOpParamX();
    uint8_t regValue = m_registers.Get8ByIndex(regIndex);

    CheckSubHalfBorrow(regValue, 1);
    m_registers.SetSubtractFlag();

    m_registers.Set8ByIndex(regIndex, --regValue);
    m_registers.SetZeroFlagTo(!regValue);

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_DEC_memHL() {
    // DEC [HL]
    // 3 cycles, Z 0 H -
    if(m_mCycle == 2) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 3) {
        CheckSubHalfBorrow(m_dataBus, 1);
        m_registers.SetSubtractFlag();

        WriteMemory(m_registers.GetHl(), m_dataBus - 1);
        m_registers.SetZeroFlagTo(!m_dataBus);

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_DAA() {
    // DAA
    // 1 cycle, Z - 0 C
    uint8_t adjustment = 0;

    if(m_registers.GetSubtractFlag()) {
        adjustment |= m_registers.GetHalfCarryFlag() ? 0x06 : 0;
        adjustment |= m_registers.GetCarryFlag() ? 0x60 : 0;
        m_registers.A -= adjustment;
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

    m_registers.SetZeroFlagTo(!m_registers.A);
    m_registers.ClearHalfCarryFlag();

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_CPL() {
    // CPL
    // 1 cycle, - 1 1 -
    m_registers.SetSubtractFlag();
    m_registers.SetHalfCarryFlag();

    m_registers.A ^= 0xFF;

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_SCF() {
    // SCF
    // 1 cycle, - 0 0 1
    m_registers.ClearSubtractFlag();
    m_registers.ClearHalfCarryFlag();
    m_registers.SetCarryFlag();

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_CCF() {
    // CCF
    // 1 cycle, - 0 0 C
    m_registers.ClearSubtractFlag();
    m_registers.ClearHalfCarryFlag();
    m_registers.GetCarryFlag() ? m_registers.ClearCarryFlag() : m_registers.SetCarryFlag();

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_ADD_ADC_A_reg() {
    // ADD A, y (0x80-0x87)
    // ADC A, y (0x88-0x8F)
    // 1 cycle, Z 0 H C
    OpAddToRegisterA(m_registers.Get8ByIndex(GetOpParamY()), m_op >= 0x88);
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_ADD_ADC_A_memHL() {
    // ADD A, [HL] (0x86)
    // ADC A, [HL] (0x8E)
    // 2 cycles, Z 0 H C
    if(m_mCycle == 2) {
        OpAddToRegisterA(ReadMemory(m_registers.GetHl()), m_op == 0x8E);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_ADD_ADC_A_imm8() {
    // ADD A, imm8 (0xC6)
    // ADC A, imm8 (0xCE)
    // 2 cycles, Z 0 H C
    if(m_mCycle == 2) {
        OpAddToRegisterA(FetchAtPc(), m_op == 0xCE);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_SUB_SBC_A_reg() {
    // SUB A, y (0x90-0x97)
    // SBC A, y (0x98-0x9F)
    // 1 cycle, Z 1 H C
    OpSubtractFromRegisterA(m_registers.Get8ByIndex(GetOpParamY()), m_op >= 0x98, false);
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_SUB_SBC_A_memHL() {
    // SUB A, [HL] (0x96)
    // SBC A, [HL] (0x9E)
    // 2 cycles, Z 1 H C
    if(m_mCycle == 2) {
        OpSubtractFromRegisterA(ReadMemory(m_registers.GetHl()), m_op == 0x9E, false);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_SUB_SBC_A_imm8() {
    // SUB A, imm8 (0xD6)
    // SBC A, imm8 (0xDE)
    // 2 cycles, Z 1 H C
    if(m_mCycle == 2) {
        OpSubtractFromRegisterA(FetchAtPc(), m_op == 0xDE, false);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_AND_A_reg() {
    // AND A, y
    // 1 cycle, Z 0 1 0
    OpBitwiseAndRegisterA(m_registers.Get8ByIndex(GetOpParamY()));
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_AND_A_memHL() {
    // AND A, [HL]
    // 2 cycles, Z 0 1 0
    if(m_mCycle == 2) {
        OpBitwiseAndRegisterA(ReadMemory(m_registers.GetHl()));
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_AND_A_imm8() {
    // AND A, imm8
    // 2 cycles, Z 0 1 0
    if(m_mCycle == 2) {
        OpBitwiseAndRegisterA(FetchAtPc());
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_XOR_A_reg() {
    // XOR A, y
    // 1 cycle, Z 0 0 0
    OpBitwiseXorRegisterA(m_registers.Get8ByIndex(GetOpParamY()));
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_XOR_A_memHL() {
    // XOR A, [HL]
    // 2 cycles, Z 0 0 0
    if(m_mCycle == 2) {
        OpBitwiseXorRegisterA(ReadMemory(m_registers.GetHl()));
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_XOR_A_imm8() {
    // XOR A, imm8
    // 2 cycles, Z 0 0 0
    if(m_mCycle == 2) {
        OpBitwiseXorRegisterA(FetchAtPc());
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_OR_A_reg() {
    // OR A, y
    // 1 cycle, Z 0 0 0
    OpBitwiseOrRegisterA(m_registers.Get8ByIndex(GetOpParamY()));
    m_state = SharpSm83State::Decode;

}

void SharpSm83::Op_OR_A_memHL() {
    // OR A, [HL]
    // 2 cycles, Z 0 0 0
    if(m_mCycle == 2) {
        OpBitwiseOrRegisterA(ReadMemory(m_registers.GetHl()));
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_OR_A_imm8() {
    // OR A, imm8
    // 2 cycles, Z 0 0 0
    if(m_mCycle == 2) {
        OpBitwiseOrRegisterA(FetchAtPc());
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_CP_A_reg() {
    // CP A, y
    // 1 cycle, Z 1 H C
    OpSubtractFromRegisterA(m_registers.Get8ByIndex(GetOpParamY()), false, true);
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_CP_A_memHL() {
    // CP A, [HL]
    // 2 cycles, Z 1 H C
    if(m_mCycle == 2) {
        OpSubtractFromRegisterA(ReadMemory(m_registers.GetHl()), false, true);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_CP_A_imm8() {
    // CP A, imm8
    // 2 cycles, Z 1 H C
    if(m_mCycle == 2) {
        OpSubtractFromRegisterA(FetchAtPc(), false, true);
        m_state = SharpSm83State::Decode;
    }
}


//
// 16-bit Arithmetic
//
void SharpSm83::Op_INC_reg16() {
    // INC zz
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        const auto regIndex = GetOpParam16();
        m_registers.Set16ByIndex(regIndex, m_registers.Get16ByIndex(regIndex) + 1);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_DEC_reg16() {
    // DEC zz
    // 2 cycles, - - - -
    if(m_mCycle == 2) {
        const auto regIndex = GetOpParam16();
        m_registers.Set16ByIndex(regIndex, m_registers.Get16ByIndex(regIndex) - 1);
        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_ADD_HL_reg16() {
    // ADD HL, zz
    // 2 cycles, - 0 H C
    if(m_mCycle == 2) {
        const auto regValue = m_registers.Get16ByIndex(GetOpParam16());

        int part = regValue & 0xFF;
        const auto lowCarry = (m_registers.L + part) > 0xFF;
        m_registers.L += part;

        part = regValue >> 8;
        const auto carry = lowCarry ? 1 : 0;
        CheckAddCarry(m_registers.H, part + carry);
        CheckAddHalfCarry(m_registers.H, part, carry);
        m_registers.H += part + carry;
        m_registers.ClearSubtractFlag();

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_ADD_SP_imm8s() {
    // ADD SP, imm8s
    // 4 cycles, 0 0 H C
    if(m_mCycle == 2) { FetchAtPc(); }
    if(m_mCycle == 3) {
        m_registers.ClearZeroFlag();
        m_registers.ClearSubtractFlag();
        CheckAddHalfCarry(m_registers.SP & 0x0F, m_dataBus);
        CheckAddCarry(m_registers.SP & 0xFF, m_dataBus);
    }
    if(m_mCycle == 4) {
        if(m_dataBus & 0b10000000) {
            // imm8s is negative
            m_registers.SP -= (m_dataBus ^ 0xFF) + 1;
        } else {
            m_registers.SP += m_dataBus;
        }

        m_state = SharpSm83State::Decode;
    }
}


//
// 8-bit Shift, Rotate, and Bit Instructions
//
void SharpSm83::Op_RLCA() {
    // RLCA
    // 1 cycle, 0 0 0 C
    m_registers.A = OpRotateLeft(m_registers.A, false, false);
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_RLA() {
    // RLA
    // 1 cycle, 0 0 0 C
    m_registers.A = OpRotateLeft(m_registers.A, true, false);
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_RRCA() {
    // RRCA
    // 1 cycle, 0 0 0 C
    m_registers.A = OpRotateRight(m_registers.A, false, false);
    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_RRA() {
    // RRA
    // 1 cycle, 0 0 0 C
    m_registers.A = OpRotateRight(m_registers.A, true, false);
    m_state = SharpSm83State::Decode;
}


//
// Bit Manipulation (0xCB Ops)
//
void SharpSm83::Op_RLC_RL_reg() {
    // RLC y (0x00-0x07)
    // RL y (0x10-0x17)
    // 2 cycles, Z 0 0 C
    const auto regIndex = GetCbParamY();
    const auto regValue = m_registers.Get8ByIndex(regIndex);
    const auto result = OpRotateLeft(regValue, m_cb >= 0x10, true);
    m_registers.Set8ByIndex(regIndex, result);

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_RLC_RL_memHL() {
    // RLC [HL] (0x06)
    // RL [HL] (0x16)
    // 4 cycles, Z 0 0 C
    if(m_mCycle == 3) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 4) {
        const auto result = OpRotateLeft(m_dataBus, m_cb == 0x16, true);
        WriteMemory(m_registers.GetHl(), result);

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_RRC_RR_reg() {
    // RRC y (0x08-0x0F)
    // RR y (0x18-0x1F)
    // 2 cycles, Z 0 0 C
    const auto regIndex = GetCbParamY();
    const auto regValue = m_registers.Get8ByIndex(regIndex);
    const auto result = OpRotateRight(regValue, m_cb >= 0x18, true);
    m_registers.Set8ByIndex(regIndex, result);

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_RRC_RR_memHL() {
    // RRC [HL] (0x0E)
    // RR [HL] (0x1E)
    // 4 cycles, Z 0 0 C
    if(m_mCycle == 3) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 4) {
        const auto result = OpRotateRight(m_dataBus, m_cb == 0x1E, true);
        WriteMemory(m_registers.GetHl(), result);

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_SLA_reg() {
    // SLA y
    // 2 cycles, Z 0 0 C
    const auto regIndex = GetCbParamY();
    m_registers.Set8ByIndex(regIndex, OpShiftLeft(m_registers.Get8ByIndex(regIndex)));

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_SLA_memHL() {
    // SLA [HL]
    // 4 cycles, Z 0 0 C
    if(m_mCycle == 3) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 4) {
        const auto result = OpShiftLeft(m_dataBus);
        WriteMemory(m_registers.GetHl(), result);

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_SRA_reg() {
    // SRA y
    // 2 cycles, Z 0 0 C
    const auto regIndex = GetCbParamY();
    m_registers.Set8ByIndex(regIndex, OpShiftRight(m_registers.Get8ByIndex(regIndex), true));

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_SRA_memHL() {
    // SRA [HL]
    // 4 cycles, Z 0 0 C
    if(m_mCycle == 3) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 4) {
        const auto result = OpShiftRight(m_dataBus, true);
        WriteMemory(m_registers.GetHl(), result);

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_SWAP_reg() {
    // SWAP y
    // 2 cycles, Z 0 0 0
    const auto regIndex = GetCbParamY();
    m_registers.Set8ByIndex(regIndex, OpSwapNibbles(m_registers.Get8ByIndex(regIndex)));

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_SWAP_memHL() {
    // SWAP [HL]
    // 4 cycles, Z 0 0 0
    if(m_mCycle == 3) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 4) {
        const auto result = OpSwapNibbles(m_dataBus);
        WriteMemory(m_registers.GetHl(), result);

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_SRL_reg() {
    // SRL y
    // 2 cycles, Z 0 0 C
    const auto regIndex = GetCbParamY();
    m_registers.Set8ByIndex(regIndex, OpShiftRight(m_registers.Get8ByIndex(regIndex), false));

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_SRL_memHL() {
    // SRL [HL]
    // 4 cycles, Z 0 0 C
    if(m_mCycle == 3) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 4) {
        const auto result = OpShiftRight(m_dataBus, false);
        WriteMemory(m_registers.GetHl(), result);

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_BIT_reg() {
    // BIT imm3, y
    // 2 cycles, Z 0 1 -
    ((m_registers.Get8ByIndex(GetCbParamY()) >> GetCbParamX()) & 0b1)
        ? m_registers.ClearZeroFlag() : m_registers.SetZeroFlag();
    m_registers.ClearSubtractFlag();
    m_registers.SetHalfCarryFlag();

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_BIT_memHL() {
    // BIT imm3, [HL]
    // 3 cycles, Z 0 1 -
    if(m_mCycle == 3) {
        ReadMemory(m_registers.GetHl());

        ((m_dataBus >> GetCbParamX()) & 0b1) ? m_registers.ClearZeroFlag() : m_registers.SetZeroFlag();
        m_registers.ClearSubtractFlag();
        m_registers.SetHalfCarryFlag();

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_RES_reg() {
    // RES imm3, y
    // 2 cycles, - - - -
    const auto regIndex = GetCbParamY();
    m_registers.Set8ByIndex(regIndex, BIT_CLEAR(m_registers.Get8ByIndex(regIndex), GetCbParamX()));

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_RES_memHL() {
    // RES imm3, [HL]
    // 4 cycles, - - - -
    if(m_mCycle == 3) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 4) {
        const auto result = BIT_CLEAR(m_dataBus, GetCbParamX());
        WriteMemory(m_registers.GetHl(), result);

        m_state = SharpSm83State::Decode;
    }
}

void SharpSm83::Op_SET_reg() {
    // SET imm3, y
    // 2 cycles, - - - -
    const auto regIndex = GetCbParamY();
    m_registers.Set8ByIndex(regIndex, BIT_SET(m_registers.Get8ByIndex(regIndex), GetCbParamX()));

    m_state = SharpSm83State::Decode;
}

void SharpSm83::Op_SET_memHL() {
    // SET imm3, [HL]
    // 4 cycles, - - - -
    if(m_mCycle == 3) { ReadMemory(m_registers.GetHl()); }
    if(m_mCycle == 4) {
        const auto result = BIT_SET(m_dataBus, GetCbParamX());
        WriteMemory(m_registers.GetHl(), result);

        m_state = SharpSm83State::Decode;
    }
}

// ReSharper restore CppMemberFunctionMayBeStatic
// ReSharper restore CppMemberFunctionMayBeConst
// NOLINTEND(*-make-member-function-const)
