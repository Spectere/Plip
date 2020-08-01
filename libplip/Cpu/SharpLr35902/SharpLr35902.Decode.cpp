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
        } else {
            std::stringstream ex;
            ex << "unknown opcode: " << PlipUtility::FormatHex(m_instr[0], 2)
               << "\n\n" << DumpRegisters();
            throw PlipEmulationException(ex.str().c_str());
        }
    }

    void SharpLr35902::DecodeCB() {
        if(OP_CB_MASK(0b11000000, 0b01000000)) {
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
