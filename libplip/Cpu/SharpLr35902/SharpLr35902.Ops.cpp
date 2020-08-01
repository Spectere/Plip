/* SharpLr35902.Ops.cpp
 *
 * Sharp LR35902 opcode execution.
 */

#include <sstream>

#include "../../PlipEmulationException.h"

#include "SharpLr35902.h"
#include "SharpLr35902.Macros.h"

namespace Plip::Cpu {
    /*
     * Standard opcodes: starts at m_mcycle == 2.
     */
    // ADD A, r
    // ADD A, (HL)
    void SharpLr35902::OpAdd() {
        auto src = OP_REG_Y(0);
        uint16_t res;

        if(src == IDX_HL) {
            // ADD A, (HL)
            if(m_mcycle == 2) {
                FETCH_ADDR(REG_HL);
            } else {
                res = m_reg.a + m_instr[1];
                CHECK_CARRY(res);
                CHECK_HALFCARRY(m_reg.a, res);
                m_reg.a = res & 0xFF;
                CHECK_ZERO(m_reg.a);
                FLAG_CLEAR(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // ADD A, r
        res = m_reg.a + *(GetRegister8(src));
        CHECK_CARRY(res);
        CHECK_HALFCARRY(m_reg.a, res);
        m_reg.a = res & 0xFF;
        CHECK_ZERO(m_reg.a);
        FLAG_CLEAR(SUBTRACT);

        NUM_MCYCLES(2);
    }

    // ADC A, r
    // ADC A, (HL)
    void SharpLr35902::OpAddCarry() {
        auto src = OP_REG_Y(0);
        uint16_t res;

        if(src == IDX_HL) {
            // ADC A, (HL)
            if(m_mcycle == 2) {
                FETCH_ADDR(REG_HL);
            } else {
                res = m_reg.a + m_instr[1] + FLAG_TEST(CARRY) ? 1 : 0;
                CHECK_CARRY(res);
                CHECK_HALFCARRY(m_reg.a, res);
                m_reg.a = res & 0xFF;
                CHECK_ZERO(m_reg.a);
                FLAG_CLEAR(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // ADC A, r
        res = m_reg.a + *(GetRegister8(src)) + FLAG_TEST(CARRY) ? 1 : 0;
        CHECK_CARRY(res);
        CHECK_HALFCARRY(m_reg.a, res);
        m_reg.a = res & 0xFF;
        CHECK_ZERO(m_reg.a);
        FLAG_CLEAR(SUBTRACT);

        NUM_MCYCLES(2);
    }

    // AND r
    // AND (HL)
    void SharpLr35902::OpAnd() {
        auto src = OP_REG_Y(0);

        if(src == IDX_HL) {
            // AND (HL)
            if(m_mcycle == 2) {
                FETCH_ADDR(REG_HL);
            } else {
                m_reg.a &= m_instr[1];
                CHECK_ZERO(m_reg.a);
                FLAG_CLEAR(SUBTRACT);
                FLAG_CLEAR(CARRY);
                FLAG_SET(HALFCARRY);
            }
            NUM_MCYCLES(3);
            return;
        }

        // AND r
        m_reg.a &= *(GetRegister8(src));
        CHECK_ZERO(m_reg.a);
        FLAG_CLEAR(SUBTRACT);
        FLAG_CLEAR(CARRY);
        FLAG_SET(HALFCARRY);

        NUM_MCYCLES(2);
    }

    // CP r
    // CP (HL)
    void SharpLr35902::OpCarry() {
        auto src = OP_REG_Y(0);
        uint16_t res;

        if(src == IDX_HL) {
            // CP (HL)
            if(m_mcycle == 2) {
                FETCH_ADDR(REG_HL);
            } else {
                res = m_reg.a - m_instr[1];
                CHECK_CARRY(res);
                CHECK_HALFCARRY(m_reg.a, res);
                CHECK_ZERO(res & 0xFF);
                FLAG_SET(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // CP r
        res = m_reg.a - *(GetRegister8(src));
        CHECK_CARRY(res);
        CHECK_HALFCARRY(m_reg.a, res);
        CHECK_ZERO(res & 0xFF);
        FLAG_SET(SUBTRACT);

        NUM_MCYCLES(2);
    }

    // DEC r
    // DEC (HL)
    void SharpLr35902::OpDecReg() {
        auto dest = OP_REG_X(0);
        uint8_t old;

        if(dest == IDX_HL) {
            // DEC (HL)
            switch(m_mcycle) {
                case 2:
                    FETCH_ADDR(REG_HL);
                    old = m_instr[1];
                    (m_instr[1])--;

                    if((m_instr[1] & 0xF0) != (old & 0xF0)) FLAG_SET(HALFCARRY);
                    else FLAG_CLEAR(HALFCARRY);

                    break;

                case 3:
                    MEM_WRITE(REG_HL, m_instr[1]);

                    FLAG_SET(SUBTRACT);
                    CHECK_ZERO(m_instr[1]);
                    break;

                default: break;
            }

            NUM_MCYCLES(4);
            return;
        }

        // DEC r
        auto reg = GetRegister8(dest);
        old = *reg;
        (*reg)--;

        FLAG_SET(SUBTRACT);
        CHECK_ZERO(*reg);

        if((old & 0xF0) != ((*reg) & 0xF0)) FLAG_SET(HALFCARRY);
        else FLAG_CLEAR(HALFCARRY);

        NUM_MCYCLES(2);
    }

    // DEC rr
    void SharpLr35902::OpDecPair() {
        uint8_t *high = nullptr;
        uint8_t *low = nullptr;

        std::tie(high, low) = GetRegisterPair(OP_REG_16(0));
        DecPair(high, low);
        NUM_MCYCLES(2);
    }

    // INC r
    // INC (HL)
    void SharpLr35902::OpIncReg() {
        auto dest = OP_REG_X(0);
        uint8_t old;

        if(dest == IDX_HL) {
            // INC (HL)
            switch(m_mcycle) {
                case 2:
                    FETCH_ADDR(REG_HL);
                    old = m_instr[1];
                    (m_instr[1])++;

                    if((m_instr[1] & 0xF0) != (old & 0xF0)) FLAG_SET(HALFCARRY);
                    else FLAG_CLEAR(HALFCARRY);

                    break;

                case 3:
                    MEM_WRITE(REG_HL, m_instr[1]);

                    FLAG_CLEAR(SUBTRACT);
                    CHECK_ZERO(m_instr[1]);
                    break;

                default: break;
            }

            NUM_MCYCLES(4);
            return;
        }

        // INC r
        auto reg = GetRegister8(dest);
        old = *reg;
        (*reg)--;

        FLAG_CLEAR(SUBTRACT);
        CHECK_ZERO(*reg);

        if((old & 0xF0) != ((*reg) & 0xF0)) FLAG_SET(HALFCARRY);
        else FLAG_CLEAR(HALFCARRY);

        NUM_MCYCLES(2);
    }

    // INC rr
    void SharpLr35902::OpIncPair() {
        uint8_t *high = nullptr;
        uint8_t *low = nullptr;

        std::tie(high, low) = GetRegisterPair(OP_REG_16(0));
        IncPair(high, low);
        NUM_MCYCLES(2);
    }

    // LD (HL), r
    // LD (rr), A
    void SharpLr35902::OpLdMemReg() {
        uint16_t addr;
        uint8_t *src;

        if(m_mcycle == 2) {
            if(OP_MASK(0b11111000, 0b01110000)) {
                // LD (HL), r
                addr = REG_HL;
                src = GetRegister8(OP_REG_X(0));
            } else if(OP_MASK(0b11001111, 0b00000010)) {
                // LD (rr), A
                addr = GetAddress((m_instr[0] >> 4) & 0b11);
                src = &(m_reg.a);
            } else {
                std::stringstream ex;
                ex << "OpLdMemReg: invalid operation\n\n" << DumpRegisters();
                throw PlipEmulationException(ex.str().c_str());
            }

            *src = MEM_READ(addr);
        }
        NUM_MCYCLES(3);
    }

    // LD r, n
    // LD (HL), n
    void SharpLr35902::OpLdRegImm() {
        auto dest = OP_REG_Y(0);

        if(dest == IDX_HL) {
            // LD (HL), n
            if(m_mcycle == 2) {
                FETCH;
            } else if(m_mcycle == 3) {
                MEM_WRITE(REG_HL, *(GetRegister8(OP_REG_Y(0))));
            }
            NUM_MCYCLES(3);
            return;
        }

        // LD r, n
        if(m_mcycle == 2) {
            FETCH;
        } else if(m_mcycle == 3) {
            *(GetRegister8(OP_REG_Y(0))) = m_instr[1];
        }
        NUM_MCYCLES(4);
    }

    // LD r, (HL)
    // LD A, (rr)
    void SharpLr35902::OpLdRegMem() {
        uint16_t addr;
        uint8_t *dest;

        if(m_mcycle == 2) {
            if(OP_MASK(0b11000111, 0b01000110)) {
                // LD r, (HL)
                addr = REG_HL;
                dest = GetRegister8(OP_REG_X(0));
            } else if(OP_MASK(0b11001111, 0b00001010)) {
                // LD A, (rr)
                addr = GetAddress(OP_REG_16(0));
                dest = &(m_reg.a);
            } else {
                std::stringstream ex;
                ex << "OpLdRegMem: invalid operation\n\n" << DumpRegisters();
                throw PlipEmulationException(ex.str().c_str());
            }

            MEM_WRITE(addr, *dest);
        }
        NUM_MCYCLES(3);
    }

    // (!) LD (HL), (HL)
    // LD r, r'
    // LD r, (HL) -> OpLdRegMem()
    // LD (HL), r
    void SharpLr35902::OpLdRegReg() {
        auto dest = OP_REG_X(0);
        auto src = OP_REG_Y(0);

        if(dest == IDX_HL && src == IDX_HL) {
            // (!) LD (HL), (HL)
            std::stringstream ex;
            ex << "illegal instruction: LD (HL), (HL)\n\n" << DumpRegisters();
            throw PlipEmulationException(ex.str().c_str());
        }

        if(dest == IDX_HL) {
            // LD (HL), r
            if(m_mcycle == 2) {
                MEM_WRITE(REG_HL, *(GetRegister8(src)));
            }

            NUM_MCYCLES(3);
        } else if(src == IDX_HL) {
            // LD r, (HL)
            OpLdRegMem();
        } else {
            // LD r, r'
            *(GetRegister8(dest)) = *(GetRegister8(src));
            NUM_MCYCLES(2);
        }
    }

    // OR r
    // OR (HL)
    void SharpLr35902::OpOr() {
        auto src = OP_REG_Y(0);

        if(src == IDX_HL) {
            // OR (HL)
            if(m_mcycle == 2) {
                FETCH_ADDR(REG_HL);
            } else {
                m_reg.a |= m_instr[1];
                CHECK_ZERO(m_reg.a);
                FLAG_CLEAR(SUBTRACT);
                FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
            }
            NUM_MCYCLES(3);
            return;
        }

        // OP r
        m_reg.a |= *(GetRegister8(src));
        CHECK_ZERO(m_reg.a);
        FLAG_CLEAR(SUBTRACT);
        FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);

        NUM_MCYCLES(2);
    }

    // SUB r
    // SUB (HL)
    void SharpLr35902::OpSub() {
        auto src = OP_REG_Y(0);
        uint16_t res;

        if(src == IDX_HL) {
            // SUB A, (HL)
            if(m_mcycle == 2) {
                FETCH_ADDR(REG_HL);
            } else {
                res = m_reg.a - m_instr[1];
                CHECK_CARRY(res);
                CHECK_HALFCARRY(m_reg.a, res);
                m_reg.a = res & 0xFF;
                CHECK_ZERO(m_reg.a);
                FLAG_SET(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // SUB A, r
        res = m_reg.a - *(GetRegister8(src));
        CHECK_CARRY(res);
        CHECK_HALFCARRY(m_reg.a, res);
        m_reg.a = res & 0xFF;
        CHECK_ZERO(m_reg.a);
        FLAG_SET(SUBTRACT);

        NUM_MCYCLES(2);
    }

    // SBC r
    // SBC (HL)
    void SharpLr35902::OpSubBorrow() {
        auto src = OP_REG_Y(0);
        uint16_t res;

        if(src == IDX_HL) {
            // SBC A, (HL)
            if(m_mcycle == 2) {
                FETCH_ADDR(REG_HL);
            } else {
                res = m_reg.a - m_instr[1] - FLAG_TEST(CARRY) ? 1 : 0;
                CHECK_CARRY(res);
                CHECK_HALFCARRY(m_reg.a, res);
                m_reg.a = res & 0xFF;
                CHECK_ZERO(m_reg.a);
                FLAG_SET(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // SBC A, r
        res = m_reg.a - *(GetRegister8(src)) - FLAG_TEST(CARRY) ? 1 : 0;
        CHECK_CARRY(res);
        CHECK_HALFCARRY(m_reg.a, res);
        m_reg.a = res & 0xFF;
        CHECK_ZERO(m_reg.a);
        FLAG_SET(SUBTRACT);

        NUM_MCYCLES(2);
    }

    // XOR r
    // XOR (HL)
    void SharpLr35902::OpXor() {
        auto src = OP_REG_Y(0);

        if(src == IDX_HL) {
            // XOR (HL)
            if(m_mcycle == 2) {
                FETCH_ADDR(REG_HL);
            } else {
                m_reg.a ^= m_instr[1];
                CHECK_ZERO(m_reg.a);
                FLAG_CLEAR(SUBTRACT);
                FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
            }
            NUM_MCYCLES(3);
            return;
        }

        // XOR r
        m_reg.a ^= *(GetRegister8(src));
        CHECK_ZERO(m_reg.a);
        FLAG_CLEAR(SUBTRACT);
        FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);

        NUM_MCYCLES(2);
    }

    /*
     * CB-prefixed opcodes: starts at m_mcycle == 3
     */
    // RES n, r
    // RES n, (HL)
    void SharpLr35902::OpBitClear() {
        auto idx = OP_REG_X(1);
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // RES n, (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else {
                MEM_WRITE(REG_HL, m_instr[2] &= (1 << idx));
            }
            NUM_MCYCLES(5);
            return;
        }

        // RES n, r
        *(GetRegister8(reg)) &= (1 << idx);
        NUM_MCYCLES(3);
    }

    // SET n, r
    // SET n, (HL)
    void SharpLr35902::OpBitSet() {
        auto idx = OP_REG_X(1);
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // SET n, (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else {
                MEM_WRITE(REG_HL, m_instr[2] |= (1 << idx));
            }
            NUM_MCYCLES(5);
            return;
        }

        // SET n, r
        *(GetRegister8(reg)) |= (1 << idx);
        NUM_MCYCLES(3);
    }

    // BIT n, r
    // BIT n, (HL)
    void SharpLr35902::OpBitTest() {
        auto idx = OP_REG_X(1);
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // BIT n, (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else {
                FLAG_CLEAR(SUBTRACT);
                FLAG_SET(HALFCARRY);
                CHECK_ZERO(m_instr[2] & (1 << idx));
            }
            NUM_MCYCLES(5);
            return;
        }

        // BIT n, r
        FLAG_CLEAR(SUBTRACT);
        FLAG_SET(HALFCARRY);
        CHECK_ZERO(*(GetRegister8(reg)) & (1 << idx));

        NUM_MCYCLES(3);
    }

    // SWAP n
    // SWAP (HL)
    void SharpLr35902::OpNibbleSwap() {
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // SWAP (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else {
                uint8_t res = (m_instr[2] << 4) + (m_instr[2] & 0x0F);
                MEM_WRITE(REG_HL, res);
                CHECK_ZERO(res);
                FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
                FLAG_CLEAR(SUBTRACT);
            }

            NUM_MCYCLES(5);
            return;
        }

        // SWAP r
        auto r = GetRegister8(reg);
        *r = (*r << 4) + (*r & 0x0F);
        CHECK_ZERO(*r);
        FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);

        NUM_MCYCLES(3);
    }

    // RLC r
    // RLC (HL)
    void SharpLr35902::OpRotateLeft() {
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // RLC (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else if(m_mcycle == 4) {
                auto res = m_instr[2];
                uint8_t msb = res >> 7;
                if(msb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
                FLAG_CLEAR(SUBTRACT);
                res <<= 1;
                res |= msb;
                CHECK_ZERO(res);
                MEM_WRITE(REG_HL, res);
            }

            NUM_MCYCLES(5);
            return;
        }

        // RLC r
        auto r = GetRegister8(reg);
        uint8_t msb = *r >> 7;
        if(msb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        *r <<= 1;
        *r |= msb;
        CHECK_ZERO(*r);

        NUM_MCYCLES(3);
    }

    // RL r
    // RL (HL)
    void SharpLr35902::OpRotateLeftThruCarry() {
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // RL (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else {
                auto res = m_instr[2];
                uint8_t cf = FLAG_TEST(CARRY) ? 1 : 0;
                if(res & 0b10000000) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
                FLAG_CLEAR(SUBTRACT);
                res <<= 1;
                res |= cf;
                CHECK_ZERO(res);
            }

            NUM_MCYCLES(5);
            return;
        }

        // RL r
        auto r = GetRegister8(reg);
        uint8_t cf = FLAG_TEST(CARRY) ? 1 : 0;
        if(*r & 0b10000000) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        *r <<= 1;
        *r |= cf;
        CHECK_ZERO(*r);

        NUM_MCYCLES(3);
    }

    // RRC r
    // RRC (HL)
    void SharpLr35902::OpRotateRight() {
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // RRC (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else if(m_mcycle == 4) {
                auto res = m_instr[2];
                uint8_t lsb = res & 0b00000001;
                if(lsb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
                FLAG_CLEAR(SUBTRACT);
                res >>= 1;
                res |= (lsb << 7);
                CHECK_ZERO(res);
                MEM_WRITE(REG_HL, res);
            }

            NUM_MCYCLES(5);
            return;
        }

        // RRC r
        auto r = GetRegister8(reg);
        uint8_t lsb = *r & 0b00000001;
        if(lsb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        *r >>= 1;
        *r |= (lsb << 7);
        CHECK_ZERO(*r);

        NUM_MCYCLES(3);
    }

    // RR r
    // RR (HL)
    void SharpLr35902::OpRotateRightThruCarry() {
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // RR (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else {
                auto res = m_instr[2];
                uint8_t cf = FLAG_TEST(CARRY) ? 0b10000000 : 0;
                if(res & 0b00000001) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
                FLAG_CLEAR(SUBTRACT);
                res >>= 1;
                res |= cf;
                CHECK_ZERO(res);
            }

            NUM_MCYCLES(5);
            return;
        }

        // RR r
        auto r = GetRegister8(reg);
        uint8_t cf = FLAG_TEST(CARRY) ? 0b10000000 : 0;
        if(*r & 0b00000001) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        *r >>= 1;
        *r |= cf;
        CHECK_ZERO(*r);

        NUM_MCYCLES(3);
    }

