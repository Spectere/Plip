/* SharpLr35902.Ops.cpp
 *
 * Sharp LR35902 opcode execution.
 */

#include <sstream>

#include "../../PlipEmulationException.h"

#include "SharpLr35902.h"
#include "SharpLr35902.Macros.h"

namespace Plip::Cpu {
    void SharpLr35902::OpLdRN() {
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

    void SharpLr35902::OpLdRR() {
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
            if(m_mcycle == 2) {
                FETCH_ADDR(REG_COMBINE(m_reg.h, m_reg.l));
            } else if(m_mcycle == 3) {
                *(GetRegister8(dest)) = m_instr[1];
            }
            NUM_MCYCLES(3);
        } else {
            // LD r, r'
            *(GetRegister8(dest)) = *(GetRegister8(src));
            NUM_MCYCLES(2);
        }
    }
}
