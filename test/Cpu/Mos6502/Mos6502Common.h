/* SharpLr35902Common.h
 *
 * Part of the test fixture for the MOS 6502 CPU.
 * This file functions as a common header among the tests for all 6502 instruction groups.
 */

#pragma once

#include "../MockCpu.h"

#include "Cpu/Mos6502/Mos6502.h"

using namespace Plip;

#define TEST_NAME(name) "[MOS 6502 CPU] " name
#define OP(op) "[mos6502cpu][" op "]"
#define TEST(name, op) TEST_CASE_METHOD(MockCpu<MockMos6502>, TEST_NAME(name), OP(op))

#define CHECK_A(val) CHECK(cpu->GetRegisterA() == val);
#define CHECK_X(val) CHECK(cpu->GetRegisterX() == val);
#define CHECK_Y(val) CHECK(cpu->GetRegisterY() == val);
#define CHECK_S(val) CHECK(cpu->GetRegisterS() == val);
#define CHECK_PC(val) CHECK(cpu->GetPc() == val);

#define CHECK_BREAK_CLEAR CHECK(cpu->GetBreakCommand() == false);
#define CHECK_BREAK_SET CHECK(cpu->GetBreakCommand() == true);

#define CHECK_CARRY_CLEAR CHECK(cpu->GetCarryFlag() == false);
#define CHECK_CARRY_SET CHECK(cpu->GetCarryFlag() == true);

#define CHECK_DECIMAL_MODE_DISABLED CHECK(cpu->GetDecimalMode() == false);
#define CHECK_DECIMAL_MODE_ENABLED CHECK(cpu->GetDecimalMode() == true);

#define CHECK_INTERRUPT_DISABLED CHECK(cpu->GetInterruptDisable() == false);
#define CHECK_INTERRUPT_ENABLED CHECK(cpu->GetInterruptDisable() == true);

#define CHECK_NEGATIVE_CLEAR CHECK(cpu->GetNegativeFlag() == false);
#define CHECK_NEGATIVE_SET CHECK(cpu->GetNegativeFlag() == true);

#define CHECK_OVERFLOW_CLEAR CHECK(cpu->GetOverflowFlag() == false);
#define CHECK_OVERFLOW_SET CHECK(cpu->GetOverflowFlag() == true);

#define CHECK_ZERO_CLEAR CHECK(cpu->GetZeroFlag() == false);
#define CHECK_ZERO_SET CHECK(cpu->GetZeroFlag() == true);

class MockMos6502 final : public Cpu::Mos6502 {
public:
    MockMos6502(const long hz, PlipMemoryMap* memoryMap) : Mos6502(hz, memoryMap, Cpu::Mos6502Version::Mos6502) {
        // Set reset vector to 0x200.
        memoryMap->SetByte(0xFFFD, 0x02);
        memoryMap->SetByte(0xFFFC, 0x00);
    }

    void SetVersion(const Cpu::Mos6502Version version) { m_version = version; }

    [[nodiscard]] uint8_t GetRegisterA() const { return m_registers.A; }
    void SetRegisterA(const uint8_t value) { m_registers.A = value; }

    [[nodiscard]] uint8_t GetRegisterX() const { return m_registers.X; }
    void SetRegisterX(const uint8_t value) { m_registers.X = value; }

    [[nodiscard]] uint8_t GetRegisterY() const { return m_registers.Y; }
    void SetRegisterY(const uint8_t value) { m_registers.Y = value; }

    [[nodiscard]] uint8_t GetRegisterS() const { return m_registers.S; }
    void SetRegisterS(const uint8_t value) { m_registers.S = value; }

    void ClearBreakCommand() { m_registers.ClearBreakCommand(); }
    [[nodiscard]] bool GetBreakCommand() const { return m_registers.GetBreakCommand(); }
    void SetBreakCommand() { m_registers.SetBreakCommand(); }

    void ClearCarryFlag() { m_registers.ClearCarryFlag(); }
    [[nodiscard]] bool GetCarryFlag() const { return m_registers.GetCarryFlag(); }
    void SetCarryFlag() { m_registers.SetCarryFlag(); }

    void ClearDecimalMode() { m_registers.ClearDecimalMode(); }
    [[nodiscard]] bool GetDecimalMode() const { return m_registers.GetDecimalMode(); }
    void SetDecimalMode() { m_registers.SetDecimalMode(); }

    void ClearInterruptDisable() { m_registers.ClearInterruptDisable(); }
    [[nodiscard]] bool GetInterruptDisable() const { return m_registers.GetInterruptDisable(); }
    void SetInterruptDisable() { m_registers.SetInterruptDisable(); }

    void ClearNegativeFlag() { m_registers.ClearNegativeFlag(); }
    [[nodiscard]] bool GetNegativeFlag() const { return m_registers.GetNegativeFlag(); }
    void SetNegativeFlag() { m_registers.SetNegativeFlag(); }

    void ClearOverflowFlag() { m_registers.ClearOverflowFlag(); }
    [[nodiscard]] bool GetOverflowFlag() const { return m_registers.GetOverflowFlag(); }
    void SetOverflowFlag() { m_registers.SetOverflowFlag(); }

    void ClearZeroFlag() { m_registers.ClearZeroFlag(); }
    [[nodiscard]] bool GetZeroFlag() const { return m_registers.GetZeroFlag(); }
    void SetZeroFlag() { m_registers.SetZeroFlag(); }
};
