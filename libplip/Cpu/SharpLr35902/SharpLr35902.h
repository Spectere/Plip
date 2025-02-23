/* SharpLr35902.h
 *
 * An implementation of a Sharp LR35902 CPU (SM83 core).
 */

#pragma once

#include "SharpLr35902Registers.h"
#include "../PlipCpu.h"

namespace Plip::Cpu {
    class SharpLr35902 : public PlipCpu {
    public:
        SharpLr35902(long hz, PlipMemoryMap* memoryMap);
        virtual ~SharpLr35902() = default;

        long Cycle() override;
        [[nodiscard]] unsigned long GetPc() const override;
        [[nodiscard]] std::map<std::string, DebugValue> GetRegisters() const override;
        void Reset(uint32_t pc) override;

    protected:
        SharpLr35902Registers m_registers {};

    private:
        long DecodeAndExecute();
        void DecodeAndExecuteCb();
        uint16_t GetPointerAddress(int pointerIndex);
        void OpAddToRegisterA(int value, bool addWithCarry);
        void OpBitwiseAndRegisterA(uint8_t value);
        void OpBitwiseOrRegisterA(uint8_t value);
        void OpBitwiseXorRegisterA(uint8_t value);
        void OpSubtractFromRegisterA(int value, bool subtractWithBorrow, bool discardResult);
    };
}
