/* SharpLr35902.cpp
 *
 * An implementation of the Sharp LR35902 (GameBoy) CPU.
 */

#include <sstream>

#include "../../PlipEmulationException.h"
#include "../../PlipUtility.h"

#include "SharpLr35902.h"
#include "SharpLr35902.Macros.h"

namespace Plip::Cpu {
    SharpLr35902::SharpLr35902(long hz, PlipMemoryMap *memoryMap, uint32_t pc)
    : PlipCpu(hz, memoryMap) {
        PerformReset(pc);
    }

    void SharpLr35902::Cycle() {
        if(!m_instr.empty())
            Decode();

        // The LR35902 allows the next instruction to be fetched when the
        // previous instruction finishes its last execute stage.
        if(m_allowFetch) {
            FETCH;
            BEGIN_EXECUTE;
        }
    }

    std::string SharpLr35902::DumpRegisters() const {
        using util = PlipUtility;
        std::stringstream dump;

        dump << util::DumpValue(" A", m_reg.a, 2) << '\n'
             << util::DumpValue(" F", m_reg.f, 2) << '\n'
             << util::DumpValue(" B", m_reg.b, 2) << '\n'
             << util::DumpValue(" C", m_reg.c, 2) << '\n'
             << util::DumpValue(" D", m_reg.d, 2) << '\n'
             << util::DumpValue(" E", m_reg.e, 2) << '\n'
             << util::DumpValue(" H", m_reg.h, 2) << '\n'
             << util::DumpValue(" L", m_reg.l, 2) << "\n\n"
             << util::DumpValue("PC", m_reg.pc, 4) << '\n'
             << util::DumpValue("SP", m_reg.sp, 4) << '\n';

        return dump.str();
    }

    uint8_t* SharpLr35902::GetRegister8(uint8_t idx) {
        switch(idx) {
            case IDX_A: return &(m_reg.a);
            case IDX_B: return &(m_reg.b);
            case IDX_C: return &(m_reg.c);
            case IDX_D: return &(m_reg.d);
            case IDX_E: return &(m_reg.e);
            case IDX_H: return &(m_reg.h);
            case IDX_L: return &(m_reg.l);
            default:
                std::stringstream ex;
                ex << "invalid 8-bit register index: " << PlipUtility::FormatHex(idx, 2)
                   << "\n\n" << DumpRegisters();
                throw PlipEmulationException(ex.str().c_str());
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
        m_mcycle = 0;
    }

    void SharpLr35902::Reset(uint32_t pc) {
        PerformReset(pc);
    }
}
