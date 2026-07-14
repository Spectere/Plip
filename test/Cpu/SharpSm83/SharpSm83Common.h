/* SharpSm83Common.h
 *
 * The test fixture for Sharp SM83-based CPUs.
 */

#pragma once

#include "../MockCpu.h"

#include "Cpu/SharpSm83/SharpSm83.h"

using namespace Plip;

#define TEST_NAME(name) "[Sharp SM83 CPU] " name
#define OP(op) "[sm83cpu][" op "]"
#define TEST(name, op) TEST_CASE_METHOD(MockCpu<MockSharpSm83>, TEST_NAME(name), OP(op))

#define CHECK_CARRY_CLEAR CHECK(cpu->GetCarryFlag() == false)
#define CHECK_CARRY_SET CHECK(cpu->GetCarryFlag() == true)

#define CHECK_HALF_CLEAR CHECK(cpu->GetHalfCarryFlag() == false)
#define CHECK_HALF_SET CHECK(cpu->GetHalfCarryFlag() == true)

#define CHECK_SUBTRACT_CLEAR CHECK(cpu->GetSubtractFlag() == false)
#define CHECK_SUBTRACT_SET CHECK(cpu->GetSubtractFlag() == true)

#define CHECK_ZERO_CLEAR CHECK(cpu->GetZeroFlag() == false)
#define CHECK_ZERO_SET CHECK(cpu->GetZeroFlag() == true)

#ifdef EXECUTE
#undef EXECUTE
#endif // EXECUTE

// SM83 core can no longer return the cycle count.
#define EXECUTE(expectedCycles) { for(auto i = 0; i < expectedCycles; ++i) { cpu->Cycle(); } };

class MockSharpSm83 final : public Cpu::SharpSm83 {
public:
    static constexpr int RegIndexB  = 0b000;
    static constexpr int RegIndexC  = 0b001;
    static constexpr int RegIndexD  = 0b010;
    static constexpr int RegIndexE  = 0b011;
    static constexpr int RegIndexH  = 0b100;
    static constexpr int RegIndexL  = 0b101;
    static constexpr int RegIndexHl = 0b110;
    static constexpr int RegIndexA  = 0b111;

    MockSharpSm83(const long hz, PlipMemoryMap* memoryMap) : SharpSm83(hz, memoryMap, false) { }

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

    [[nodiscard]] Cpu::SharpSm83ImeState GetImeState() const { return m_ime; }
    void SetImeState(const Cpu::SharpSm83ImeState newState) { m_ime = newState; }

    [[nodiscard]] bool GetGbcMode() const { return m_gbcMode; }
    void SetGbcMode(const bool value) { m_gbcMode = value; }

    [[nodiscard]] bool GetHalt() const { return m_halt; }
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
