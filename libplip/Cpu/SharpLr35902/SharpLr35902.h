/* SharpLr35902.h
 *
 * An implementation of a Sharp LR35902 CPU (SM83 core).
 */

#pragma once

#include "SharpLr35902Registers.h"
#include "../PlipCpu.h"

namespace Plip::Cpu {
    class SharpLr35902 final : public PlipCpu {
    public:
        SharpLr35902(long hz, PlipMemoryMap* memoryMap);

        void Cycle() override;
        [[nodiscard]] unsigned long GetPc() const override;
        [[nodiscard]] std::map<std::string, DebugValue> GetRegisters() const override;
        void Reset(uint32_t pc) override;

    private:
        SharpLr35902Registers m_registers {};
    };
}