    // SLA r
    // SLA (HL)
    void SharpLr35902::OpShiftLeftArithmetic() {
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // SLA (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else if(m_mcycle == 4) {
                auto res = m_instr[2];
                uint8_t msb = res & 0b10000000;
                if(msb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
                FLAG_CLEAR(SUBTRACT);
                res <<= 1;
                CHECK_ZERO(res);
                MEM_WRITE(REG_HL, res);
            }

            NUM_MCYCLES(5);
            return;
        }

        // SLA r
        auto r = GetRegister8(reg);
        uint8_t msb = *r & 0b10000000;
        if(msb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        *r <<= 1;
        CHECK_ZERO(*r);

        NUM_MCYCLES(3);
    }

    // SRA r
    // SRA (HL)
    void SharpLr35902::OpShiftRightArithmetic() {
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // SRA (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else if(m_mcycle == 4) {
                auto res = m_instr[2];
                uint8_t msb = reg & 0b10000000;
                uint8_t lsb = res & 0b00000001;
                if(lsb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
                FLAG_CLEAR(SUBTRACT);
                res >>= 1;
                res |= msb;
                CHECK_ZERO(res);
                MEM_WRITE(REG_HL, res);
            }

            NUM_MCYCLES(5);
            return;
        }

        // SRA r
        auto r = GetRegister8(reg);
        uint8_t msb = *r & 0b10000000;
        uint8_t lsb = *r & 0b00000001;
        if(lsb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        *r >>= 1;
        *r |= msb;
        CHECK_ZERO(*r);

        NUM_MCYCLES(3);
    }

    // SRL r
    // SRL (HL)
    void SharpLr35902::OpShiftRightLogical() {
        auto reg = OP_REG_Y(1);

        if(reg == IDX_HL) {
            // SRL (HL)
            if(m_mcycle == 3) {
                FETCH_ADDR(REG_HL);
            } else if(m_mcycle == 4) {
                auto res = m_instr[2];
                uint8_t lsb = res & 0b00000001;
                if(lsb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
                FLAG_CLEAR(HALFCARRY);
                FLAG_CLEAR(SUBTRACT);
                res >>= 1;
                CHECK_ZERO(res);
                MEM_WRITE(REG_HL, res);
            }

            NUM_MCYCLES(5);
            return;
        }

        // SRL r
        auto r = GetRegister8(reg);
        uint8_t lsb = *r & 0b00000001;
        if(lsb) FLAG_SET(CARRY); else FLAG_CLEAR(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        *r >>= 1;
        CHECK_ZERO(*r);

        NUM_MCYCLES(3);
    }
}
