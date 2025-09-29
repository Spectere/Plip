/* Mos6502.System.cpp
 *
 * Tests the CPU core's ability to handle unofficial/illegal opcodes.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

TEST("Unofficial-WDC65C02", "Unofficial-WDC65C02") {
    // The WDC 65C02 should halt (i.e. the core should throw an exception) if *any* of these are used.
    // TODO: As WDC 65C02 support is added, implemented opcodes should be removed from this list.
    constexpr int opCount = 105;
    
    LoadData(0x200, {
              0x02, 0x03, 0x04, 0x07,             0x0B, 0x0C,       0x0F,
              0x12, 0x13, 0x14, 0x17,       0x1A, 0x1B, 0x1C,       0x1F,
              0x22, 0x23,       0x27,             0x2B,             0x2F,
              0x32, 0x33, 0x34, 0x37,       0x3A, 0x3B, 0x3C,       0x3F,
              0x42, 0x43, 0x44, 0x47,             0x4B,             0x4F,
              0x52, 0x53, 0x54, 0x57,       0x5A, 0x5B, 0x5C,       0x5F,
              0x62, 0x63, 0x64, 0x67,             0x6B,             0x6F,
              0x72, 0x73, 0x74, 0x77,       0x7A, 0x7B, 0x7C,       0x7F,
        0x80, 0x82, 0x83,       0x87, 0x89,       0x8B,             0x8F,
              0x92, 0x93,       0x97,             0x9B, 0x9C, 0x9E, 0x9F,
                    0xA3,       0xA7,             0xAB,             0xAF,
              0xB2, 0xB3,       0xB7,             0xBB,             0xBF,
              0xC2, 0xC3,       0xC7,             0xCB,             0xCF,
              0xD2, 0xD3, 0xD4, 0xD7,       0xDA, 0xDB, 0xDC,       0xDF,
              0xE2, 0xE3,       0xE7,             0xEB,             0xEF,
              0xF2, 0xF3, 0xF4, 0xF7,       0xFA, 0xFB, 0xFC,       0xFF,
    });

    cpu->SetVersion(Cpu::Mos6502Version::Wdc65C02);

    for(auto i = 0; i < opCount; i++) {
        CHECK_THROWS(cpu->Step());
    }
}

//
// Unofficial NOPs
//

// NOP (2 cycle, 1 byte)
TEST("Unofficial-NOP", "Unofficial-NOP") {
    LoadData(0x200, {
        0x1A, 0x3A, 0x5A, 0x7A, 0xDA, 0xFA,
    });

    for(auto i = 0; i < 6; i++) {
        constexpr int expectedCycles = 2;
        EXECUTE(expectedCycles);
    }
}

// SKB (2 cycles, 2 bytes)
TEST("Unofficial-SKB", "Unofficial-SKB") {
    LoadData(0x200, {
        0x80, 0x00,
        0x82, 0x00,
        0x89, 0x00,
        0xC2, 0x00,
        0xE2, 0x00,
    });

    for(auto i = 0; i < 5; i++) {
        constexpr int expectedCycles = 2;
        EXECUTE(expectedCycles);
    }
}

// IGN zp (3 cycles, 2 bytes)
TEST("Unofficial-IGN zp", "Unofficial-IGN-zp") {
    LoadData(0x200, {
        0x04, 0x00,
        0x44, 0x00,
        0x64, 0x00,
    });

    for(auto i = 0; i < 3; i++) {
        constexpr int expectedCycles = 3;
        EXECUTE(expectedCycles);
    }
}

// IGN zp, X (4 cycles, 2 bytes)
TEST("Unofficial-IGN zp, X", "Unofficial-IGN-zp,X") {
    LoadData(0x200, {
        0x14, 0x00,
        0x34, 0x00,
        0x54, 0x00,
        0x74, 0x00,
        0xD4, 0x00,
        0xF4, 0x00,
    });

    for(auto i = 0; i < 6; i++) {
        constexpr int expectedCycles = 4;
        EXECUTE(expectedCycles);
    }
}

// IGN abs16 (4 cycles, 3 bytes)
TEST("Unofficial-IGN abs16", "Unofficial-IGN-abs16") {
    LoadData(0x200, {
        0x0C, 0x10, 0x20,
    });

    constexpr int expectedCycles = 4;
    EXECUTE(expectedCycles);
}

// IGN abs16, X (4-5 cycles, 2 bytes)
TEST("Unofficial-IGN abs16, X", "Unofficial-IGN-abs16,X") {
    LoadData(0x200, {
        0x1C, 0x34, 0x12,
        0x3C, 0x34, 0x12,
        0x5C, 0x34, 0x12,
        0x7C, 0x34, 0x12,
        0xDC, 0x34, 0x12,
        0xFC, 0x34, 0x12,

        0x1C, 0xF0, 0x20,
        0x3C, 0xF0, 0x20,
        0x5C, 0xF0, 0x20,
        0x7C, 0xF0, 0x20,
        0xDC, 0xF0, 0x20,
        0xFC, 0xF0, 0x20,
    });

    for(auto i = 0; i < 6; i++) {
        constexpr int expectedCycles = 4;
        EXECUTE(expectedCycles);
    }

    // Crossing the page boundary adds a cycle.
    cpu->SetRegisterX(0x80);
    for(auto i = 0; i < 6; i++) {
        constexpr int expectedCycles = 5;
        EXECUTE(expectedCycles);
    }
}

//
// SBC (Duplicate)
//
TEST("Unofficial-SBC imm8", "Unofficial-SBC-imm8") {  // 0xEB
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0xEB, 0b00000001,  // C+ Z- V- N-
        0xEB, 0b00000001,  // C- Z- V- N+
        0xEB, 0b00000001,  // C+ Z- V+ N-
        0xEB, 0b00000001,  // C+ Z+ V- N-
        0xEB, 0b11111111,  // C+ Z+ V- N-
        0xEB, 0b00000000,  // C+ Z- V- N-
        0xEB, 0b00000000,  // C- Z- V- N+
        0xEB, 0b00000000,  // C+ Z- V+ N-
        0xEB, 0b00000000,  // C+ Z+ V- N-
        0xEB, 0b11111111,  // C- Z- V- N+
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-SBC imm8-BCD", "Unofficial-SBC-imm8-BCD") {  // 0xE9, with decimal mode enabled
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0xEB, 0b00000001,  // C+ Z- V- N-
        0xEB, 0b00000001,  // C+ Z- V+ N-
        0xEB, 0b00000001,  // C+ Z- V- N+
        0xEB, 0b00000001,  // C- Z- V- N+
        0xEB, 0b00000001,  // C+ Z- V- N+
        0xEB, 0b00000000,  // C+ Z- V- N-
        0xEB, 0b00000000,  // C+ Z- V+ N-
        0xEB, 0b00000000,  // C+ Z- V- N+
        0xEB, 0b00000000,  // C- Z- V- N+
        0xEB, 0b00000000,  // C+ Z- V- N+
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial SBC imm8-BCD-2A03", "Unofficial-SBC-imm8-BCD-2A03") {  // 0xEB, with no BCD support
    constexpr int expectedCycles = 2;

    cpu->SetVersion(Cpu::Mos6502Version::Ricoh2A03);

    LoadData(0x200, {
        0xEB, 0b00000001,  // C+ Z- V- N-
        0xEB, 0b00000001,  // C+ Z- V+ N-
        0xEB, 0b00000001,  // C+ Z- V- N+
        0xEB, 0b00000001,  // C- Z- V- N+
        0xEB, 0b00000001,  // C+ Z- V- N+
        0xEB, 0b00000000,  // C+ Z- V- N-
        0xEB, 0b00000000,  // C+ Z- V+ N-
        0xEB, 0b00000000,  // C+ Z- V- N+
        0xEB, 0b00000000,  // C- Z- V- N+
        0xEB, 0b00000000,  // C+ Z- V- N+
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// ALR/ASR
//
TEST("Unofficial-ALR imm8", "Unofficial-ALR-imm8") {  // 0x4B
    constexpr int expectedCycles = 2;
    
    LoadData(0x200, {
        0x4B, 0b00001111,  // C- Z- N-
        0x4B, 0b00001011,  // C+ Z- N-
        0x4B, 0b00001011,  // C- Z+ N-
        0x4B, 0b00000011,  // C+ Z+ N-
    });

    cpu->SetRegisterA(0b11111100);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000110);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000100);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000100);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

//
// ANC
//
TEST("Unofficial-ANC imm8", "Unofficial-ANC-imm8") {  // 0x0B/0x2B
    LoadData(0x200, {
        0x0B, 0b00011000,  // C- Z- N-
        0x0B, 0b11110000,  // C- Z+ N-
        0x0B, 0b10101010,  // C+ Z- N+

        0x2B, 0b00011000,  // C- Z- N-
        0x2B, 0b11110000,  // C- Z+ N-
        0x2B, 0b10101010,  // C+ Z- N+
    });

    for(auto i = 0; i < 2; i++) {
        constexpr int expectedCycles = 2;

        cpu->SetRegisterA(0b11110000);
        EXECUTE(expectedCycles);
        CHECK_A(0b00010000);
        CHECK_CARRY_CLEAR;
        CHECK_ZERO_CLEAR;
        CHECK_NEGATIVE_CLEAR;
        
        cpu->SetRegisterA(0b00001111);
        EXECUTE(expectedCycles);
        CHECK_A(0b00000000);
        CHECK_CARRY_CLEAR;
        CHECK_ZERO_SET;
        CHECK_NEGATIVE_CLEAR;

        cpu->SetRegisterA(0b11110000);
        EXECUTE(expectedCycles);
        CHECK_A(0b10100000);
        CHECK_CARRY_SET;
        CHECK_ZERO_CLEAR;
        CHECK_NEGATIVE_SET;
    }
}

//
// ARR
//
TEST("Unofficial-ARR imm8", "Unofficial-ARR-imm8") {  // 0x6B
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0x6B, 0b11110000,  // C- V- N- Z-
        0x6B, 0b10000000,  // C+ V+ N- Z-
        0x6B, 0b01000000,  // C- V+ N- Z-
        0x6B, 0b11000000,  // C+ V- N- Z-
        0x6B, 0b00000000,  // C- V- N- Z+
        0x6B, 0b00000000,  // C- V- N+ Z-
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00011000);
    CHECK_CARRY_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01000000);
    CHECK_CARRY_SET;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_CARRY_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100000);
    CHECK_CARRY_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
}

//
// AXS
//
TEST("Unofficial-AXS imm8", "Unofficial-AXS-imm8") {  // 0xCB
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0xCB, 0b00000001,  // C- Z- N-
        0xCB, 0b11111110,  // C+ Z- N-
        0xCB, 0b00001111,  // C- Z+ N-
        0xCB, 0b00001110,  // C- Z- N+
    });

    cpu->SetRegisterA(0b10101111);
    cpu->SetRegisterX(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_X(0b00001110);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10101111);
    cpu->SetRegisterX(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_X(0b00010001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10101111);
    cpu->SetRegisterX(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_X(0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10101111);
    cpu->SetRegisterX(0b10001111);
    EXECUTE(expectedCycles);
    CHECK_X(0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// LAX
//
TEST("Unofficial-LAX zp", "Unofficial-LAX-zp") {  // 0xA7
    constexpr int expectedCycles = 3;

    LoadData(0x200, {
        0xA7, 0x80,  // N- Z-
        0xA7, 0x81,  // N- Z+
        0xA7, 0x82,  // N+ Z-
    });

    LoadData(0x80, {
        0x12, 0x00, 0x88,
    });

    EXECUTE(expectedCycles);
    CHECK_A(0x12);
    CHECK_X(0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_X(0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    EXECUTE(expectedCycles);
    CHECK_A(0x88);
    CHECK_X(0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
}

TEST("Unofficial-LAX zp, Y", "Unofficial-LAX-zp,Y") {  // 0xB7
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xB7, 0x80,  // Y: $00, N- Z-
        0xB7, 0x80,  // Y: $01, N- Z+
        0xB7, 0x80,  // Y: $02, N+ Z-
    });

    LoadData(0x80, {
        0x12, 0x00, 0x88,
    });

    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0x12);
    CHECK_X(0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_X(0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0x88);
    CHECK_X(0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
}

TEST("Unofficial-LAX abs16", "Unofficial-LAX-abs16") {  // 0xAF
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xAF, 0x34, 0x12,  // N- Z-
        0xAF, 0x35, 0x12,  // N- Z+
        0xAF, 0x36, 0x12,  // N+ Z-
    });

    LoadData(0x1234, {
        0x12, 0x00, 0x88,
    });

    EXECUTE(expectedCycles);
    CHECK_A(0x12);
    CHECK_X(0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_X(0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    EXECUTE(expectedCycles);
    CHECK_A(0x88);
    CHECK_X(0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
}

TEST("Unofficial-LAX abs16, Y", "Unofficial-LAX-abs,Y") {  // 0xBF
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xBF, 0x34, 0x12,  // Y: $00, N- Z-
        0xBF, 0x34, 0x12,  // Y: $01, N- Z+
        0xBF, 0x34, 0x12,  // Y: $02, N+ Z-

        0xBF, 0xF0, 0x20,  // +1 Y: $30, N- Z-
        0xBF, 0xF0, 0x20,  // +1 Y: $31, N- Z+
        0xBF, 0xF0, 0x20,  // +1 Y: $32, N+ Z-
    });

    LoadData(0x1234, {
        0x12, 0x00, 0x88,
    });

    LoadData(0x2120, {
        0x12, 0x00, 0x88,
    });

    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0x12);
    CHECK_X(0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_X(0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0x88);
    CHECK_X(0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x30);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0x12);
    CHECK_X(0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x31);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0x00);
    CHECK_X(0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x32);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0x88);
    CHECK_X(0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
}

TEST("Unofficial-LAX (imm8, X)", "Unofficial-LAX-(imm8, X)") {  // 0xA3
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xA3, 0x80,  // X: $00, N- Z-
        0xA3, 0x80,  // X: $02, N- Z+
        0xA3, 0x80,  // X: $04, N+ Z-

        0xA3, 0xF0,  // X: $20, N- Z-
        0xA3, 0xF0,  // X: $22, N- Z+
        0xA3, 0xF0,  // X: $24, N+ Z-
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
    });

    LoadData(0x10, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
    });

    LoadData(0x1234, {
        0x12, 0x00, 0x88,
    });

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0x12);
    CHECK_X(0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_X(0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0x88);
    CHECK_X(0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_A(0x12);
    CHECK_X(0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x22);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_X(0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterX(0x24);
    EXECUTE(expectedCycles);
    CHECK_A(0x88);
    CHECK_X(0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
}

TEST("Unofficial-LAX (imm8), Y", "Unofficial-LAX-(imm8),Y") {  // 0xB3
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0xB3, 0x80,  // Y: $00, N- Z-
        0xB3, 0x80,  // Y: $01, N- Z+
        0xB3, 0x80,  // Y: $02, N+ Z-

        0xB3, 0x82,  // +1 Y: $80, N- Z-
        0xB3, 0x82,  // +1 Y: $81, N- Z+
        0xB3, 0x82,  // +1 Y: $82, N+ Z-
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC0, 0x11,
    });
    
    LoadData(0x1240, {
        0x12, 0x00, 0x88,
    });

    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0x12);
    CHECK_X(0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_X(0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0x88);
    CHECK_X(0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x80);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0x12);
    CHECK_X(0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x81);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0x00);
    CHECK_X(0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x82);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0x88);
    CHECK_X(0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
}

//
// SAX
//
TEST("Unofficial-SAX zp", "Unofficial-SAX-zp") {  // 0x87
    constexpr int expectedCycles = 3;

    LoadData(0x200, {
        0x87, 0x80,
    });

    cpu->SetRegisterA(0b11110000);
    cpu->SetRegisterX(0b10101010);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x80, 0b10100000);
}

TEST("Unofficial-SAX zp, Y", "Unofficial-SAX-zp,Y") {  // 0x97
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0x97, 0x80,
    });

    cpu->SetRegisterA(0b11110000);
    cpu->SetRegisterX(0b10101010);
    cpu->SetRegisterY(0x10);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x90, 0b10100000);
}

TEST("Unofficial-SAX abs16", "Unofficial-SAX-abs16") {  // 0x8F
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0x8F, 0x34, 0x12,
    });

    cpu->SetRegisterA(0b11110000);
    cpu->SetRegisterX(0b10101010);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1234, 0b10100000);
}

TEST("Unofficial-SAX (imm8, X)", "Unofficial-SAX-(imm8,X)") {  // 0x83
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x83, 0x80,  // X: $02
    });

    LoadData(0x80, {
        0xFE, 0xED,
        0x34, 0x12,
    });

    cpu->SetRegisterA(0b11111111);
    cpu->SetRegisterX(0b00000010);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1234, 0b00000010);
}
