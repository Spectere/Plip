/* SharpLr35902.cpp
 *
 * An implementation of the Sharp LR35902 (GameBoy) CPU.
 */

#include "SharpLr35902.h"
#include "SharpLr35902.Macros.h"

namespace Plip::Cpu {
    SharpLr35902::SharpLr35902(long hz, PlipMemoryMap *memoryMap, uint32_t pc)
    : PlipCpu(hz, memoryMap) {
        PerformReset(pc);
    }

    void SharpLr35902::Cycle() {
        // Reduce nesting with a harmless little goto. :)
        if(m_instr.empty()) goto fetch;

    fetch:
        if(m_allowFetch) {
            m_stage = 0;
            FETCH;
        }
    }

    void SharpLr35902::PerformReset(uint32_t pc) {
        m_reg.a = 0;
        m_reg.f = 0;
        m_reg.b = 0;
        m_reg.c = 0;
        m_reg.d = 0;
        m_reg.e = 0;
        m_reg.h = 0;
        m_reg.l = 0;

        m_reg.sp = 0;
        m_reg.pc = pc;

        m_allowFetch = true;
        m_instr.clear();
        m_stage = 0;
    }

    void SharpLr35902::Reset(uint32_t pc) {
        PerformReset(pc);
    }
}
