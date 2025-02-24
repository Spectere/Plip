/* SharpLr35902Common.h
 *
 * The test fixture for the Sharp LR35902 CPU.
 */

#pragma once

#include "../MockCpu.h"

#include "Cpu/SharpLr35902/SharpLr35902.h"

using namespace Plip;

#define TEST_NAME(name) "[Sharp LR35902 CPU] " name
#define OP(op) "[lr35902cpu][" op "]"
#define TEST(name, op) TEST_CASE_METHOD(MockCpu<MockSharpLr35902>, TEST_NAME(name), OP(op))

#define CHECK_CARRY_CLEAR CHECK(cpu->GetCarryFlag() == false)
#define CHECK_CARRY_SET CHECK(cpu->GetCarryFlag() == true)

#define CHECK_HALF_CLEAR CHECK(cpu->GetHalfCarryFlag() == false)
#define CHECK_HALF_SET CHECK(cpu->GetHalfCarryFlag() == true)

#define CHECK_SUBTRACT_CLEAR CHECK(cpu->GetSubtractFlag() == false)
#define CHECK_SUBTRACT_SET CHECK(cpu->GetSubtractFlag() == true)

#define CHECK_ZERO_CLEAR CHECK(cpu->GetZeroFlag() == false)
#define CHECK_ZERO_SET CHECK(cpu->GetZeroFlag() == true)

class MockSharpLr35902 final : public Cpu::SharpLr35902 {
public:
    static constexpr int RegIndexB  = 0b000;
    static constexpr int RegIndexC  = 0b001;
    static constexpr int RegIndexD  = 0b010;
    static constexpr int RegIndexE  = 0b011;
    static constexpr int RegIndexH  = 0b100;
    static constexpr int RegIndexL  = 0b101;
    static constexpr int RegIndexHl = 0b110;
    static constexpr int RegIndexA  = 0b111;

    MockSharpLr35902(const long hz, PlipMemoryMap* memoryMap) : SharpLr35902(hz, memoryMap) { }

    void ClearCarryFlag() { m_registers.ClearCarryFlag(); }
    [[nodiscard]] bool GetCarryFlag() const { return m_registers.GetCarryFlag(); }
    void SetCarryFlag() { return m_registers.SetCarryFlag(); }

    void ClearSubtractFlag() { m_registers.ClearSubtractFlag(); }
    [[nodiscard]] bool GetSubtractFlag() const { return m_registers.GetSubtractFlag(); }
    void SetSubtractFlag() { return m_registers.SetSubtractFlag(); }

    void ClearHalfCarryFlag() { m_registers.ClearHalfCarryFlag(); }
    [[nodiscard]] bool GetHalfCarryFlag() const { return m_registers.GetHalfCarryFlag(); }
    void SetHalfCarryFlag() { return m_registers.SetHalfCarryFlag(); }

    void ClearZeroFlag() { m_registers.ClearZeroFlag(); }
    [[nodiscard]] bool GetZeroFlag() const { return m_registers.GetZeroFlag(); }
    void SetZeroFlag() { return m_registers.SetZeroFlag(); }

    Cpu::SharpLr35902ImeState GetImeState() const { return m_ime; }
    void SetImeState(const Cpu::SharpLr35902ImeState newState) { m_ime = newState; }

    bool GetHalt() const { return m_halt; }
    void SetHalt(const bool value) { m_halt = value; }

    [[nodiscard]] uint16_t GetAf() const { return m_registers.GetAf(); }
    [[nodiscard]] uint16_t GetBc() const { return m_registers.GetBc(); }
    [[nodiscard]] uint16_t GetDe() const { return m_registers.GetDe(); }
    [[nodiscard]] uint16_t GetHl() const { return m_registers.GetHl(); }
    [[nodiscard]] uint16_t GetSp() const { return m_registers.SP; }

    [[nodiscard]] uint8_t GetRegister8ByIndex(const int index) {
        return m_registers.Get8ByIndex(index);
    }

    void SetRegister8ByIndex(const int index, const uint8_t value) {
        m_registers.Set8ByIndex(index, value);
    }

    void SetBc(const uint16_t value) { m_registers.SetBc(value); }
    void SetDe(const uint16_t value) { m_registers.SetDe(value); }
    void SetHl(const uint16_t value) { m_registers.SetHl(value); }
    void SetSp(const uint16_t value) { m_registers.SP = value; }
};
