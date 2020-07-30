/* SharpLr35902.Ops.cpp
 *
 * Sharp LR35902 opcode execution.
 */

#include <sstream>

#include "../../PlipEmulationException.h"

#include "SharpLr35902.h"
#include "SharpLr35902.Macros.h"

namespace Plip::Cpu {
    void SharpLr35902::OpLdMemReg() {
        uint16_t addr;
        uint8_t *src;

        if(m_mcycle == 2) {
            if(OP_MASK(0b11111000, 0b01110000)) {
                // LD (HL), r
                addr = REG_COMBINE(m_reg.h, m_reg.l);
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
        } else {
            // LD r, n
            if(m_mcycle == 2) {
                FETCH;
            } else if(m_mcycle == 3) {
                *(GetRegister8(OP_REG_Y(0))) = m_instr[1];
            }
            NUM_MCYCLES(4);
        }
    }

    void SharpLr35902::OpLdRegMem() {
        uint16_t addr;
        uint8_t *dest;

        if(m_mcycle == 2) {
            if(OP_MASK(0b11000111, 0b01000110)) {
                // LD r, (HL)
                addr = REG_COMBINE(m_reg.h, m_reg.l);
                dest = GetRegister8(OP_REG_X(0));
            } else if(OP_MASK(0b11001111, 0b00001010)) {
                // LD A, (rr)
                addr = GetAddress((m_instr[0] >> 4) & 0b11);
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
            if(m_mcycle == 2)
                MEM_WRITE(REG_HL, *(GetRegister8(src)));

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
}
