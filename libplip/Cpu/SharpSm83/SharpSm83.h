/* SharpSm83.h
 *
 * An implementation of a Sharp SM83-based CPU.
 */

#pragma once

#include <array>
#include <utility>

#include "SharpSm83Registers.h"
#include "Cpu/PlipCpu.h"

namespace Plip::Cpu {
    enum class SharpSm83ImeState {
        Disabled,
        PendingEnable,
        Enabled
    };

    enum class SharpSm83Interrupt {
        VBlank = 0b00001,
        Lcd    = 0b00010,
        Timer  = 0b00100,
        Serial = 0b01000,
        Joypad = 0b10000
    };

    class SharpSm83 : public PlipCpu {
    public:
        SharpSm83(long hz, PlipMemoryMap* memoryMap, bool gbcMode);
        virtual ~SharpSm83() = default;

        [[nodiscard]] bool IsChangingSpeed() const { return m_changingSpeed; }
        [[nodiscard]] bool IsDoubleSpeed() const { return m_doubleSpeed; }
        [[nodiscard]] bool IsHalted() const { return m_halt; }
        long Cycle() override;
        [[nodiscard]] unsigned long GetPc() const override;
        [[nodiscard]] std::map<std::string, DebugValue> GetDebugInfo() const;
        [[nodiscard]] std::map<std::string, DebugValue> GetRegisters() const override;
        void Reset(uint32_t pc) override;

    protected:
        bool m_halt = false;
        bool m_holdPc = false;
        SharpSm83Registers m_registers {};
        SharpSm83ImeState m_ime = SharpSm83ImeState::Enabled;
        long m_baseSpeed {};
        bool m_gbcMode {};
        bool m_doubleSpeed {};
        bool m_changingSpeed {};
        int m_speedChangeTimer {};
        bool m_enableInterrupts {};

    private:
        static constexpr int SpeedSwitchDelay = 8200;  // In T-cycles
        static constexpr int MCycleLength = 4;

        int m_activeInterrupts {};
        int m_cycleCount {};
        uint8_t m_op {};

        long DecodeAndExecute();
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
        std::pair<uint8_t, uint8_t> Pop16FromStackSplit();
        void Push8ToStack(uint8_t value);
        void Push16ToStack(uint16_t value);
        void Push16ToStack(uint8_t high, uint8_t low);
        void ServiceInterrupt(int activeInterrupts);
        [[nodiscard]] bool TestConditional(int conditional) const;

        // Helper Methods
        void AdvanceMCycle(const int cycles = 1) { m_cycleCount += MCycleLength * cycles; }
        [[nodiscard]] uint8_t GetOpParamX() const { return (m_op >> 3) & 0b111; }
        [[nodiscard]] uint8_t GetOpParamY() const { return m_op & 0b111; }
        [[nodiscard]] uint8_t GetOpParam16() const { return (m_op >> 4) & 0b11; }
        [[nodiscard]] uint8_t GetOpConditional() const { return (m_op >> 3) & 0b11; }

        void CheckAddCarry(const int left, const int right)
            { m_registers.SetCarryFlagTo((left + right) > 0xFF); }
        void CheckAddHalfCarry(const int left, const int right, const int carry = 0)
            { m_registers.SetHalfCarryFlagTo(((left & 0xF) + (right & 0xF) + carry) > 0xF); }
        void CheckSubBorrow(const int left, const int right, const int borrow = 0)
            { m_registers.SetCarryFlagTo((left - right - borrow) < 0); }
        void CheckSubHalfBorrow(const int left, const int right, const int borrow = 0)
            { m_registers.SetHalfCarryFlagTo(((left & 0xF) - (right & 0xF) - borrow) < 0); }

        [[nodiscard]] uint8_t FetchAtAddress(const uint16_t addr) {
            const auto val = m_memory->GetByte(addr);
            AdvanceMCycle();
            return val;
        }

        [[nodiscard]] uint8_t FetchAtPc() {
            const auto val = m_memory->GetByte(m_registers.PC);
            if(!m_holdPc) ++m_registers.PC; else m_holdPc = false;
            AdvanceMCycle();
            return val;
        }

        [[nodiscard]] uint16_t FetchAtPc16() {
            const auto low = FetchAtPc();
            const auto high = FetchAtPc();
            return (high << 8) | low;
        }

