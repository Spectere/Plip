/* Mos6502Registers.h
 *
 * The registers struct used by the MOS 6502 CPU implementation.
 */

#pragma once

#include <cstdint>

#include "PlipSupport.h"

namespace Plip::Cpu {
    struct Mos6502Registers {
        static constexpr int CarryFlagBit = 0;
        static constexpr int ZeroFlagBit = 1;
        static constexpr int InterruptDisableBit = 2;
        static constexpr int DecimalModeBit = 3;
        static constexpr int BreakCommandBit = 4;
        static constexpr int OverflowFlagBit = 6;
        static constexpr int NegativeFlagBit = 7;

        uint8_t A {};
        uint8_t P = 0b00100000;
        uint8_t X {};
        uint8_t Y {};

        uint16_t PC {};
        uint8_t S {};

        void ClearBreakCommand() { P = BIT_CLEAR(P, BreakCommandBit); }
        [[nodiscard]] bool GetBreakCommand() const { return BIT_TEST(P, BreakCommandBit); }
        void SetBreakCommand() { P = BIT_SET(P, BreakCommandBit); }

        void ClearCarryFlag() { P = BIT_CLEAR(P, CarryFlagBit); }
        [[nodiscard]] bool GetCarryFlag() const { return BIT_TEST(P, CarryFlagBit); }
        void SetCarryFlag() { P = BIT_SET(P, CarryFlagBit); }
        void FlipCarryFlag() { P = BIT_FLIP(P, CarryFlagBit); }

        void ClearDecimalMode() { P = BIT_CLEAR(P, DecimalModeBit); }
        [[nodiscard]] bool GetDecimalMode() const { return BIT_TEST(P, DecimalModeBit); }
        void SetDecimalMode() { P = BIT_SET(P, DecimalModeBit); }

        void ClearInterruptDisable() { P = BIT_CLEAR(P, InterruptDisableBit); }
        [[nodiscard]] bool GetInterruptDisable() const { return BIT_TEST(P, InterruptDisableBit); }
        void SetInterruptDisable() { P = BIT_SET(P, InterruptDisableBit); }

        void ClearNegativeFlag() { P = BIT_CLEAR(P, NegativeFlagBit); }
        [[nodiscard]] bool GetNegativeFlag() const { return BIT_TEST(P, NegativeFlagBit); }
        void SetNegativeFlag() { P = BIT_SET(P, NegativeFlagBit); }

        void ClearOverflowFlag() { P = BIT_CLEAR(P, OverflowFlagBit); }
        [[nodiscard]] bool GetOverflowFlag() const { return BIT_TEST(P, OverflowFlagBit); }
        void SetOverflowFlag() { P = BIT_SET(P, OverflowFlagBit); }

        void ClearZeroFlag() { P = BIT_CLEAR(P, ZeroFlagBit); }
        [[nodiscard]] bool GetZeroFlag() const { return BIT_TEST(P, ZeroFlagBit); }
        void SetZeroFlag() { P = BIT_SET(P, ZeroFlagBit); }
    };
}
