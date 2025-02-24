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
        void OpJumpRelative(int8_t offset);
        void OpReturn();
        uint8_t OpRotateLeft(uint8_t value, bool throughCarry, bool checkZeroFlag);
        uint8_t OpRotateRight(uint8_t value, bool throughCarry, bool checkZeroFlag);
        uint8_t OpShiftLeft(uint8_t value);
        uint8_t OpShiftRight(uint8_t value, bool arithmetic);
        void OpSubtractFromRegisterA(int value, bool subtractWithBorrow, bool discardResult);
        uint8_t OpSwapNibbles(uint8_t value);
        uint8_t Pop8FromStack();
        uint16_t Pop16FromStack();
        void Pop16FromStack(uint8_t& high, uint8_t& low);
        void Push8ToStack(uint8_t value);
        void Push16ToStack(uint16_t value);
        void Push16ToStack(uint8_t high, uint8_t low);
        bool TestConditional(int conditional) const;
    };
}
