/* PlipCpu.h
 *
 * Defines a CPU implementation.
 */

#pragma once

#include <map>
#include <string>

#include "../DebugValue.h"
#include "../Memory/PlipMemoryMap.h"

namespace Plip::Cpu {
    class PlipCpu {
    public:
        [[nodiscard]] long GetCycleTime() const;
        [[nodiscard]] long GetHz(long hz) const;
        void SetHz(long hz);

        virtual void Cycle() = 0;
        [[nodiscard]] virtual unsigned long GetPc() const = 0;
        [[nodiscard]] virtual std::map<std::string, DebugValue> GetRegisters() const = 0;
        virtual void Reset(uint32_t pc) = 0;

    protected:
        PlipCpu(long hz, PlipMemoryMap* memoryMap);
        ~PlipCpu() = default;

        long m_hz {};
        PlipMemoryMap *m_memory;

    private:
        long m_cycle {};
    };
}
