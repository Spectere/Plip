/* SharpLr35902Registers.h
 *
 * The registers struct used by the Sharp LR35902 CPU implementation.
 */

#pragma once

#include <cstdint>

#include "../../PlipEmulationException.h"
#include "../../PlipSupport.h"

namespace Plip::Cpu {
    struct SharpLr35902Registers {
        static constexpr int CarryFlagBit     = 4;
        static constexpr int HalfCarryFlagBit = 5;
        static constexpr int SubtractFlagBit  = 6;
        static constexpr int ZeroFlagBit      = 7;

        static constexpr int RegIndexB  = 0b000;
        static constexpr int RegIndexC  = 0b001;
        static constexpr int RegIndexD  = 0b010;
        static constexpr int RegIndexE  = 0b011;
        static constexpr int RegIndexH  = 0b100;
        static constexpr int RegIndexL  = 0b101;
        static constexpr int RegIndexHl = 0b110;
        static constexpr int RegIndexA  = 0b111;

        static constexpr int RegIndex16Bc = 0b00;
        static constexpr int RegIndex16De = 0b01;
        static constexpr int RegIndex16Hl = 0b10;
        static constexpr int RegIndex16Sp = 0b11;
        static constexpr int RegIndex16Af = 0b11;

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

        [[nodiscard]] uint8_t Get8ByIndex(const int index) {
            return *(GetRegisterPtr8ByIndex(index));
        }

        [[nodiscard]] uint16_t Get16ByIndex(const int index) const {
            switch(index) {
                case RegIndex16Bc: return GetBc();
                case RegIndex16De: return GetDe();
                case RegIndex16Hl: return GetHl();
                case RegIndex16Sp: return SP;
                default:
                    throw new PlipEmulationException("BUG: Attempted to get a 16-bit register with an out of range index.");
            }
        }

        void Set8ByIndex(const int index, const uint8_t value) {
            *(GetRegisterPtr8ByIndex(index)) = value;
        }

        void Set16ByIndex(const int index, const uint16_t value) {
            switch(index) {
                case RegIndex16Bc:
                    SetBc(value);
                    break;
                case RegIndex16De:
                    SetDe(value);
                    break;
                case RegIndex16Hl:
                    SetHl(value);
                    break;
                case RegIndex16Sp:
                    SP = value;
                    break;
                default:
                    throw new PlipEmulationException("BUG: Attempted to set a 16-bit register with an out of range index.");
            }
        }

        void SetAf(const uint16_t value) { Set16(A, F, value & 0xFFF0); }
        void SetBc(const uint16_t value) { Set16(B, C, value); }
        void SetDe(const uint16_t value) { Set16(D, E, value); }
        void SetHl(const uint16_t value) { Set16(H, L, value); }

    private:
        uint8_t* GetRegisterPtr8ByIndex(const int index) {
            switch(index) {
                case RegIndexA: return &A;
                case RegIndexB: return &B;
                case RegIndexC: return &C;
                case RegIndexD: return &D;
                case RegIndexE: return &E;
                case RegIndexH: return &H;
                case RegIndexL: return &L;
                default:
                    throw new PlipEmulationException("BUG: Attempted to get an 8-bit register with an out of range index.");
            }
        }

        static uint16_t Get16(const uint8_t high, const uint8_t low) {
            return (static_cast<uint16_t>(high) << 8) | low;
        }

        static void Set16(uint8_t& high, uint8_t& low, const uint16_t value) {
            high = static_cast<uint8_t>((value & 0xFF00) >> 8);
            low = static_cast<uint8_t>(value & 0x00FF);
        }
    };
}
