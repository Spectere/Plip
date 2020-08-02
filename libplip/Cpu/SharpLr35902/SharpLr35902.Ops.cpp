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
    // ADD A, n
    void SharpLr35902::OpAccumAddImm() {
        FETCH_IMM_CYCLE(2);
        CYCLE(3) {
            uint16_t res = m_reg.a + m_instr[1];
            CHECK_CARRY(res);
            CHECK_ADD_HALFCARRY(m_reg.a, m_instr[1]);
            m_reg.a = res & 0xFF;
            CHECK_ZERO(m_reg.a);
            FLAG_CLEAR(SUBTRACT);
        }
        NUM_MCYCLES(3);
    }

    // ADC A, n
    void SharpLr35902::OpAccumAddCarryImm() {
        FETCH_IMM_CYCLE(2);
        CYCLE(3) {
            uint16_t res = m_reg.a + m_instr[1] + (FLAG_TEST(CARRY) ? 1 : 0);
            CHECK_CARRY(res);
            CHECK_ADD_HALFCARRY(m_reg.a, m_instr[1]);
            m_reg.a = res & 0xFF;
            CHECK_ZERO(m_reg.a);
            FLAG_CLEAR(SUBTRACT);
        }
        NUM_MCYCLES(3);
    }

    // AND n
    void SharpLr35902::OpAccumAndImm() {
        FETCH_IMM_CYCLE(2);
        CYCLE(3) {
            m_reg.a &= m_instr[1];
            CHECK_ZERO(m_reg.a);
            FLAG_SET(HALFCARRY);
            FLAG_CLEAR(CARRY);
            FLAG_CLEAR(SUBTRACT);
        }
        NUM_MCYCLES(3);
    }

    // DAA
    void SharpLr35902::OpAccumBcd() {
        // DAAaaaaaaaaaaaaaaaahhhhhhh!!!!
        // Source: https://www.reddit.com/r/EmuDev/comments/4ycoix/a_guide_to_the_gameboys_halfcarry_flag/d6p3rtl/
        uint8_t adjust = 0x00;

        if(FLAG_TEST(CARRY) || (!FLAG_TEST(SUBTRACT) && m_reg.a > 0x99)) {
            adjust |= 0x60;
            FLAG_SET(CARRY);
        }

        if(FLAG_TEST(HALFCARRY) || (!FLAG_TEST(SUBTRACT) && (m_reg.a & 0x0F) > 0x09)) {
            adjust |= 0x06;
        }

        m_reg.a += FLAG_TEST(SUBTRACT) ? -adjust : adjust;
        CHECK_ZERO(m_reg.a);
        FLAG_CLEAR(HALFCARRY);

        NUM_MCYCLES(2);
    }

    // CP n
    void SharpLr35902::OpAccumCarryImm() {
        FETCH_IMM_CYCLE(2);
        CYCLE(3) {
            uint16_t res = m_reg.a - m_instr[1];
            CHECK_CARRY(res);
            CHECK_SUB_HALFCARRY(m_reg.a, m_instr[1]);
            CHECK_ZERO(res & 0xFF);
            FLAG_SET(SUBTRACT);
        }
        NUM_MCYCLES(3);
    }

    // CPL
    void SharpLr35902::OpAccumFlip() {
        m_reg.a = ~(m_reg.a);
        FLAG_SET(SUBTRACT);
        FLAG_SET(HALFCARRY);
        NUM_MCYCLES(2);
    }

    // OR n
    void SharpLr35902::OpAccumOrImm() {
        FETCH_IMM_CYCLE(2);
        CYCLE(3) {
            m_reg.a |= m_instr[1];
            CHECK_ZERO(m_reg.a);
            FLAG_CLEAR(HALFCARRY);
            FLAG_CLEAR(CARRY);
            FLAG_CLEAR(SUBTRACT);
        }
        NUM_MCYCLES(3);
    }

    // RLCA
    void SharpLr35902::OpAccumRotateLeft() {
        CHECK_BIT_CARRY(m_reg.a & 0b10000000);
        m_reg.a <<= 1;
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        FLAG_CLEAR(ZERO);

        NUM_MCYCLES(2);
    }

    // RLA
    void SharpLr35902::OpAccumRotateLeftThruCarry() {
        uint8_t cf = FLAG_TEST(CARRY) ? 1 : 0;
        CHECK_BIT_CARRY(m_reg.a & 0b10000000);
        m_reg.a <<=1;
        m_reg.a |= cf;
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        FLAG_CLEAR(ZERO);

        NUM_MCYCLES(2);
    }

    // RRCA
    void SharpLr35902::OpAccumRotateRight() {
        CHECK_BIT_CARRY(m_reg.a & 0b00000001);
        m_reg.a >>= 1;
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        FLAG_CLEAR(ZERO);

        NUM_MCYCLES(2);
    }

    // RRA
    void SharpLr35902::OpAccumRotateRightThruCarry() {
        uint8_t cf = FLAG_TEST(CARRY) ? 0b10000000 : 0;
        CHECK_BIT_CARRY(m_reg.a & 0b00000001);
        m_reg.a >>= 1;
        m_reg.a |= cf;
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        FLAG_CLEAR(ZERO);

        NUM_MCYCLES(2);
    }

    // SUB A, n
    void SharpLr35902::OpAccumSubImm() {
        FETCH_IMM_CYCLE(2);
        CYCLE(3) {
            uint16_t res = m_reg.a - m_instr[1];
            CHECK_CARRY(res);
            CHECK_SUB_HALFCARRY(m_reg.a, m_instr[1]);
            m_reg.a = res & 0xFF;
            CHECK_ZERO(m_reg.a);
            FLAG_SET(SUBTRACT);
        }
        NUM_MCYCLES(3);
    }

    // SBC A, n
    void SharpLr35902::OpAccumSubBorrowImm() {
        FETCH_IMM_CYCLE(2);
        CYCLE(3) {
            uint16_t res = m_reg.a - m_instr[1] - (FLAG_TEST(CARRY) ? 1 : 0);
            CHECK_CARRY(res);
            CHECK_SUB_HALFCARRY(m_reg.a, m_instr[1]);
            m_reg.a = res & 0xFF;
            CHECK_ZERO(m_reg.a);
            FLAG_SET(SUBTRACT);
        }
        NUM_MCYCLES(3);
    }

    // XOR n
    void SharpLr35902::OpAccumXorImm() {
        FETCH_IMM_CYCLE(2);
        CYCLE(3) {
            m_reg.a ^= m_instr[1];
            CHECK_ZERO(m_reg.a);
            FLAG_CLEAR(HALFCARRY);
            FLAG_CLEAR(CARRY);
            FLAG_CLEAR(SUBTRACT);
        }
        NUM_MCYCLES(3);
    }

    // ADD A, r
    // ADD A, (HL)
    void SharpLr35902::OpAdd() {
        auto src = OP_REG_Y(0);
        uint16_t res;

        if(src == IDX_HL) {
            // ADD A, (HL)
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
                res = m_reg.a + m_instr[1];
                CHECK_CARRY(res);
                CHECK_ADD_HALFCARRY(m_reg.a, m_instr[1]);
                m_reg.a = res & 0xFF;
                CHECK_ZERO(m_reg.a);
                FLAG_CLEAR(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // ADD A, r
        auto val = *(GetRegister8(src));
        res = m_reg.a + val;
        CHECK_CARRY(res);
        CHECK_ADD_HALFCARRY(m_reg.a, val);
        m_reg.a = res & 0xFF;
        CHECK_ZERO(m_reg.a);
        FLAG_CLEAR(SUBTRACT);

        NUM_MCYCLES(2);
    }

    // ADD HL, rr
    void SharpLr35902::OpAdd16() {
        CYCLE(3) {
            auto hl = GetRegister16Value(IDX_16_HL);
            auto val = GetRegister16Value(OP_REG16(0));

            uint32_t res = hl + val;
            CHECK_CARRY16(res);
            CHECK_ADD_HALFCARRY16(hl, val);
            FLAG_CLEAR(SUBTRACT);

            m_reg.h = res >> 8;
            m_reg.l = res & 0xFF;
        }

        NUM_MCYCLES(3);
    }

    // ADC A, r
    // ADC A, (HL)
    void SharpLr35902::OpAddCarry() {
        auto src = OP_REG_Y(0);
        uint16_t res;

        if(src == IDX_HL) {
            // ADC A, (HL)
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
                res = m_reg.a + m_instr[1] + (FLAG_TEST(CARRY) ? 1 : 0);
                CHECK_CARRY(res);
                CHECK_ADD_HALFCARRY(m_reg.a, m_instr[1]);
                m_reg.a = res & 0xFF;
                CHECK_ZERO(m_reg.a);
                FLAG_CLEAR(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // ADC A, r
        auto val = *(GetRegister8(src));
        res = m_reg.a + val + FLAG_TEST(CARRY) ? 1 : 0;
        CHECK_CARRY(res);
        CHECK_ADD_HALFCARRY(m_reg.a, val);
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
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
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

    // CALL cc, nn
    void SharpLr35902::OpCallCond() {
        FETCH_CYCLE(2);
        FETCH_CYCLE(3);

        if(!TestConditional(OP_COND)) {
            NUM_MCYCLES(4);
            return;
        }

        STACK_PUSH_PC(5);
        CYCLE(7) { m_reg.pc = (m_instr[2] << 8) | m_instr[1]; }
        NUM_MCYCLES(7);
    }

    // CALL nn
    void SharpLr35902::OpCallUnc() {
        FETCH_CYCLE(2);
        FETCH_CYCLE(3);
        STACK_PUSH_PC(5);
        CYCLE(7) { m_reg.pc = (m_instr[2] << 8) | m_instr[1]; }
        NUM_MCYCLES(7);
    }

    // CP r
    // CP (HL)
    void SharpLr35902::OpCarry() {
        auto src = OP_REG_Y(0);
        uint16_t res;

        if(src == IDX_HL) {
            // CP (HL)
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
                res = m_reg.a - m_instr[1];
                CHECK_CARRY(res);
                CHECK_SUB_HALFCARRY(m_reg.a, m_instr[1]);
                CHECK_ZERO(res & 0xFF);
                FLAG_SET(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // CP r
        auto val = *(GetRegister8(src));
        res = m_reg.a - val;
        CHECK_CARRY(res);
        CHECK_SUB_HALFCARRY(m_reg.a, val);
        CHECK_ZERO(res & 0xFF);
        FLAG_SET(SUBTRACT);

        NUM_MCYCLES(2);
    }

    // DEC r
    // DEC (HL)
    void SharpLr35902::OpDecReg() {
        auto dest = OP_REG_X(0);
        uint16_t res;

        if(dest == IDX_HL) {
            // DEC (HL)
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
                res = m_instr[1] - 1;

                FLAG_SET(SUBTRACT);
                CHECK_SUB_HALFCARRY(m_instr[1], 1);
                CHECK_ZERO(m_instr[1]);

                MEM_WRITE(REG_HL, res);
            }
            NUM_MCYCLES(4);
            return;
        }

        // DEC r
        auto reg = GetRegister8(dest);
        res = *reg - 1;

        FLAG_SET(SUBTRACT);
        CHECK_SUB_HALFCARRY(*reg, 1);
        CHECK_ZERO(*reg);
        *reg = res;

        NUM_MCYCLES(2);
    }

    // DEC rr
    void SharpLr35902::OpDecPair() {
        uint8_t *high = nullptr;
        uint8_t *low = nullptr;

        std::tie(high, low) = GetRegisterPair(OP_REG16(0));
        DecPair(high, low);
        NUM_MCYCLES(2);
    }

    // DI
    void SharpLr35902::OpDisableInterrupts() {
        m_ime = Disabled;
        NUM_MCYCLES(2);
    }

    // EI
    void SharpLr35902::OpEnableInterrupts() {
        m_ime = Scheduled;
        NUM_MCYCLES(2);
    }

    // CCF
    void SharpLr35902::OpFlipCarry() {
        FLAG_FLIP(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        NUM_MCYCLES(2);
    }

    // RST [00h, 08h, 10h, 18h, 20h, 28h, 30h, 38h]
    void SharpLr35902::OpFuncFixedUnc() {
        CYCLE(3) { STACK_PUSH(m_reg.pc >> 8); }
        CYCLE(4) { STACK_PUSH(m_reg.pc & 0xFF); }
        CYCLE(5) { m_reg.pc = OP_IDX(0) * 0x08; }
        NUM_MCYCLES(5);
    }

    // INC r
    // INC (HL)
    void SharpLr35902::OpIncReg() {
        auto dest = OP_REG_X(0);
        uint16_t res;

        if(dest == IDX_HL) {
            // INC (HL)
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
                res = m_instr[1] + 1;

                FLAG_CLEAR(SUBTRACT);
                CHECK_ADD_HALFCARRY(m_instr[1], 1);
                CHECK_ZERO(m_instr[1]);

                MEM_WRITE(REG_HL, res);
            }
            NUM_MCYCLES(4);
            return;
        }

        // INC r
        auto reg = GetRegister8(dest);
        res = *reg + 1;

        FLAG_CLEAR(SUBTRACT);
        CHECK_ADD_HALFCARRY(*reg, 1);
        CHECK_ZERO(*reg);
        *reg = res;

        NUM_MCYCLES(2);
    }

    // INC rr
    void SharpLr35902::OpIncPair() {
        uint8_t *high = nullptr;
        uint8_t *low = nullptr;

        std::tie(high, low) = GetRegisterPair(OP_REG16(0));
        IncPair(high, low);
        NUM_MCYCLES(2);
    }

    // JP cc, nn
    void SharpLr35902::OpJumpAbsCond() {
        FETCH_CYCLE(2);
        FETCH_CYCLE(3);

        if(!TestConditional(OP_COND)) {
            NUM_MCYCLES(4);
            return;
        }

        CYCLE(5) { SET_PC_IMM; }
        NUM_MCYCLES(5);
    }

    // JP nn
    void SharpLr35902::OpJumpAbsUnc() {
        FETCH_CYCLE(2);
        FETCH_CYCLE(3);
        CYCLE(5) { SET_PC_IMM; }
        NUM_MCYCLES(5);
    }

    // JP HL
    void SharpLr35902::OpJumpRegUnc() {
        m_reg.pc = GetRegister16Value(IDX_16_HL);
        NUM_MCYCLES(2);
    }

    // JR cc, n
    void SharpLr35902::OpJumpRelCond() {
        FETCH_CYCLE(2);

        if(TestConditional(OP_COND)) {
            NUM_MCYCLES(3);
            return;
        }

        CYCLE(4) { m_reg.pc += (int8_t)m_instr[1]; }
        NUM_MCYCLES(4);
    }

    // JR n
    void SharpLr35902::OpJumpRelUnc() {
        FETCH_CYCLE(2);
        CYCLE(4) { m_reg.pc += (int8_t)m_instr[1]; }
        NUM_MCYCLES(4);
    }

    // LD (HL), r
    // LD (rr), A
    void SharpLr35902::OpLdMemReg() {
        auto dest = OP_REG_X(0);
        auto src = OP_REG_Y(0);

        if(dest == REG_HL) {
            // LD (HL), r
            CYCLE(2) {
                auto reg = *GetRegister8(src);
                MEM_WRITE(REG_HL, reg);
            }
        } else {
            // LD (rr), A
            CYCLE(2) {
                auto reg = GetRegister16Value(OP_REG16(0));
                MEM_WRITE(reg, m_reg.a);
            }
        }
        NUM_MCYCLES(3);
    }

    // LD rr, nn
    void SharpLr35902::OpLdReg16Imm16() {
        FETCH_IMM_CYCLE(2);
        FETCH_IMM_CYCLE(3);
        CYCLE(4) {
            auto [high, low] = GetRegisterPair(OP_REG16(0));
            *low = m_instr[1];
            *high = m_instr[2];
        }

        NUM_MCYCLES(4);
    }

    // LD r, n
    // LD (HL), n
    void SharpLr35902::OpLdRegImm() {
        auto dest = OP_REG_Y(0);

        if(dest == IDX_HL) {
            // LD (HL), n
            FETCH_IMM_CYCLE(2);
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
                MEM_WRITE(REG_HL, m_instr[1]);
            }
            NUM_MCYCLES(4);
            return;
        }

        // LD r, n
        FETCH_IMM_CYCLE(2);
        CYCLE(3) {
            *(GetRegister8(OP_REG_Y(0))) = m_instr[1];
        }
        NUM_MCYCLES(3);
    }

    // LD A, (rr)
    void SharpLr35902::OpLdRegMem() {
        FETCH_ADDR_CYCLE(2, GetRegister16Value(OP_REG16(0)));
        CYCLE(3) {
            m_reg.a = m_instr[1];
        }
        NUM_MCYCLES(3);
    }

    // [!] LD (HL), (HL)
    // LD r, r'
    // LD r, (HL)
    // LD (HL), r
    void SharpLr35902::OpLdRegReg() {
        auto dest = OP_REG_X(0);
        auto src = OP_REG_Y(0);

        if(dest == IDX_HL && src == IDX_HL) {
            // [!] LD (HL), (HL)
            std::stringstream ex;
            ex << "illegal instruction: LD (HL), (HL)\n\n" << DumpRegisters();
            throw PlipEmulationException(ex.str().c_str());
        }

        if(dest == IDX_HL) {
            // LD (HL), r
            CYCLE(2) {
                MEM_WRITE(REG_HL, *(GetRegister8(src)));
            }
            NUM_MCYCLES(3);
        } else if(src == IDX_HL) {
            // LD r, (HL)
            FETCH_ADDR_CYCLE(2, IDX_HL);
            CYCLE(3) {
                *(GetRegister8(dest)) = m_instr[1];
            }
            NUM_MCYCLES(3);
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
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
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

    // POP rr
    void SharpLr35902::OpPopReg16() {
        auto reg = OP_REG16(0);
        uint8_t *high, *low;

        if(reg == IDX_16_SP) {
            // AF shares an index with SP for PUSH/POP.
            high = &(m_reg.a);
            low = &(m_reg.f);
        } else {
            std::tie(high, low) = GetRegisterPair(reg);
        }

        CYCLE(2) { *low = STACK_POP; }
        CYCLE(3) { *high = STACK_POP; }
        NUM_MCYCLES(4);
    }

    // PUSH rr
    void SharpLr35902::OpPushReg16() {
        auto reg = OP_REG16(0);
        uint16_t val;

        if(reg == IDX_16_SP) {
            // AF shares an index with SP for PUSH/POP.
            val = (m_reg.a << 8) + m_reg.f;
        } else {
            GetRegister16Value(OP_REG16(0));
        }

        CYCLE(3) { STACK_PUSH(val >> 8); }
        CYCLE(4) { STACK_PUSH(val & 0xFF); }
        NUM_MCYCLES(5);
    }

    // RET
    void SharpLr35902::OpRetUnc() {
        SET_PC_STACK(2);
        NUM_MCYCLES(5);
    }

    // RET cc
    void SharpLr35902::OpRetCond() {
        if(!TestConditional(OP_COND)) {
            NUM_MCYCLES(3);
            return;
        }

        SET_PC_STACK(3);
        NUM_MCYCLES(6);
    }

    // RETI
    void SharpLr35902::OpRetImeUnc() {
        SET_PC_STACK(2);
        CYCLE(4) { m_ime = Enabled; }
        NUM_MCYCLES(5);
    }

    // SCF
    void SharpLr35902::OpSetCarry() {
        FLAG_SET(CARRY);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        NUM_MCYCLES(2);
    }

    // SUB r
    // SUB (HL)
    void SharpLr35902::OpSub() {
        auto src = OP_REG_Y(0);
        uint16_t res;

        if(src == IDX_HL) {
            // SUB A, (HL)
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
                res = m_reg.a - m_instr[1];
                CHECK_CARRY(res);
                CHECK_SUB_HALFCARRY(m_reg.a, m_instr[1]);
                m_reg.a = res & 0xFF;
                CHECK_ZERO(m_reg.a);
                FLAG_SET(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // SUB A, r
        auto val = *(GetRegister8(src));
        res = m_reg.a - val;
        CHECK_CARRY(res);
        CHECK_SUB_HALFCARRY(m_reg.a, val);
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
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
                res = m_reg.a - m_instr[1] - (FLAG_TEST(CARRY) ? 1 : 0);
                CHECK_CARRY(res);
                CHECK_SUB_HALFCARRY(m_reg.a, m_instr[1]);
                m_reg.a = res & 0xFF;
                CHECK_ZERO(m_reg.a);
                FLAG_SET(SUBTRACT);
            }
            NUM_MCYCLES(3);
            return;
        }

        // SBC A, r
        auto val = *(GetRegister8(src));
        res = m_reg.a - val - FLAG_TEST(CARRY) ? 1 : 0;
        CHECK_CARRY(res);
        CHECK_SUB_HALFCARRY(m_reg.a, val);
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
            FETCH_ADDR_CYCLE(2, REG_HL);
            CYCLE(3) {
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
                auto res = m_instr[2];
                uint8_t msb = res >> 7;
                CHECK_BIT_CARRY(msb);
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
        CHECK_BIT_CARRY(msb);
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
                auto res = m_instr[2];
                uint8_t cf = FLAG_TEST(CARRY) ? 1 : 0;
                CHECK_BIT_CARRY(res & 0b10000000);
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
        CHECK_BIT_CARRY(*r & 0b10000000);
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
                auto res = m_instr[2];
                uint8_t lsb = res & 0b00000001;
                CHECK_BIT_CARRY(lsb);
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
        CHECK_BIT_CARRY(lsb);
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
                auto res = m_instr[2];
                uint8_t cf = FLAG_TEST(CARRY) ? 0b10000000 : 0;
                CHECK_BIT_CARRY(res & 0b00000001);
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
        CHECK_BIT_CARRY(*r & 0b00000001);
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
                auto res = m_instr[2];
                uint8_t msb = res & 0b10000000;
                CHECK_BIT_CARRY(msb);
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
        CHECK_BIT_CARRY(msb);
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
                auto res = m_instr[2];
                uint8_t msb = reg & 0b10000000;
                uint8_t lsb = res & 0b00000001;
                CHECK_BIT_CARRY(lsb);
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
        CHECK_BIT_CARRY(lsb);
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
            FETCH_ADDR_CYCLE(3, REG_HL);
            CYCLE(4) {
                auto res = m_instr[2];
                uint8_t lsb = res & 0b00000001;
                CHECK_BIT_CARRY(lsb);
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
        CHECK_BIT_CARRY(lsb);
        FLAG_CLEAR(HALFCARRY);
        FLAG_CLEAR(SUBTRACT);
        *r >>= 1;
        CHECK_ZERO(*r);

        NUM_MCYCLES(3);
    }
}
