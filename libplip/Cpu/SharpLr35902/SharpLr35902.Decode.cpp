/* SharpLr35902.Decode.cpp
 *
 * Sharp LR35902 instruction decoding.
 */

#include <sstream>

#include "../../PlipEmulationException.h"
#include "../../PlipUtility.h"

#include "SharpLr35902.h"
#include "SharpLr35902.Macros.h"

namespace Plip::Cpu {
    void SharpLr35902::Decode() {
        if(OP(0b00000000)) {
            // NOP
            NUM_MCYCLES(2);
        } else if(OP(0b11001011)) {
            // 0xCB
            if(m_mcycle == 2)
                FETCH;
            else
                DecodeCB();
        } else if(OP(0b00000111)) {
            // RLCA
            OpAccumRotateLeft();
        } else if(OP(0b00001111)) {
            // RRCA
            OpAccumRotateRight();
        } else if(OP(0b00011000)) {
            // JR n
            OpJumpRelUnc();
        } else if(OP(0b00010111)) {
            // RLA
            OpAccumRotateLeftThruCarry();
        } else if(OP(0b00011111)) {
            // RRA
            OpAccumRotateRightThruCarry();
        } else if(OP(0b00100111)) {
            // DAA
            OpAccumBcd();
        } else if(OP(0b00101111)) {
            // CPL
            OpAccumFlip();
        } else if(OP(0b00110111)) {
            // SCF
            OpSetCarry();
        } else if(OP(0b00111111)) {
            // CCF
            OpFlipCarry();
        } else if(OP(0b11000110)) {
            // ADD A, n
            OpAccumAddImm();
        } else if(OP(0b11001001)) {
            // RET
            OpRetUnc();
        } else if(OP(0b11000011)) {
            // JP nn
            OpJumpAbsUnc();
        } else if(OP(0b11001101)) {
            // CALL nn
            OpCallUnc();
        } else if(OP(0b11001110)) {
            // ADC A, n
            OpAccumAddCarryImm();
        } else if(OP(0b11011001)) {
            // RETI
            OpRetImeUnc();
        } else if(OP(0b11010110)) {
            // SUB A, n
            OpAccumSubImm();
        } else if(OP(0b11011110)) {
            // SBC A, n
            OpAccumSubBorrowImm();
        } else if(OP(0b11100110)) {
            // AND n
            OpAccumAndImm();
        } else if(OP(0b11101001)) {
            // JP HP
            OpJumpRegUnc();
        } else if(OP(0b11101110)) {
            // XOR n
            OpAccumXorImm();
        } else if(OP(0b11110110)) {
            // OR n
            OpAccumOrImm();
        } else if(OP(0b11111110)) {
            // CP n
            OpAccumCarryImm();
        } else if(OP(0b11110011)) {
            // DI
            OpDisableInterrupts();
        } else if(OP(0b11111011)) {
            // EI
            OpEnableInterrupts();
        } else if(OP_MASK(0b11000111, 0b00000100)) {
            // INC r
            OpIncReg();
        } else if(OP_MASK(0b11000111, 0b00000101)) {
            // DEC r
            OpDecReg();
        } else if(OP_MASK(0b11001111, 0b00000011)) {
            // INC rr
            OpIncPair();
        } else if(OP_MASK(0b11001111, 0b00001011)) {
            // DEC rr
            OpDecPair();
        } else if(OP_MASK(0b11000000, 0b01000000)) {
            // LD r, r' / LD r, (HL) / LD (HL), r
            OpLdRegReg();
        } else if(OP_MASK(0b11000111, 0b00000110)) {
            // LD r, n
            OpLdRegImm();
        } else if(OP_MASK(0b11001111, 0b00001010)) {
            // LD A, (rr)
            OpLdRegMem();
        } else if(OP_MASK(0b11001111, 0b00000010)) {
            // LD (rr), A
            OpLdMemReg();
        } else if(OP_MASK(0b11001111, 0b00000001)) {
            // LD rr, nn
            OpLdReg16Imm16();
        } else if(OP_MASK(0b11100111, 0b00100000)) {
            // JR cc, n
            OpJumpRelCond();
        } else if(OP_MASK(0b11111000, 0b10000000)) {
            // ADD A, r
            OpAdd();
        } else if(OP_MASK(0b11001111, 0b00001010)) {
            // ADD HL, rr
            OpAdd16();
        } else if(OP_MASK(0b11111000, 0b10001000)) {
            // ADC A, r
            OpAddCarry();
        } else if(OP_MASK(0b11111000, 0b10010000)) {
            // SUB A, r
            OpSub();
        } else if(OP_MASK(0b11111000, 0b10011000)) {
            // SBC A, r
            OpSubBorrow();
        } else if(OP_MASK(0b11111000, 0b10100000)) {
            // AND r
            OpAnd();
        } else if(OP_MASK(0b11111000, 0b10101000)) {
            // XOR r
            OpXor();
        } else if(OP_MASK(0b11111000, 0b10110000)) {
            // OR r
            OpOr();
        } else if(OP_MASK(0b11111000, 0b10111000)) {
            // CP r
            OpCarry();
        } else if(OP_MASK(0b11100111, 0b11000000)) {
            // RET cc
            OpRetCond();
        } else if(OP_MASK(0b11100111, 0b11000010)) {
            // JP cc, nn
            OpJumpAbsCond();
        } else if(OP_MASK(0b11100111, 0b11000100)) {
            // CALL cc, nn
            OpCallCond();
        } else if(OP_MASK(0b11001111, 0b11000001)) {
            // POP rr
            OpPopReg16();
        } else if(OP_MASK(0b11001111, 0b11000101)) {
            // PUSH rr
            OpPushReg16();
        } else if(OP_MASK(0b11000111, 0b11000111)) {
            // RST [00h, 08h, 10h, 18h, 20h, 28h, 30h, 38h]
            OpFuncFixedUnc();
        } else {
            std::stringstream ex;
            ex << "unknown opcode: " << PlipUtility::FormatHex(m_instr[0], 2)
               << "\n\n" << DumpRegisters();
            throw PlipEmulationException(ex.str().c_str());
        }
    }

