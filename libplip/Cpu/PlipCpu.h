/* PlipCpu.h
 *
 * Defines a CPU implementation.
 */

#pragma once

#include <string>

#include "../Memory/PlipMemoryMap.h"

namespace Plip::Cpu {
    class PlipCpu {
    public:
        [[nodiscard]] long GetCycleTime() const;
        [[nodiscard]] long GetHz(long hz) const;
        void SetHz(long hz);

        virtual void Cycle() = 0;
        virtual void Reset(uint32_t pc) = 0;

    protected:
        PlipCpu(long hz, PlipMemoryMap* memoryMap);

        long m_hz {};
        Plip::PlipMemoryMap *m_memory;

    private:
        long m_cycle {};
    };
}
