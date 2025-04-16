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

    enum class SharpLr35902Interrupt {
        VBlank = 0b00001,
        Lcd    = 0b00010,
        Timer  = 0b00100,
        Serial = 0b01000,
        Joypad = 0b10000
    };

    class SharpLr35902 : public PlipCpu {
    public:
        SharpLr35902(long hz, PlipMemoryMap* memoryMap, bool gbcMode);
        virtual ~SharpLr35902() = default;

        bool IsChangingSpeed() const { return m_changingSpeed; }
        bool IsDoubleSpeed() const { return m_doubleSpeed; }
        bool IsHalted() const { return m_halt; }
        long Step() override;
        [[nodiscard]] unsigned long GetPc() const override;
        [[nodiscard]] std::map<std::string, DebugValue> GetDebugInfo() const;
        [[nodiscard]] std::map<std::string, DebugValue> GetRegisters() const override;
        void Reset(uint32_t pc) override;

    protected:
        bool m_halt = false;
        bool m_holdPc = false;
        SharpLr35902Registers m_registers {};
        SharpLr35902ImeState m_ime = SharpLr35902ImeState::Enabled;
        long m_baseSpeed {};
        bool m_gbcMode {};
        bool m_doubleSpeed {};
        bool m_changingSpeed {};
        int m_speedChangeTimer {};

    private:
        static constexpr int SpeedSwitchDelay = 2050;  // In M-cycles
        
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
        [[nodiscard]] bool TestConditional(int conditional) const;

        bool m_enableInterrupts {};
    };
}
