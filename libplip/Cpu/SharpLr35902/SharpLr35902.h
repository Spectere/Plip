/* SharpLr35902.h
 *
 * An implementation of the Sharp LR35902 (GameBoy) CPU.
 */

#pragma once

#include <vector>

#include "../PlipCpu.h"
#include "../../Memory/PlipMemoryMap.h"

namespace Plip::Cpu {
    class SharpLr35902 : PlipCpu {
    public:
        SharpLr35902(long hz, PlipMemoryMap* memoryMap, uint32_t pc);

        struct Registers {
            uint8_t a;
            uint8_t f;
            uint8_t b;
            uint8_t c;
            uint8_t d;
            uint8_t e;
            uint8_t h;
            uint8_t l;

            uint16_t sp;
            uint16_t pc;
        };

        void Cycle() override;
        void Reset(uint32_t pc) override;

    private:
        void PerformReset(uint32_t pc);

        bool m_allowFetch = true;
        std::vector<uint8_t> m_instr;
        Registers m_reg {};
        uint8_t m_stage = 0;
    };
}
