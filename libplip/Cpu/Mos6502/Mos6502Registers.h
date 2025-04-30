/* Mos6502Registers.h
 *
 * The registers struct used by the MOS 6502 CPU implementation.
 */

#pragma once

#include <cstdint>

#include "../../PlipSupport.h"

namespace Plip::Cpu {
    struct Mos6502Registers {
        static constexpr int CarryFlagBit = 0;
        static constexpr int ZeroFlagBit = 1;
        static constexpr int InterruptDisableBit = 2;
        static constexpr int DecimalModeBit = 3;
        static constexpr int BreakCommandBit = 4;
        static constexpr int OverflowFlagBit = 6;
        static constexpr int NegativeFlagBit = 7;

        uint8_t A;
        uint8_t F = 0b00100000;
        uint8_t X;
        uint8_t Y;

        uint16_t PC;
        uint8_t S;

        void ClearBreakCommand() { F = BIT_CLEAR(F, BreakCommandBit); }
        [[nodiscard]] bool GetBreakCommand() const { return BIT_TEST(F, BreakCommandBit); }
        void SetBreakCommand() { F = BIT_SET(F, BreakCommandBit); }

        void ClearCarryFlag() { F = BIT_CLEAR(F, CarryFlagBit); }
        [[nodiscard]] bool GetCarryFlag() const { return BIT_TEST(F, CarryFlagBit); }
        void SetCarryFlag() { F = BIT_SET(F, CarryFlagBit); }

        void ClearDecimalMode() { F = BIT_CLEAR(F, DecimalModeBit); }
        [[nodiscard]] bool GetDecimalMode() const { return BIT_TEST(F, DecimalModeBit); }
        void SetDecimalMode() { F = BIT_SET(F, DecimalModeBit); }

        void ClearInterruptDisable() { F = BIT_CLEAR(F, InterruptDisableBit); }
        [[nodiscard]] bool GetInterruptDisable() const { return BIT_TEST(F, InterruptDisableBit); }
        void SetInterruptDisable() { F = BIT_SET(F, InterruptDisableBit); }

        void ClearNegativeFlag() { F = BIT_CLEAR(F, NegativeFlagBit); }
        [[nodiscard]] bool GetNegativeFlag() const { return BIT_TEST(F, NegativeFlagBit); }
        void SetNegativeFlag() { F = BIT_SET(F, NegativeFlagBit); }

        void ClearOverflowFlag() { F = BIT_CLEAR(F, OverflowFlagBit); }
        [[nodiscard]] bool GetOverflowFlag() const { return BIT_TEST(F, OverflowFlagBit); }
        void SetOverflowFlag() { F = BIT_SET(F, OverflowFlagBit); }

        void ClearZeroFlag() { F = BIT_CLEAR(F, ZeroFlagBit); }
        [[nodiscard]] bool GetZeroFlag() const { return BIT_TEST(F, ZeroFlagBit); }
        void SetZeroFlag() { F = BIT_SET(F, ZeroFlagBit); }
    };
}
