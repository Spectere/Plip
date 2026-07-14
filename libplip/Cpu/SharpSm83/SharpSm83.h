/* SharpSm83.h
 *
 * An implementation of a Sharp SM83-based CPU.
 */

#pragma once

#include <array>

#include "SharpSm83Registers.h"
#include "SharpSm83State.h"
#include "Cpu/PlipCpu.h"

namespace Plip::Cpu {
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
        void Step() override;

    protected:
        static constexpr int InitialImeDelay = 2;  // EI's effect is delayed by a single instruction.

        bool m_halt = false;
        bool m_holdPc = false;
        SharpSm83Registers m_registers {};
        bool m_ime {};
        int m_imeDelay {};
        long m_baseSpeed {};
        bool m_gbcMode {};
        bool m_doubleSpeed {};
        bool m_changingSpeed {};
        int m_speedChangeTimer {};

    private:
        static constexpr int SpeedSwitchDelay = 2050;  // In M-cycles
        static constexpr int MCycleLength = 4;

        int m_mCycle {};
        int m_tCycle = MCycleLength;  // Guarantees the the first T-cycle will always result in an M-cycle.
        uint8_t m_op {};
        SharpSm83State m_state = SharpSm83State::Decode;

        uint8_t m_cb {};   // Prefixed instruction.
        uint8_t m_w {};    // Temporary storage.
        uint8_t m_z {};    // Temporary storage.

        long DecodeAndExecute();
        void ServiceInterrupt(int activeInterrupts);

        void OpAddToRegisterA(int value, bool addWithCarry);
        void OpBitwiseAndRegisterA(uint8_t value);
        void OpBitwiseOrRegisterA(uint8_t value);
        void OpBitwiseXorRegisterA(uint8_t value);
        void OpSubtractFromRegisterA(int value, bool subtractWithBorrow, bool discardResult);
        uint8_t OpRotateLeft(uint8_t value, bool throughCarry, bool checkZeroFlag);
        uint8_t OpRotateRight(uint8_t value, bool throughCarry, bool checkZeroFlag);
        uint8_t OpShiftLeft(uint8_t value);
        uint8_t OpShiftRight(uint8_t value, bool arithmetic);
        uint8_t OpSwapNibbles(uint8_t value);

        // Helper Methods
        static constexpr int AddrBc  = 0b00;
        static constexpr int AddrDe  = 0b01;
        static constexpr int AddrHlI = 0b10;
        static constexpr int AddrHlD = 0b11;

        static constexpr int CondNZ = 0b00;
        static constexpr int CondZ  = 0b01;
        static constexpr int CondNC = 0b10;
        static constexpr int CondC  = 0b11;

        [[nodiscard]] uint8_t GetOpParamX() const { return (m_op >> 3) & 0b111; }
        [[nodiscard]] uint8_t GetOpParamY() const { return m_op & 0b111; }
        [[nodiscard]] uint8_t GetOpParam16() const { return (m_op >> 4) & 0b11; }
        [[nodiscard]] uint8_t GetOpConditional() const { return (m_op >> 3) & 0b11; }

        [[nodiscard]] uint8_t GetCbParamX() const { return (m_cb >> 3) & 0b111; }
        [[nodiscard]] uint8_t GetCbParamY() const { return m_cb & 0b111; }

        [[nodiscard]] uint8_t GetInterruptEnable() const { return m_memory->GetByte(0xFFFF); }
        [[nodiscard]] uint8_t GetInterruptFlag() const { return m_memory->GetByte(0xFF0F); }

        void CheckAddCarry(const int left, const int right)
            { m_registers.SetCarryFlagTo((left + right) > 0xFF); }
        void CheckAddHalfCarry(const int left, const int right, const int carry = 0)
            { m_registers.SetHalfCarryFlagTo(((left & 0xF) + (right & 0xF) + carry) > 0xF); }
        void CheckSubBorrow(const int left, const int right, const int borrow = 0)
            { m_registers.SetCarryFlagTo((left - right - borrow) < 0); }
        void CheckSubHalfBorrow(const int left, const int right, const int borrow = 0)
            { m_registers.SetHalfCarryFlagTo(((left & 0xF) - (right & 0xF) - borrow) < 0); }

        bool TestConditional(const int conditional) const {
            switch(conditional) {
                case CondC:  return m_registers.GetCarryFlag();
                case CondNC: return !m_registers.GetCarryFlag();
                case CondZ:  return m_registers.GetZeroFlag();
                case CondNZ: return !m_registers.GetZeroFlag();
                default:
                    throw PlipEmulationException("BUG: Conditional value out of range.");
            }
        }

        uint8_t ReadMemory(const uint16_t address) {
            m_memoryBusState = MemoryBusState::Read;
            m_addressBus = address;
            return m_dataBus = m_memory->GetByte(address);
        }

        void WriteMemory(const uint16_t address, const uint8_t value) {
            m_memoryBusState = MemoryBusState::Write;
            m_addressBus = address;
            m_dataBus = value;
            m_memory->SetByte(address, value);
        }

        uint8_t FetchAtPc() {
            ReadMemory(m_registers.PC);
            if(!m_holdPc) ++m_registers.PC; else m_holdPc = false;
            return m_dataBus;
        }

        uint16_t GetPointerAddress(const int pointerIndex) {
            switch(pointerIndex) {
                case AddrBc: return m_registers.GetBc();
                case AddrDe: return m_registers.GetDe();
                case AddrHlI: {
                    const auto addr = m_registers.GetHl();
                    m_registers.SetHl(addr + 1);
                    return addr;
                }
                case AddrHlD: {
                    const auto addr = m_registers.GetHl();
                    m_registers.SetHl(addr - 1);
                    return addr;
                }
                default:
                    throw PlipEmulationException("BUG: Attempted to resolve a pointer using an out of range index.");
            }
        }

        uint8_t PopByte() { return ReadMemory(m_registers.SP++); }
        void PushByte(const uint8_t val) { WriteMemory(--m_registers.SP, val); }

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
