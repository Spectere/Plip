/* PlipCpu.h
 *
 * Defines a CPU implementation.
 */

#pragma once

#include <map>
#include <string>

#include "../Memory/PlipMemoryMap.h"

namespace Plip::Cpu {
    enum RegisterValueType {
        Unknown,
        Int8,
        Int16Le,
        Int16Be,
        Int32Le,
        Int32Be,
        Int64Le,
        Int64Be,
        Float32Le,
        Float32Be,
        Float64Le,
        Float64Be
    };

    struct RegisterValue {
        RegisterValue(): Type(Unknown), ValueInt(0) {}
        RegisterValue(const RegisterValueType type, const uint64_t value) : Type(type), ValueInt(value) {}
        RegisterValue(const RegisterValueType type, const double value) : Type(type), ValueFloat(value) {}

        RegisterValueType Type;

        union {
            uint64_t ValueInt;
            double ValueFloat;
        };
    };

    class PlipCpu {
    public:
        [[nodiscard]] long GetCycleTime() const;
        [[nodiscard]] long GetHz(long hz) const;
        void SetHz(long hz);

        virtual void Cycle() = 0;
        [[nodiscard]] virtual unsigned long GetPc() const = 0;
        [[nodiscard]] virtual std::map<std::string, RegisterValue> GetRegisters() const = 0;
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
