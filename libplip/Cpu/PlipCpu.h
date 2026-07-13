/* PlipCpu.h
 *
 * Defines a CPU implementation.
 */

#pragma once

#include <map>
#include <string>

#include "DebugValue.h"
#include "Cpu/MemoryBusState.h"
#include "Memory/PlipMemoryMap.h"

namespace Plip::Cpu {
    class PlipCpu {
    public:
        [[nodiscard]] double GetCycleTime() const;
        [[nodiscard]] long GetHz() const;
        void SetHz(long hz);

        virtual long Cycle() = 0;
        [[nodiscard]] uint64_t GetLastOp() const { return m_lastOp; }
        [[nodiscard]] virtual unsigned long GetPc() const = 0;
        [[nodiscard]] virtual std::map<std::string, DebugValue> GetRegisters() const = 0;
        virtual void Reset(uint32_t pc) = 0;
        virtual void Step();

        [[nodiscard]] MemoryBusState GetMemoryBusState() const { return m_memoryBusState; }
        [[nodiscard]] uint64_t GetAddressBus() const { return m_addressBus; }
        [[nodiscard]] uint8_t GetDataBus() const { return m_dataBus; }

    protected:
        PlipCpu(long hz, PlipMemoryMap* memoryMap);
        ~PlipCpu() = default;

        long m_hz {};
        double m_cycle {};
        PlipMemoryMap *m_memory;
        uint64_t m_lastOp;

        MemoryBusState m_memoryBusState = MemoryBusState::None;
        uint64_t m_addressBus {};
        uint8_t m_dataBus {};

    private:
        long m_remainingCycles {};
    };
}