    void SharpLr35902::DecodeCB() {
        if(OP_CB_MASK(0b11111000, 0b00000000)) {
            // RLC r
            OpRotateLeft();
        } else if(OP_CB_MASK(0b11111000, 0b00001000)) {
            // RRC r
            OpRotateRight();
        } else if(OP_CB_MASK(0b11111000, 0b00010000)) {
            // RL r
            OpRotateLeftThruCarry();
        } else if(OP_CB_MASK(0b11111000, 0b00011000)) {
            // RR r
            OpRotateRightThruCarry();
        } else if(OP_CB_MASK(0b11111000, 0b00100000)) {
            // SLA r
            OpShiftLeftArithmetic();
        } else if(OP_CB_MASK(0b11111000, 0b00101000)) {
            // SRA r
            OpShiftRightArithmetic();
        } else if(OP_CB_MASK(0b11111000, 0b00110000)) {
            // SWAP r
            OpNibbleSwap();
        } else if(OP_CB_MASK(0b11111000, 0b00111000)) {
            // SRL r
            OpShiftRightLogical();
        } else if(OP_CB_MASK(0b11000000, 0b01000000)) {
            // BIT n, r
            OpBitTest();
        } else if(OP_CB_MASK(0b11000000, 0b10000000)) {
            // SET n, r
            OpBitSet();
        } else if(OP_CB_MASK(0b11000000, 0b11000000)) {
            // RES n, r
            OpBitClear();
        } else {
            std::stringstream ex;
            ex << "unknown opcode: 0xCB " << PlipUtility::FormatHex(m_instr[1], 2)
               << "\n\n" << DumpRegisters();
            throw PlipEmulationException(ex.str().c_str());
        }
    }
}
