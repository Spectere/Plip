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
        switch(m_instr[0]) {
            /*
             * Non-parameterized opcodes.
             */

            // NOP
            case 0b00000000:
                NUM_MCYCLES(2); break;

            // 0xCB
            case 0b11001011:
                CYCLE(2) { FETCH; m_mcycle++; }
                else { DecodeCB(); }
                break;

            // RLCA
            case 0b00000111:
                OpAccumRotateLeft(); break;

            // LD (nn), SP
            case 0b00001000:
                OpLdMemSp(); break;

            // RRCA
            case 0b00001111:
                OpAccumRotateRight(); break;

            // STOP
            case 0b00010000:
                OpStop(); break;

            // JR e
            case 0b00011000:
                OpJumpRelUnc(); break;

            // RLA
            case 0b00010111:
                OpAccumRotateLeftThruCarry(); break;

            // RRA
            case 0b00011111:
                OpAccumRotateRightThruCarry(); break;

            // DAA
            case 0b00100111:
                OpAccumBcd(); break;

            // CPL
            case 0b00101111:
                OpAccumFlip(); break;

            // SCF
            case 0b00110111:
                OpSetCarry(); break;

            // CCF
            case 0b00111111:
                OpFlipCarry(); break;

            // HALT
            case 0b01110110:
                OpHalt(); break;

            // ADD A, n
            case 0b11000110:
                OpAccumAddImm(); break;

            // RET
            case 0b11001001:
                OpRetUnc(); break;

            // JP nn
            case 0b11000011:
                OpJumpAbsUnc(); break;

            // CALL nn
            case 0b11001101:
                OpCallUnc(); break;

            // ADC A, n
            case 0b11001110:
                OpAccumAddCarryImm(); break;

            // RETI
            case 0b11011001:
                OpRetImeUnc(); break;

            // SUB A, n
            case 0b11010110:
                OpAccumSubImm(); break;

            // SBC A, n
            case 0b11011110:
                OpAccumSubBorrowImm(); break;

            // LDH (n), A
            case 0b11100000:
                OpLdMemHighImmAccum(); break;

            // LDH (C), A
            case 0b11100010:
                OpLdMemHighCAccum(); break;

            // AND n
            case 0b11100110:
                OpAccumAndImm(); break;

            // ADD SP, e
            case 0b11101000:
                OpAddSpReg(); break;

            // JP HP
            case 0b11101001:
                OpJumpRegUnc(); break;

            // LD (nn), A
            case 0b11101010:
                OpLdMemAccum(); break;

            // XOR n
            case 0b11101110:
                OpAccumXorImm(); break;

            // LDH A, (n)
            case 0b11110000:
                OpLdAccumMemHighImm(); break;

            // OR n
            case 0b11110110:
                OpAccumOrImm(); break;

            // LDH A, (C)
            case 0b11110010:
                OpLdAccumMemHighC(); break;

            // DI
            case 0b11110011:
                OpDisableInterrupts(); break;

            // LD HL, SP+e
            case 0b11111000:
                OpLdHlSpOffset(); break;

            // LD SP, HL
            case 0b11111001:
                OpLdSpHl(); break;

            // LD A, (nn)
            case 0b11111010:
                OpLdAccumMem(); break;

            // EI
            case 0b11111011:
                OpEnableInterrupts(); break;

            // CP n
            case 0b11111110:
                OpAccumCarryImm(); break;

            /*
             * Parameterized opcodes.
             */

            // INC r (mask: 0b11000111, op: 0b00000100)
            case 0b00000100: case 0b00001100: case 0b00010100: case 0b00011100:
            case 0b00100100: case 0b00101100: case 0b00110100: case 0b00111100:
                OpIncReg(); break;

            // DEC r (mask: 0b11000111, op: 0b00000101)
            case 0b00000101: case 0b00001101: case 0b00010101: case 0b00011101:
            case 0b00100101: case 0b00101101: case 0b00110101: case 0b00111101:
                OpDecReg(); break;

            // INC rr (mask: 0b11001111, op: 0b00000011)
            case 0b00000011: case 0b00010011: case 0b00100011: case 0b00110011:
                OpIncPair(); break;

            // DEC rr (mask: 0b11001111, op: 0b00001011)
            case 0b00001011: case 0b00011011: case 0b00101011: case 0b00111011:
                OpDecPair(); break;

            // LD r, r' / LD r, (HL) / LD (HL), r
            // (mask: 0b11000000, 0b01000000)
            case 0b01000000: case 0b01000001: case 0b01000010: case 0b01000011:
            case 0b01000100: case 0b01000101: case 0b01000110: case 0b01000111:
            case 0b01001000: case 0b01001001: case 0b01001010: case 0b01001011:
            case 0b01001100: case 0b01001101: case 0b01001110: case 0b01001111:
            case 0b01010000: case 0b01010001: case 0b01010010: case 0b01010011:
            case 0b01010100: case 0b01010101: case 0b01010110: case 0b01010111:
            case 0b01011000: case 0b01011001: case 0b01011010: case 0b01011011:
            case 0b01011100: case 0b01011101: case 0b01011110: case 0b01011111:
            case 0b01100000: case 0b01100001: case 0b01100010: case 0b01100011:
            case 0b01100100: case 0b01100101: case 0b01100110: case 0b01100111:
            case 0b01101000: case 0b01101001: case 0b01101010: case 0b01101011:
            case 0b01101100: case 0b01101101: case 0b01101110: case 0b01101111:
            case 0b01110000: case 0b01110001: case 0b01110010: case 0b01110011:
            case 0b01110100: case 0b01110101: /* 0b01110110 */ case 0b01110111:
            case 0b01111000: case 0b01111001: case 0b01111010: case 0b01111011:
            case 0b01111100: case 0b01111101: case 0b01111110: case 0b01111111:
                OpLdRegReg(); break;

            // LD r, n (mask: 0b11000111, op: 0b00000110)
            case 0b00000110: case 0b00001110: case 0b00010110: case 0b00011110:
            case 0b00100110: case 0b00101110: case 0b00110110: case 0b00111110:
                OpLdRegImm(); break;

            // LD A, (rr) (mask: 0b11001111, op: 0b00001010)
            case 0b00001010: case 0b00011010: case 0b00101010: case 0b00111010:
                OpLdRegMem(); break;

            // LD (rr), A (mask: 0b11001111, op: 0b00000010)
            case 0b00000010: case 0b00010010: case 0b00100010: case 0b00110010:
                OpLdMemReg(); break;

            // LD rr, nn (mask: 0b11001111, op: 0b00000001)
            case 0b00000001: case 0b00010001: case 0b00100001: case 0b00110001:
                OpLdReg16Imm16(); break;

            // JR cc, e (mask: 0b11100111, op: 0b00100000)
            case 0b00100000: case 0b00101000: case 0b00110000: case 0b00111000:
                OpJumpRelCond(); break;

            // ADD A, r (mask: 0b11111000, op: 0b10000000)
            case 0b10000000: case 0b10000001: case 0b10000010: case 0b10000011:
            case 0b10000100: case 0b10000101: case 0b10000110: case 0b10000111:
                OpAdd(); break;

            // ADD HL, rr (mask: 0b11001111, op: 0b00001001)
            case 0b00001001: case 0b00011001: case 0b00101001: case 0b00111001:
                OpAdd16(); break;

            // ADC A, r (mask: 0b11111000, op: 0b10001000)
            case 0b10001000: case 0b10001001: case 0b10001010: case 0b10001011:
            case 0b10001100: case 0b10001101: case 0b10001110: case 0b10001111:
                OpAddCarry(); break;

            // SUB A, r (mask: 0b11111000, 0b10010000)
            case 0b10010000: case 0b10010001: case 0b10010010: case 0b10010011:
            case 0b10010100: case 0b10010101: case 0b10010110: case 0b10010111:
                OpSub(); break;

            // SBC A, r (mask: 0b11111000, 0b10011000)
            case 0b10011000: case 0b10011001: case 0b10011010: case 0b10011011:
            case 0b10011100: case 0b10011101: case 0b10011110: case 0b10011111:
                OpSubBorrow(); break;

            // AND r (mask: 0b11111000, 0b10100000)
            case 0b10100000: case 0b10100001: case 0b10100010: case 0b10100011:
            case 0b10100100: case 0b10100101: case 0b10100110: case 0b10100111:
                OpAnd(); break;

            // XOR r (mask: 0b11111000, op: 0b10101000)
            case 0b10101000: case 0b10101001: case 0b10101010: case 0b10101011:
            case 0b10101100: case 0b10101101: case 0b10101110: case 0b10101111:
                OpXor(); break;

            // OR r (mask: 0b11111000, op: 0b10110000)
            case 0b10110000: case 0b10110001: case 0b10110010: case 0b10110011:
            case 0b10110100: case 0b10110101: case 0b10110110: case 0b10110111:
                OpOr(); break;

            // CP r (mask: 0b11111000, op: 0b10111000))
            case 0b10111000: case 0b10111001: case 0b10111010: case 0b10111011:
            case 0b10111100: case 0b10111101: case 0b10111110: case 0b10111111:
                OpCarry(); break;

            // RET cc (mask: 0b11100111, op: 0b11000000)
            case 0b11000000: case 0b11001000: case 0b11010000: case 0b11011000:
                OpRetCond(); break;

            // JP cc, nn (mask: 0b11100111, op: 0b11000010)
            case 0b11000010: case 0b11001010: case 0b11010010: case 0b11011010:
                OpJumpAbsCond(); break;

            // CALL cc, nn (mask: 0b11100111, op: 0b11000100)
            case 0b11000100: case 0b11001100: case 0b11010100: case 0b11011100:
                OpCallCond(); break;

            // POP rr (mask: 0b11001111, op: 0b11000001)
            case 0b11000001: case 0b11010001: case 0b11100001: case 0b11110001:
                OpPopReg16(); break;

            // PUSH rr (mask: 0b11001111, op: 0b11000101)
            case 0b11000101: case 0b11010101: case 0b11100101: case 0b11110101:
                OpPushReg16(); break;

            // RST [00h, 08h, 10h, 18h, 20h, 28h, 30h, 38h]
            // (mask: 0b11000111, op: 0b11000111)
            case 0b11000111: case 0b11001111: case 0b11010111: case 0b11011111:
            case 0b11100111: case 0b11101111: case 0b11110111: case 0b11111111:
                OpFuncFixedUnc(); break;

            default:
                std::stringstream ex;
                ex << "unknown opcode: " << PlipUtility::FormatHex(m_instr[0], 2)
                   << "\n\n" << DumpRegisters();
                throw PlipEmulationException(ex.str().c_str());
        }
    }

    void SharpLr35902::DecodeCB() {
        switch(m_instr[1]) {
            // RLC r (mask: 0b11111000, op: 0b00000000)
            case 0b00000000: case 0b00000001: case 0b00000010: case 0b00000011:
            case 0b00000100: case 0b00000101: case 0b00000110: case 0b00000111:
                OpRotateLeft(); break;

            // RRC r (mask: 0b11111000, op: 0b00001000)
            case 0b00001000: case 0b00001001: case 0b00001010: case 0b00001011:
            case 0b00001100: case 0b00001101: case 0b00001110: case 0b00001111:
                OpRotateRight(); break;

            // RL r (mask: 0b11111000, op: 0b00010000)
            case 0b00010000: case 0b00010001: case 0b00010010: case 0b00010011:
            case 0b00010100: case 0b00010101: case 0b00010110: case 0b00010111:
                OpRotateLeftThruCarry(); break;

            // RR r (mask: 0b11111000, op: 0b00011000)
            case 0b00011000: case 0b00011001: case 0b00011010: case 0b00011011:
            case 0b00011100: case 0b00011101: case 0b00011110: case 0b00011111:
                OpRotateRightThruCarry(); break;

            // SLA r (mask: 0b11111000, op: 0b00100000)
            case 0b00100000: case 0b00100001: case 0b00100010: case 0b00100011:
            case 0b00100100: case 0b00100101: case 0b00100110: case 0b00100111:
                OpShiftLeftArithmetic(); break;

            // SRA r (mask: 0b11111000, op: 0b00101000)
            case 0b00101000: case 0b00101001: case 0b00101010: case 0b00101011:
            case 0b00101100: case 0b00101101: case 0b00101110: case 0b00101111:
                OpShiftRightArithmetic(); break;

            // SWAP r (mask: 0b11111000, op: 0b00110000)
            case 0b00110000: case 0b00110001: case 0b00110010: case 0b00110011:
            case 0b00110100: case 0b00110101: case 0b00110110: case 0b00110111:
                OpNibbleSwap(); break;

            // SRL r (mask: 0b11111000, op: 0b00111000)
            case 0b00111000: case 0b00111001: case 0b00111010: case 0b00111011:
            case 0b00111100: case 0b00111101: case 0b00111110: case 0b00111111:
                OpShiftRightLogical(); break;

            // BIT n, r (mask: 0b11000000, op: 0b01000000)
            case 0b01000000: case 0b01000001: case 0b01000010: case 0b01000011:
            case 0b01000100: case 0b01000101: case 0b01000110: case 0b01000111:
            case 0b01001000: case 0b01001001: case 0b01001010: case 0b01001011:
            case 0b01001100: case 0b01001101: case 0b01001110: case 0b01001111:
            case 0b01010000: case 0b01010001: case 0b01010010: case 0b01010011:
            case 0b01010100: case 0b01010101: case 0b01010110: case 0b01010111:
            case 0b01011000: case 0b01011001: case 0b01011010: case 0b01011011:
            case 0b01011100: case 0b01011101: case 0b01011110: case 0b01011111:
            case 0b01100000: case 0b01100001: case 0b01100010: case 0b01100011:
            case 0b01100100: case 0b01100101: case 0b01100110: case 0b01100111:
            case 0b01101000: case 0b01101001: case 0b01101010: case 0b01101011:
            case 0b01101100: case 0b01101101: case 0b01101110: case 0b01101111:
            case 0b01110000: case 0b01110001: case 0b01110010: case 0b01110011:
            case 0b01110100: case 0b01110101: case 0b01110110: case 0b01110111:
            case 0b01111000: case 0b01111001: case 0b01111010: case 0b01111011:
            case 0b01111100: case 0b01111101: case 0b01111110: case 0b01111111:
                OpBitTest(); break;

            // RES n, r (mask: 0b11000000, op: 0b10000000)
            case 0b10000000: case 0b10000001: case 0b10000010: case 0b10000011:
            case 0b10000100: case 0b10000101: case 0b10000110: case 0b10000111:
            case 0b10001000: case 0b10001001: case 0b10001010: case 0b10001011:
            case 0b10001100: case 0b10001101: case 0b10001110: case 0b10001111:
            case 0b10010000: case 0b10010001: case 0b10010010: case 0b10010011:
            case 0b10010100: case 0b10010101: case 0b10010110: case 0b10010111:
            case 0b10011000: case 0b10011001: case 0b10011010: case 0b10011011:
            case 0b10011100: case 0b10011101: case 0b10011110: case 0b10011111:
            case 0b10100000: case 0b10100001: case 0b10100010: case 0b10100011:
            case 0b10100100: case 0b10100101: case 0b10100110: case 0b10100111:
            case 0b10101000: case 0b10101001: case 0b10101010: case 0b10101011:
            case 0b10101100: case 0b10101101: case 0b10101110: case 0b10101111:
            case 0b10110000: case 0b10110001: case 0b10110010: case 0b10110011:
            case 0b10110100: case 0b10110101: case 0b10110110: case 0b10110111:
            case 0b10111000: case 0b10111001: case 0b10111010: case 0b10111011:
            case 0b10111100: case 0b10111101: case 0b10111110: case 0b10111111:
                OpBitClear(); break;

            // SET n, r (mask: 0b11000000, op: 0b11000000)
            case 0b11000000: case 0b11000001: case 0b11000010: case 0b11000011:
            case 0b11000100: case 0b11000101: case 0b11000110: case 0b11000111:
            case 0b11001000: case 0b11001001: case 0b11001010: case 0b11001011:
            case 0b11001100: case 0b11001101: case 0b11001110: case 0b11001111:
            case 0b11010000: case 0b11010001: case 0b11010010: case 0b11010011:
            case 0b11010100: case 0b11010101: case 0b11010110: case 0b11010111:
            case 0b11011000: case 0b11011001: case 0b11011010: case 0b11011011:
            case 0b11011100: case 0b11011101: case 0b11011110: case 0b11011111:
            case 0b11100000: case 0b11100001: case 0b11100010: case 0b11100011:
            case 0b11100100: case 0b11100101: case 0b11100110: case 0b11100111:
            case 0b11101000: case 0b11101001: case 0b11101010: case 0b11101011:
            case 0b11101100: case 0b11101101: case 0b11101110: case 0b11101111:
            case 0b11110000: case 0b11110001: case 0b11110010: case 0b11110011:
            case 0b11110100: case 0b11110101: case 0b11110110: case 0b11110111:
            case 0b11111000: case 0b11111001: case 0b11111010: case 0b11111011:
            case 0b11111100: case 0b11111101: case 0b11111110: case 0b11111111:
                OpBitSet(); break;

            default:
                std::stringstream ex;
                ex << "unknown opcode: 0xCB " << PlipUtility::FormatHex(m_instr[1], 2)
                   << "\n\n" << DumpRegisters();
                throw PlipEmulationException(ex.str().c_str());
        }
    }
}