        [[nodiscard]] uint8_t GetInterruptEnable() const { return m_memory->GetByte(0xFFFF); }
        [[nodiscard]] uint8_t GetInterruptFlag() const { return m_memory->GetByte(0xFF0F); }

        void JumpAbsolute(const uint16_t addr) {
            m_registers.PC = addr;
            AdvanceMCycle();
        }

        void StoreAtAddress(const uint16_t addr, const uint8_t val) {
            m_memory->SetByte(addr, val);
            AdvanceMCycle();
        }

        // Op Tables
        using OpHandler = void (SharpSm83::*)();  // what even is this syntax? X_x
        static const std::array<OpHandler, 256> OpTable;    // SharpSm83.Ops.cpp
        static const std::array<OpHandler, 256> OpTableCb;  // SharpSm83.Ops.cpp

        //
        // Ops
        //
        void Op_Invalid();
        void Op_InvalidCB();

        void Op_CB();

        // Miscellaneous / Control Instructions
        void Op_NOP();
        void Op_STOP();
        void Op_HALT();
        void Op_DI();
        void Op_EI();

        // Jumps/Calls
        void Op_JR_imm8s();
        void Op_JR_c_imm8s();
        void Op_RET();
        void Op_RETI();
        void Op_RET_c();
        void Op_JP_imm16();
        void Op_JP_c_imm16();
        void Op_RST_vec();
        void Op_JP_HL();
        void Op_CALL_imm16();
        void Op_CALL_c_imm16();

        // 8-bit Load Instructions
        void Op_LD_mem_A();
        void Op_LD_reg_imm8();
        void Op_LD_A_mem();
        void Op_LD_memHL_imm8();
        void Op_LD_reg_reg();
        void Op_LD_reg_memHL();
        void Op_LD_memHL_reg();
        void Op_LDH_memImm8_A();
        void Op_LDH_memC_A();
        void Op_LD_memImm16_A();
        void Op_LDH_A_imm8();
        void Op_LDH_A_memC();
        void Op_LD_A_memImm16();

        // 16-bit Load Instructions
        void Op_LD_reg16_imm16();
        void Op_LD_memImm16_SP();
        void Op_POP_reg16();
        void Op_PUSH_reg16();
        void Op_LD_HL_SPimm8s();
        void Op_LD_SP_HL();

        // 8-bit Arithmetic / Logical Instructions
        void Op_INC_reg();              void Op_INC_memHL();
        void Op_DEC_reg();              void Op_DEC_memHL();
        void Op_DAA();
        void Op_CPL();
        void Op_SCF();
        void Op_CCF();
        void Op_ADD_ADC_A_reg();        void Op_ADD_ADC_A_memHL();      void Op_ADD_ADC_A_imm8();
        void Op_SUB_SBC_A_reg();        void Op_SUB_SBC_A_memHL();      void Op_SUB_SBC_A_imm8();
        void Op_AND_A_reg();            void Op_AND_A_memHL();          void Op_AND_A_imm8();
        void Op_XOR_A_reg();            void Op_XOR_A_memHL();          void Op_XOR_A_imm8();
        void Op_OR_A_reg();             void Op_OR_A_memHL();           void Op_OR_A_imm8();
        void Op_CP_A_reg();             void Op_CP_A_memHL();           void Op_CP_A_imm8();

        // 16-bit Arithmetic
        void Op_INC_reg16();
        void Op_DEC_reg16();
        void Op_ADD_HL_reg16();
        void Op_ADD_SP_imm8s();

        // 8-bit Shift, Rotate, and Bit Instructions
        void Op_RLCA();                 void Op_RLA();
        void Op_RRCA();                 void Op_RRA();

        // Bit Manipulation (0xCB Ops)
        void Op_RLC_RL_reg();           void Op_RLC_RL_memHL();
        void Op_RRC_RR_reg();           void Op_RRC_RR_memHL();
        void Op_SLA_reg();              void Op_SLA_memHL();
        void Op_SRA_reg();              void Op_SRA_memHL();
        void Op_SWAP_reg();             void Op_SWAP_memHL();
        void Op_SRL_reg();              void Op_SRL_memHL();
        void Op_BIT_reg();              void Op_BIT_memHL();
        void Op_RES_reg();              void Op_RES_memHL();
        void Op_SET_reg();              void Op_SET_memHL();
    };
}
