/* SharpLr35902.h
 *
 * An implementation of a Sharp LR35902 CPU (SM83 core).
 */

#pragma once

#include "SharpLr35902Registers.h"
#include "../PlipCpu.h"

namespace Plip::Cpu {
    enum class SharpLr35902ImeState {
        Disabled,
        PendingEnable,
        Enabled
    };

    class SharpLr35902 : public PlipCpu {
    public:
        SharpLr35902(long hz, PlipMemoryMap* memoryMap);
        virtual ~SharpLr35902() = default;

        long Cycle() override;
        [[nodiscard]] unsigned long GetPc() const override;
        [[nodiscard]] std::map<std::string, DebugValue> GetDebugInfo() const;
        [[nodiscard]] std::map<std::string, DebugValue> GetRegisters() const override;
        void Reset(uint32_t pc) override;

        constexpr static int InterruptVBlank = 0b00001;
        constexpr static int InterruptLcd    = 0b00010;
        constexpr static int InterruptTimer  = 0b00100;
        constexpr static int InterruptSerial = 0b01000;
        constexpr static int InterruptJoypad = 0b10000;

    protected:
        bool m_halt = false;
        bool m_holdPc = false;
        SharpLr35902Registers m_registers {};
        SharpLr35902ImeState m_ime = SharpLr35902ImeState::Enabled;

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
        void ServiceInterrupt(int activeInterrupts);
        bool TestConditional(int conditional) const;

        bool m_enableInterrupts = false;
    };
}
