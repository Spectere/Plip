/* SharpLr35902Registers.h
 *
 * The registers struct used by the Sharp LR35902 CPU implementation.
 */

#pragma once

#include <cstdint>

#include "../../PlipSupport.h"

namespace Plip::Cpu {
    struct SharpLr35902Registers {
        static constexpr int CarryFlagBit = 4;
        static constexpr int HalfCarryFlagBit = 5;
        static constexpr int SubtractFlagBit = 6;
        static constexpr int ZeroFlagBit = 7;

        uint8_t A;
        uint8_t F;
        uint8_t B;
        uint8_t C;
        uint8_t D;
        uint8_t E;
        uint8_t H;
        uint8_t L;

        uint16_t SP;
        uint16_t PC;

        void ClearCarryFlag() { BIT_CLEAR(F, CarryFlagBit); }
        [[nodiscard]] bool GetCarryFlag() const { return BIT_TEST(F, CarryFlagBit); }
        void SetCarryFlag() { BIT_SET(F, CarryFlagBit); }

        void ClearSubtractFlag() { BIT_CLEAR(F, SubtractFlagBit); }
        [[nodiscard]] bool GetSubtractFlag() const { return BIT_TEST(F, SubtractFlagBit); }
        void SetSubtractFlag() { BIT_SET(F, SubtractFlagBit); }

        void ClearHalfCarryFlag() { BIT_CLEAR(F, HalfCarryFlagBit); }
        [[nodiscard]] bool GetHalfCarryFlag() const { return BIT_TEST(F, HalfCarryFlagBit); }
        void SetHalfCarryFlag() { BIT_SET(F, HalfCarryFlagBit); }

        void ClearZeroFlag() { BIT_CLEAR(F, ZeroFlagBit); }
        [[nodiscard]] bool GetZeroFlag() const { return BIT_TEST(F, ZeroFlagBit); }
        void SetZeroFlag() { BIT_SET(F, ZeroFlagBit); }

        void AddBc(const uint16_t value) { Set16(B, C, GetBc() + value); }
        void AddDe(const uint16_t value) { Set16(D, E, GetDe() + value); }
        void AddHl(const uint16_t value) { Set16(H, L, GetHl() + value); }

        [[nodiscard]] uint16_t GetAf() const { return Get16(A, F); }
        [[nodiscard]] uint16_t GetBc() const { return Get16(B, C); }
        [[nodiscard]] uint16_t GetDe() const { return Get16(D, E); }
        [[nodiscard]] uint16_t GetHl() const { return Get16(H, L); }

        void SetHl(const uint16_t value) { Set16(H, L, value); }

    private:
        static uint16_t Get16(const uint8_t high, const uint8_t low) {
            return (static_cast<uint16_t>(high) << 8) | low;
        }

        static void Set16(uint8_t& high, uint8_t& low, const uint16_t value) {
            high = static_cast<uint8_t>((value & 0xFF00) >> 8);
            low = static_cast<uint8_t>(value & 0x00FF);
        }
    };
}
