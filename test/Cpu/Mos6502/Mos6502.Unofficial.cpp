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
// KIL
//
TEST("Unofficial-KIL $02", "Unofficial-KIL-02") {  // 0x02
    LoadData(0x200, { 0x02, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $12", "Unofficial-KIL-12") {  // 0x12
    LoadData(0x200, { 0x12, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $22", "Unofficial-KIL-22") {  // 0x22
    LoadData(0x200, { 0x22, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $32", "Unofficial-KIL-32") {  // 0x32
    LoadData(0x200, { 0x32, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $42", "Unofficial-KIL-42") {  // 0x42
    LoadData(0x200, { 0x42, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $52", "Unofficial-KIL-52") {  // 0x52
    LoadData(0x200, { 0x52, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $62", "Unofficial-KIL-62") {  // 0x62
    LoadData(0x200, { 0x62, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $72", "Unofficial-KIL-72") {  // 0x72
    LoadData(0x200, { 0x72, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $92", "Unofficial-KIL-92") {  // 0x92
    LoadData(0x200, { 0x92, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $B2", "Unofficial-KIL-B2") {  // 0xB2
    LoadData(0x200, { 0xB2, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $D2", "Unofficial-KIL-D2") {  // 0xD2
    LoadData(0x200, { 0xD2, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
}

TEST("Unofficial-KIL $F2", "Unofficial-KIL-F2") {  // 0xF2
    LoadData(0x200, { 0xF2, 0xEA });
    CHECK_FALSE(cpu->CpuHasCrashed());
    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    EXECUTE(__LONG_MAX__);
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

//
// DCP
//
TEST("Unofficial-DCP zp", "Unofficial-DCP-zp") {  // 0xC7
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0xC7, 0x80,  // C+ Z- N-
        0xC7, 0x81,  // C+ Z+ N+
        0xC7, 0x82,  // C- Z- N+
    });

    LoadData(0x80, {
        0b00010000, 0b00100001, 0b01000000,
    });

    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x80, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x81, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x82, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-DCP zp, X", "Unofficial-DCP-zp,X") {  // 0xD7
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xD7, 0x80,  // X: $00, C+ Z- N-
        0xD7, 0x80,  // X: $01, C+ Z+ N+
        0xD7, 0x80,  // X: $02, C- Z- N+
    });

    LoadData(0x80, {
        0b00010000, 0b00100001, 0b01000000,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x80, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x81, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x82, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-DCP abs16", "Unofficial-DCP-abs16") {  // 0xCF
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xCF, 0x34, 0x12,  // C+ Z- N-
        0xCF, 0x35, 0x12,  // C+ Z+ N+
        0xCF, 0x36, 0x12,  // C- Z- N+
    });

    LoadData(0x1234, {
        0b00010000, 0b00100001, 0b01000000,
    });

    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1234, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1235, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1236, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-DCP abs16, X", "Unofficial-DCP-abs16, X") {  // 0xDF
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0xDF, 0x34, 0x12,  // X: $00, C+ Z- N-
        0xDF, 0x34, 0x12,  // X: $01, C+ Z+ N+
        0xDF, 0x34, 0x12,  // X: $02, C- Z- N+

        0xDF, 0xF0, 0x20,  // X: $30, C+ Z- N-
        0xDF, 0xF0, 0x20,  // X: $31, C+ Z+ N+
        0xDF, 0xF0, 0x20,  // X: $32, C- Z- N+
    });

    LoadData(0x1234, {
        0b00010000, 0b00100001, 0b01000000,
    });

    LoadData(0x2120, {
        0b00010000, 0b00100001, 0b01000000,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1234, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1235, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1236, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x2120, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x2121, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x2122, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-DCP abs16, Y", "Unofficial-DCP-abs16, Y") {  // 0xDB
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0xDB, 0x34, 0x12,  // Y: $00, C+ Z- N-
        0xDB, 0x34, 0x12,  // Y: $01, C+ Z+ N+
        0xDB, 0x34, 0x12,  // Y: $02, C- Z- N+

        0xDB, 0xF0, 0x20,  // Y: $30, C+ Z- N-
        0xDB, 0xF0, 0x20,  // Y: $31, C+ Z+ N+
        0xDB, 0xF0, 0x20,  // Y: $32, C- Z- N+
    });

    LoadData(0x1234, {
        0b00010000, 0b00100001, 0b01000000,
    });

    LoadData(0x2120, {
        0b00010000, 0b00100001, 0b01000000,
    });

    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1234, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1235, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1236, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x2120, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x2121, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x2122, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-DCP (imm8, X)", "Unofficial-DCP-(imm8,X)") {  // 0xC3
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0xC3, 0x80,  // X: $00, C+ Z- N-
        0xC3, 0x80,  // X: $02, C+ Z+ N+
        0xC3, 0x80,  // X: $04, C- Z- N+

        0xC3, 0xF0,  // X: $20, C+ Z- N-
        0xC3, 0xF0,  // X: $22, C+ Z+ N+
        0xC3, 0xF0,  // X: $24, C- Z- N+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
    });

    LoadData(0x10, {
        0x37, 0x12,
        0x38, 0x12,
        0x39, 0x12,
    });
    
    LoadData(0x1234, {
        0b00010000, 0b00100001, 0b01000000,
        0b00010000, 0b00100001, 0b01000000,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1234, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1235, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1236, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1237, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1238, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1239, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-DCP (imm8), Y", "Unofficial-DCP-(imm8),Y") {  // 0xD3
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0xD3, 0x80,  // Y: $00, C+ Z- N-
        0xD3, 0x80,  // Y: $01, C+ Z+ N+
        0xD3, 0x80,  // Y: $02, C- Z- N+

        0xD3, 0x82,  // Y: $80, C+ Z- N-
        0xD3, 0x82,  // Y: $81, C+ Z+ N+
        0xD3, 0x82,  // Y: $82, C- Z- N+
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC3, 0x11,
    });
    
    LoadData(0x1240, {
        0b00010000, 0b00100001, 0b01000000,
        0b00010000, 0b00100001, 0b01000000,
    });

    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1240, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1241, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1242, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1243, 0b00001111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1244, 0b00100000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00100000);
    CHECK_RAM(0x1245, 0b00111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// ISC
//
TEST("Unofficial-ISC zp", "Unofficial-ISC-zp") {  // 0xE7
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0xE7, 0x80,  // C+ Z- N-
        0xE7, 0x81,  // C+ Z- N-
        0xE7, 0x82,  // C- Z- N-
        0xE7, 0x83,  // C- Z- N+
        0xE7, 0x84,  // C- Z+ N-
        0xE7, 0x85,  // C+ Z- N+
    });

    LoadData(0x80, {
        0x0F, 0xFF, 0xFE, 0x3F, 0x27, 0x00,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x18);
    CHECK_RAM(0x80, 0x10);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x28);
    CHECK_RAM(0x81, 0x00);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x29);
    CHECK_RAM(0x82, 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0xE8);
    CHECK_RAM(0x83, 0x40);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_RAM(0x84, 0x28);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0xFF);
    EXECUTE(expectedCycles);
    CHECK_A(0xFE);
    CHECK_RAM(0x85, 0x01);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-ISC zp, X", "Unofficial-ISC-zp,X") {  // 0xF7
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xF7, 0x80,  // X: $00, C+ Z- N-
        0xF7, 0x80,  // X: $01, C+ Z- N-
        0xF7, 0x80,  // X: $02, C- Z- N-
        0xF7, 0x80,  // X: $03, C- Z- N+
        0xF7, 0x80,  // X: $04, C- Z+ N-
        0xF7, 0x80,  // X: $05, C+ Z- N+
    });

    LoadData(0x80, {
        0x0F, 0xFF, 0xFE, 0x3F, 0x27, 0x00,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x18);
    CHECK_RAM(0x80, 0x10);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x28);
    CHECK_RAM(0x81, 0x00);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x29);
    CHECK_RAM(0x82, 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x03);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0xE8);
    CHECK_RAM(0x83, 0x40);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_RAM(0x84, 0x28);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x05);
    cpu->SetRegisterA(0xFF);
    EXECUTE(expectedCycles);
    CHECK_A(0xFE);
    CHECK_RAM(0x85, 0x01);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-ISC abs16", "Unofficial-ISC-abs16") {  // 0xEF
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xEF, 0x34, 0x12,  // C+ Z- N-
        0xEF, 0x35, 0x12,  // C+ Z- N-
        0xEF, 0x36, 0x12,  // C- Z- N-
        0xEF, 0x37, 0x12,  // C- Z- N+
        0xEF, 0x38, 0x12,  // C- Z+ N-
        0xEF, 0x39, 0x12,  // C+ Z- N+
    });

    LoadData(0x1234, {
        0x0F, 0xFF, 0xFE, 0x3F, 0x27, 0x00,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x18);
    CHECK_RAM(0x1234, 0x10);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x28);
    CHECK_RAM(0x1235, 0x00);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x29);
    CHECK_RAM(0x1236, 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0xE8);
    CHECK_RAM(0x1237, 0x40);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_RAM(0x1238, 0x28);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0xFF);
    EXECUTE(expectedCycles);
    CHECK_A(0xFE);
    CHECK_RAM(0x1239, 0x01);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-ISC abs16, X", "Unofficial-ISC-abs16,X") {  // 0xFF
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0xFF, 0x34, 0x12,  // X: $00, C+ Z- N-
        0xFF, 0x34, 0x12,  // X: $01, C+ Z- N-
        0xFF, 0x34, 0x12,  // X: $02, C- Z- N-
        0xFF, 0x34, 0x12,  // X: $03, C- Z- N+
        0xFF, 0x34, 0x12,  // X: $04, C- Z+ N-
        0xFF, 0x34, 0x12,  // X: $05, C+ Z- N+
    });

    LoadData(0x1234, {
        0x0F, 0xFF, 0xFE, 0x3F, 0x27, 0x00,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x18);
    CHECK_RAM(0x1234, 0x10);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x28);
    CHECK_RAM(0x1235, 0x00);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x29);
    CHECK_RAM(0x1236, 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x03);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0xE8);
    CHECK_RAM(0x1237, 0x40);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_RAM(0x1238, 0x28);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x05);
    cpu->SetRegisterA(0xFF);
    EXECUTE(expectedCycles);
    CHECK_A(0xFE);
    CHECK_RAM(0x1239, 0x01);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-ISC abs16, Y", "Unofficial-ISC-abs16,Y") {  // 0xFB
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0xFB, 0x34, 0x12,  // Y: $00, C+ Z- N-
        0xFB, 0x34, 0x12,  // Y: $01, C+ Z- N-
        0xFB, 0x34, 0x12,  // Y: $02, C- Z- N-
        0xFB, 0x34, 0x12,  // Y: $03, C- Z- N+
        0xFB, 0x34, 0x12,  // Y: $04, C- Z+ N-
        0xFB, 0x34, 0x12,  // Y: $05, C+ Z- N+
    });

    LoadData(0x1234, {
        0x0F, 0xFF, 0xFE, 0x3F, 0x27, 0x00,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x18);
    CHECK_RAM(0x1234, 0x10);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x28);
    CHECK_RAM(0x1235, 0x00);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x29);
    CHECK_RAM(0x1236, 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x03);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0xE8);
    CHECK_RAM(0x1237, 0x40);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_RAM(0x1238, 0x28);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x05);
    cpu->SetRegisterA(0xFF);
    EXECUTE(expectedCycles);
    CHECK_A(0xFE);
    CHECK_RAM(0x1239, 0x01);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-ISC (zp, X)", "Unofficial-ISC-(zp,X)") {  // 0xE3
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0xE3, 0x80,  // X: $00, C+ Z- N-
        0xE3, 0x80,  // X: $02, C+ Z- N-
        0xE3, 0x80,  // X: $04, C- Z- N-
        0xE3, 0x80,  // X: $06, C- Z- N+
        0xE3, 0x80,  // X: $08, C- Z+ N-
        0xE3, 0x80,  // X: $0A, C+ Z- N+

        0xE3, 0xF0,  // X: $20, C+ Z- N-
        0xE3, 0xF0,  // X: $22, C+ Z- N-
        0xE3, 0xF0,  // X: $24, C- Z- N-
        0xE3, 0xF0,  // X: $26, C- Z- N+
        0xE3, 0xF0,  // X: $28, C- Z+ N-
        0xE3, 0xF0,  // X: $2A, C+ Z- N+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
        0x37, 0x12,
        0x38, 0x12,
        0x39, 0x12,
    });

    LoadData(0x10, {
        0x3A, 0x12,
        0x3B, 0x12,
        0x3C, 0x12,
        0x3D, 0x12,
        0x3E, 0x12,
        0x3F, 0x12,
    });

    LoadData(0x1234, {
        0x0F, 0xFF, 0xFE, 0x3F, 0x27, 0x00,
        0x0F, 0xFF, 0xFE, 0x3F, 0x27, 0x00,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x18);
    CHECK_RAM(0x1234, 0x10);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x28);
    CHECK_RAM(0x1235, 0x00);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x29);
    CHECK_RAM(0x1236, 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x06);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0xE8);
    CHECK_RAM(0x1237, 0x40);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x08);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_RAM(0x1238, 0x28);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x0A);
    cpu->SetRegisterA(0xFF);
    EXECUTE(expectedCycles);
    CHECK_A(0xFE);
    CHECK_RAM(0x1239, 0x01);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x18);
    CHECK_RAM(0x123A, 0x10);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x28);
    CHECK_RAM(0x123B, 0x00);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x29);
    CHECK_RAM(0x123C, 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x26);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0xE8);
    CHECK_RAM(0x123D, 0x40);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x28);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_RAM(0x123E, 0x28);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x2A);
    cpu->SetRegisterA(0xFF);
    EXECUTE(expectedCycles);
    CHECK_A(0xFE);
    CHECK_RAM(0x123F, 0x01);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-ISC (zp), Y", "Unofficial-ISC-(zp),Y") {  // 0xF3
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0xF3, 0x80,  // Y: $00, C+ Z- N-
        0xF3, 0x80,  // Y: $01, C+ Z- N-
        0xF3, 0x80,  // Y: $02, C- Z- N-
        0xF3, 0x80,  // Y: $03, C- Z- N+
        0xF3, 0x80,  // Y: $04, C- Z+ N-
        0xF3, 0x80,  // Y: $05, C+ Z- N+

        0xF3, 0x82,  // Y: $80, C+ Z- N-
        0xF3, 0x82,  // Y: $81, C+ Z- N-
        0xF3, 0x82,  // Y: $82, C- Z- N-
        0xF3, 0x82,  // Y: $83, C- Z- N+
        0xF3, 0x82,  // Y: $84, C- Z+ N-
        0xF3, 0x82,  // Y: $85, C+ Z- N+
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC6, 0x11,
    });

    LoadData(0x1240, {
        0x0F, 0xFF, 0xFE, 0x3F, 0x27, 0x00,
        0x0F, 0xFF, 0xFE, 0x3F, 0x27, 0x00,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x18);
    CHECK_RAM(0x1240, 0x10);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x28);
    CHECK_RAM(0x1241, 0x00);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x29);
    CHECK_RAM(0x1242, 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x03);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0xE8);
    CHECK_RAM(0x1243, 0x40);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_RAM(0x1244, 0x28);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x05);
    cpu->SetRegisterA(0xFF);
    EXECUTE(expectedCycles);
    CHECK_A(0xFE);
    CHECK_RAM(0x1245, 0x01);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x18);
    CHECK_RAM(0x1246, 0x10);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x28);
    CHECK_RAM(0x1247, 0x00);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x29);
    CHECK_RAM(0x1248, 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x83);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0xE8);
    CHECK_RAM(0x1249, 0x40);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x84);
    cpu->SetRegisterA(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0x00);
    CHECK_RAM(0x124A, 0x28);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x85);
    cpu->SetRegisterA(0xFF);
    EXECUTE(expectedCycles);
    CHECK_A(0xFE);
    CHECK_RAM(0x124B, 0x01);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// RLA
//
TEST("Unofficial-RLA zp", "Unofficial-RLA-zp") {  // 0x27
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x27, 0x80,  // C- Z- N-
        0x27, 0x81,  // C- Z- N-
        0x27, 0x82,  // C+ Z- N-
        0x27, 0x83,  // C- Z+ N-
        0x27, 0x84,  // C- Z- N+
    });

    LoadData(0x80, {
        0b00010001, 0b00010000, 0b10010001, 0b00001000, 0b01000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x80, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x81, 0b00100001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x82, 0b00100010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x83, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_RAM(0x84, 0b10000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-RLA zp, X", "Unofficial-RLA-zp,X") {  // 0x37
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x37, 0x80,  // X: $00, C- Z- N-
        0x37, 0x80,  // X: $01, C- Z- N-
        0x37, 0x80,  // X: $02, C+ Z- N-
        0x37, 0x80,  // X: $03, C- Z+ N-
        0x37, 0x80,  // X: $04, C- Z- N+
    });

    LoadData(0x80, {
        0b00010001, 0b00010000, 0b10010001, 0b00001000, 0b01000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x80, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x81, 0b00100001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x82, 0b00100010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x03);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x83, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_RAM(0x84, 0b10000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-RLA abs16", "Unofficial-RLA-abs16") {  // 0x2F
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x2F, 0x34, 0x12,  // X: $00, C- Z- N-
        0x2F, 0x35, 0x12,  // X: $01, C- Z- N-
        0x2F, 0x36, 0x12,  // X: $02, C+ Z- N-
        0x2F, 0x37, 0x12,  // X: $03, C- Z+ N-
        0x2F, 0x38, 0x12,  // X: $04, C- Z- N+
    });

    LoadData(0x1234, {
        0b00010001, 0b00010000, 0b10010001, 0b00001000, 0b01000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1234, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1235, 0b00100001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1236, 0b00100010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1237, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_RAM(0x1238, 0b10000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-RLA abs16, X", "Unofficial-RLA-abs16,X") {  // 0x3F
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x3F, 0x34, 0x12,  // X: $00, C- Z- N-
        0x3F, 0x34, 0x12,  // X: $01, C- Z- N-
        0x3F, 0x34, 0x12,  // X: $02, C+ Z- N-
        0x3F, 0x34, 0x12,  // X: $03, C- Z+ N-
        0x3F, 0x34, 0x12,  // X: $04, C- Z- N+
    });

    LoadData(0x1234, {
        0b00010001, 0b00010000, 0b10010001, 0b00001000, 0b01000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1234, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1235, 0b00100001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1236, 0b00100010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x03);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1237, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_RAM(0x1238, 0b10000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-RLA abs16, Y", "Unofficial-RLA-abs16,Y") {  // 0x3B
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x3B, 0x34, 0x12,  // Y: $00, C- Z- N-
        0x3B, 0x34, 0x12,  // Y: $01, C- Z- N-
        0x3B, 0x34, 0x12,  // Y: $02, C+ Z- N-
        0x3B, 0x34, 0x12,  // Y: $03, C- Z+ N-
        0x3B, 0x34, 0x12,  // Y: $04, C- Z- N+
    });

    LoadData(0x1234, {
        0b00010001, 0b00010000, 0b10010001, 0b00001000, 0b01000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1234, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1235, 0b00100001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1236, 0b00100010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x03);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1237, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_RAM(0x1238, 0b10000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-RLA (zp, X)", "Unofficial-RLA-(zp,X)") {  // 0x23
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0x23, 0x80,  // X: $00, C- Z- N-
        0x23, 0x80,  // X: $02, C- Z- N-
        0x23, 0x80,  // X: $04, C+ Z- N-
        0x23, 0x80,  // X: $06, C- Z+ N-
        0x23, 0x80,  // X: $08, C- Z- N+

        0x23, 0xF0,  // X: $20, C- Z- N-
        0x23, 0xF0,  // X: $22, C- Z- N-
        0x23, 0xF0,  // X: $24, C+ Z- N-
        0x23, 0xF0,  // X: $26, C- Z+ N-
        0x23, 0xF0,  // X: $28, C- Z- N+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
        0x37, 0x12,
        0x38, 0x12,
    });

    LoadData(0x10, {
        0x39, 0x12,
        0x3A, 0x12,
        0x3B, 0x12,
        0x3C, 0x12,
        0x3D, 0x12,
    });
    
    LoadData(0x1234, {
        0b00010001, 0b00010000, 0b10010001, 0b00001000, 0b01000001,
        0b00010001, 0b00010000, 0b10010001, 0b00001000, 0b01000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1234, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1235, 0b00100001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1236, 0b00100010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x06);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1237, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x08);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_RAM(0x1238, 0b10000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1239, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x123A, 0b00100001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x123B, 0b00100010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x26);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x123C, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x28);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_RAM(0x123D, 0b10000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-RLA (zp), Y", "Unofficial-RLA-(zp),Y") {  // 0x33
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0x33, 0x80,  // Y: $00, C- Z- N-
        0x33, 0x80,  // Y: $02, C- Z- N-
        0x33, 0x80,  // Y: $04, C+ Z- N-
        0x33, 0x80,  // Y: $06, C- Z+ N-
        0x33, 0x80,  // Y: $08, C- Z- N+

        0x33, 0x82,  // Y: $80, C- Z- N-
        0x33, 0x82,  // Y: $82, C- Z- N-
        0x33, 0x82,  // Y: $84, C+ Z- N-
        0x33, 0x82,  // Y: $86, C- Z+ N-
        0x33, 0x82,  // Y: $88, C- Z- N+
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC5, 0x11,
    });

    LoadData(0x1240, {
        0b00010001, 0b00010000, 0b10010001, 0b00001000, 0b01000001,
        0b00010001, 0b00010000, 0b10010001, 0b00001000, 0b01000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1240, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1241, 0b00100001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1242, 0b00100010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x03);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1243, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_RAM(0x1244, 0b10000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1245, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1246, 0b00100001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1247, 0b00100010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x83);
    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1248, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x84);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_RAM(0x1249, 0b10000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// RRA
//
TEST("Unofficial-RRA zp", "Unofficial-RRA-zp") {  // 0x67
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x67, 0x80,  // C- Z- V- N-
        0x67, 0x81,  // C- Z- V- N+
        0x67, 0x82,  // C+ Z- V+ N-
        0x67, 0x83,  // C+ Z+ V- N-
        0x67, 0x84,  // C- Z- V- N-
    });

    LoadData(0x80, {
        0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x80, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000010);
    CHECK_RAM(0x81, 0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x82, 0b10000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x83, 0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x84, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-RRA zp, X", "Unofficial-RRA-zp,X") {  // 0x77
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x77, 0x80,  // X: $00, C- Z- V- N-
        0x77, 0x80,  // X: $01, C- Z- V- N+
        0x77, 0x80,  // X: $02, C+ Z- V+ N-
        0x77, 0x80,  // X: $03, C+ Z+ V- N-
        0x77, 0x80,  // X: $04, C- Z- V- N-
    });

    LoadData(0x80, {
        0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x80, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterX(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000010);
    CHECK_RAM(0x81, 0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x82, 0b10000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    cpu->SetRegisterX(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x83, 0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x84, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-RRA abs16", "Unofficial-RRA-abs16") {  // 0x6F
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x6F, 0x34, 0x12,  // X: $00, C- Z- V- N-
        0x6F, 0x35, 0x12,  // X: $01, C- Z- V- N+
        0x6F, 0x36, 0x12,  // X: $02, C+ Z- V+ N-
        0x6F, 0x37, 0x12,  // X: $03, C+ Z+ V- N-
        0x6F, 0x38, 0x12,  // X: $04, C- Z- V- N-
    });

    LoadData(0x1234, {
        0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1234, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000010);
    CHECK_RAM(0x1235, 0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1236, 0b10000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1237, 0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-RRA abs16, X", "Unofficial-RRA-abs16,X") {  // 0x7F
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x7F, 0x34, 0x12,  // X: $00, C- Z- V- N-
        0x7F, 0x34, 0x12,  // X: $01, C- Z- V- N+
        0x7F, 0x34, 0x12,  // X: $02, C+ Z- V+ N-
        0x7F, 0x34, 0x12,  // X: $03, C+ Z+ V- N-
        0x7F, 0x34, 0x12,  // X: $04, C- Z- V- N-
    });

    LoadData(0x1234, {
        0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1234, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterX(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000010);
    CHECK_RAM(0x1235, 0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1236, 0b10000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    cpu->SetRegisterX(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1237, 0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-RRA abs16, Y", "Unofficial-RRA-abs16,Y") {  // 0x7B
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x7B, 0x34, 0x12,  // Y: $00, C- Z- V- N-
        0x7B, 0x34, 0x12,  // Y: $01, C- Z- V- N+
        0x7B, 0x34, 0x12,  // Y: $02, C+ Z- V+ N-
        0x7B, 0x34, 0x12,  // Y: $03, C+ Z+ V- N-
        0x7B, 0x34, 0x12,  // Y: $04, C- Z- V- N-
    });

    LoadData(0x1234, {
        0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1234, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000010);
    CHECK_RAM(0x1235, 0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1236, 0b10000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    cpu->SetRegisterY(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1237, 0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterY(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-RRA (zp, X)", "Unofficial-RRA-(zp,X)") {  // 0x63
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0x63, 0x80,  // X: $00, C- Z- V- N-
        0x63, 0x80,  // X: $02, C- Z- V- N+
        0x63, 0x80,  // X: $04, C+ Z- V+ N-
        0x63, 0x80,  // X: $06, C+ Z+ V- N-
        0x63, 0x80,  // X: $08, C- Z- V- N-

        0x63, 0xF0,  // X: $20, C- Z- V- N-
        0x63, 0xF0,  // X: $22, C- Z- V- N+
        0x63, 0xF0,  // X: $24, C+ Z- V+ N-
        0x63, 0xF0,  // X: $26, C+ Z+ V- N-
        0x63, 0xF0,  // X: $28, C- Z- V- N-
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
        0x37, 0x12,
        0x38, 0x12,
    });

    LoadData(0x10, {
        0x39, 0x12,
        0x3A, 0x12,
        0x3B, 0x12,
        0x3C, 0x12,
        0x3D, 0x12,
    });
    
    LoadData(0x1234, {
        0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000001,
        0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1234, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000010);
    CHECK_RAM(0x1235, 0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1236, 0b10000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    cpu->SetRegisterX(0x06);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1237, 0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x08);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1239, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterX(0x22);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000010);
    CHECK_RAM(0x123A, 0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterX(0x24);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x123B, 0b10000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    cpu->SetRegisterX(0x26);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x123C, 0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x123D, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-RRA (zp), Y", "Unofficial-RRA-(zp),Y") {  // 0x73
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0x73, 0x80,  // Y: $00, C- Z- V- N-
        0x73, 0x80,  // Y: $02, C- Z- V- N+
        0x73, 0x80,  // Y: $04, C+ Z- V+ N-
        0x73, 0x80,  // Y: $06, C+ Z+ V- N-
        0x73, 0x80,  // Y: $08, C- Z- V- N-

        0x73, 0x82,  // Y: $80, C- Z- V- N-
        0x73, 0x82,  // Y: $82, C- Z- V- N+
        0x73, 0x82,  // Y: $84, C+ Z- V+ N-
        0x73, 0x82,  // Y: $86, C+ Z+ V- N-
        0x73, 0x82,  // Y: $88, C- Z- V- N-
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC5, 0x11,
    });

    LoadData(0x1240, {
        0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000001,
        0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000001,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1240, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000010);
    CHECK_RAM(0x1241, 0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1242, 0b10000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    cpu->SetRegisterY(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1243, 0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterY(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1244, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterY(0x80);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_RAM(0x1245, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    cpu->SetRegisterY(0x81);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000010);
    CHECK_RAM(0x1246, 0b10000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterY(0x82);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1247, 0b10000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    cpu->SetRegisterY(0x83);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1248, 0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterY(0x84);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_RAM(0x1249, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

//
// SLO
//
TEST("Unofficial-SLO zp", "Unofficial-SLO-zp") {  // 0x07
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x07, 0x80,  // C- Z- N-
        0x07, 0x81,  // C+ Z- N-
        0x07, 0x82,  // C- Z+ N-
        0x07, 0x83,  // C- Z- N+
        0x07, 0x84,  // C+ Z- N+
    });

    LoadData(0x80, {
        0b00010001, 0b10000001, 0b00000000, 0b01000100, 0b10000000,
    });

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00101111);
    CHECK_RAM(0x80, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_RAM(0x81, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x82, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x83, 0b10001000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b10001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x84, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-SLO zp, X", "Unofficial-SLO-zp,X") {  // 0x17
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x17, 0x80,  // X: $00, C- Z- N-
        0x17, 0x80,  // X: $01, C+ Z- N-
        0x17, 0x80,  // X: $02, C- Z+ N-
        0x17, 0x80,  // X: $03, C- Z- N+
        0x17, 0x80,  // X: $04, C+ Z- N+
    });

    LoadData(0x80, {
        0b00010001, 0b10000001, 0b00000000, 0b01000100, 0b10000000,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00101111);
    CHECK_RAM(0x80, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_RAM(0x81, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x82, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x83, 0b10001000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b10001111);
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x84, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-SLO abs16", "Unofficial-SLO-abs16") {  // 0x0F
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x0F, 0x34, 0x12,  // X: $00, C- Z- N-
        0x0F, 0x35, 0x12,  // X: $01, C+ Z- N-
        0x0F, 0x36, 0x12,  // X: $02, C- Z+ N-
        0x0F, 0x37, 0x12,  // X: $03, C- Z- N+
        0x0F, 0x38, 0x12,  // X: $04, C+ Z- N+
    });

    LoadData(0x1234, {
        0b00010001, 0b10000001, 0b00000000, 0b01000100, 0b10000000,
    });

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00101111);
    CHECK_RAM(0x1234, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_RAM(0x1235, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1236, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1237, 0b10001000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b10001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-SLO abs16, X", "Unofficial-SLO-abs16,X") {  // 0x1F
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x1F, 0x34, 0x12,  // X: $00, C- Z- N-
        0x1F, 0x34, 0x12,  // X: $01, C+ Z- N-
        0x1F, 0x34, 0x12,  // X: $02, C- Z+ N-
        0x1F, 0x34, 0x12,  // X: $03, C- Z- N+
        0x1F, 0x34, 0x12,  // X: $04, C+ Z- N+
    });

    LoadData(0x1234, {
        0b00010001, 0b10000001, 0b00000000, 0b01000100, 0b10000000,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00101111);
    CHECK_RAM(0x1234, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_RAM(0x1235, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1236, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1237, 0b10001000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b10001111);
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-SLO abs16, Y", "Unofficial-SLO-abs16,Y") {  // 0x1B
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x1B, 0x34, 0x12,  // Y: $00, C- Z- N-
        0x1B, 0x34, 0x12,  // Y: $01, C+ Z- N-
        0x1B, 0x34, 0x12,  // Y: $02, C- Z+ N-
        0x1B, 0x34, 0x12,  // Y: $03, C- Z- N+
        0x1B, 0x34, 0x12,  // Y: $04, C+ Z- N+
    });

    LoadData(0x1234, {
        0b00010001, 0b10000001, 0b00000000, 0b01000100, 0b10000000,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00101111);
    CHECK_RAM(0x1234, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_RAM(0x1235, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1236, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1237, 0b10001000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b10001111);
    cpu->SetRegisterY(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-SLO (zp, X)", "Unofficial-SLO-(zp,X)") {  // 0x03
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0x03, 0x80,  // X: $00, C- Z- N-
        0x03, 0x80,  // X: $02, C+ Z- N-
        0x03, 0x80,  // X: $04, C- Z+ N-
        0x03, 0x80,  // X: $06, C- Z- N+
        0x03, 0x80,  // X: $08, C+ Z- N+

        0x03, 0xF0,  // X: $20, C- Z- N-
        0x03, 0xF0,  // X: $22, C+ Z- N-
        0x03, 0xF0,  // X: $24, C- Z+ N-
        0x03, 0xF0,  // X: $26, C- Z- N+
        0x03, 0xF0,  // X: $28, C+ Z- N+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
        0x37, 0x12,
        0x38, 0x12,
    });

    LoadData(0x10, {
        0x39, 0x12,
        0x3A, 0x12,
        0x3B, 0x12,
        0x3C, 0x12,
        0x3D, 0x12,
    });
    
    LoadData(0x1234, {
        0b00010001, 0b10000001, 0b00000000, 0b01000100, 0b10000000,
        0b00010001, 0b10000001, 0b00000000, 0b01000100, 0b10000000,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00101111);
    CHECK_RAM(0x1234, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_RAM(0x1235, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1236, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x06);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1237, 0b10001000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b10001111);
    cpu->SetRegisterX(0x08);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_A(0b00101111);
    CHECK_RAM(0x1239, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x22);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_RAM(0x123A, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x24);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x123B, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x26);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x123C, 0b10001000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b10001111);
    cpu->SetRegisterX(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x123D, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("Unofficial-SLO (zp), Y", "Unofficial-SLO-(zp),Y") {  // 0x13
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0x13, 0x80,  // Y: $00, C- Z- N-
        0x13, 0x80,  // Y: $02, C+ Z- N-
        0x13, 0x80,  // Y: $04, C- Z+ N-
        0x13, 0x80,  // Y: $06, C- Z- N+
        0x13, 0x80,  // Y: $08, C+ Z- N+

        0x13, 0x82,  // Y: $80, C- Z- N-
        0x13, 0x82,  // Y: $82, C+ Z- N-
        0x13, 0x82,  // Y: $84, C- Z+ N-
        0x13, 0x82,  // Y: $86, C- Z- N+
        0x13, 0x82,  // Y: $88, C+ Z- N+
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC5, 0x11,
    });

    LoadData(0x1240, {
        0b00010001, 0b10000001, 0b00000000, 0b01000100, 0b10000000,
        0b00010001, 0b10000001, 0b00000000, 0b01000100, 0b10000000,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b00101111);
    CHECK_RAM(0x1240, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_RAM(0x1241, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1242, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1243, 0b10001000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b10001111);
    cpu->SetRegisterY(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1244, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x80);
    EXECUTE(expectedCycles);
    CHECK_A(0b00101111);
    CHECK_RAM(0x1245, 0b00100010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x81);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_RAM(0x1246, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterY(0x82);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1247, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x83);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1248, 0b10001000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b10001111);
    cpu->SetRegisterY(0x84);
    EXECUTE(expectedCycles);
    CHECK_A(0b10001111);
    CHECK_RAM(0x1249, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// SRE
//
TEST("Unofficial-SRE zp", "Unofficial-SRE-zp") {  // 0x47
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x47, 0x80,  // C- Z- N-
        0x47, 0x81,  // C+ Z- N-
        0x47, 0x82,  // C- Z+ N-
        0x47, 0x83,  // C- Z- N+
        0x47, 0x84,  // C+ Z+ N-
    });

    LoadData(0x80, {
        0b10001000, 0b00010001, 0b00011110, 0b11100000, 0b00000001,
    });

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01001011);
    CHECK_RAM(0x80, 0b01000100);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_RAM(0x81, 0b00001000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x82, 0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11110000);
    CHECK_RAM(0x83, 0b01110000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x84, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-SRE zp, X", "Unofficial-SRE-zp,X") {  // 0x57
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x57, 0x80,  // X: $00, C- Z- N-
        0x57, 0x80,  // X: $01, C+ Z- N-
        0x57, 0x80,  // X: $02, C- Z+ N-
        0x57, 0x80,  // X: $03, C- Z- N+
        0x57, 0x80,  // X: $04, C+ Z+ N-
    });

    LoadData(0x80, {
        0b10001000, 0b00010001, 0b00011110, 0b11100000, 0b00000001,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b01001011);
    CHECK_RAM(0x80, 0b01000100);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_RAM(0x81, 0b00001000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x82, 0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterX(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b11110000);
    CHECK_RAM(0x83, 0b01110000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x84, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-SRE abs16", "Unofficial-SRE-abs16") {  // 0x4F
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x4F, 0x34, 0x12,  // X: $00, C- Z- N-
        0x4F, 0x35, 0x12,  // X: $01, C+ Z- N-
        0x4F, 0x36, 0x12,  // X: $02, C- Z+ N-
        0x4F, 0x37, 0x12,  // X: $03, C- Z- N+
        0x4F, 0x38, 0x12,  // X: $04, C+ Z+ N-
    });

    LoadData(0x1234, {
        0b10001000, 0b00010001, 0b00011110, 0b11100000, 0b00000001,
    });

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01001011);
    CHECK_RAM(0x1234, 0b01000100);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_RAM(0x1235, 0b00001000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1236, 0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11110000);
    CHECK_RAM(0x1237, 0b01110000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-SRE abs16, X", "Unofficial-SRE-abs16,X") {  // 0x5F
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x5F, 0x34, 0x12,  // X: $00, C- Z- N-
        0x5F, 0x34, 0x12,  // X: $01, C+ Z- N-
        0x5F, 0x34, 0x12,  // X: $02, C- Z+ N-
        0x5F, 0x34, 0x12,  // X: $03, C- Z- N+
        0x5F, 0x34, 0x12,  // X: $04, C+ Z+ N-
    });

    LoadData(0x1234, {
        0b10001000, 0b00010001, 0b00011110, 0b11100000, 0b00000001,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b01001011);
    CHECK_RAM(0x1234, 0b01000100);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_RAM(0x1235, 0b00001000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1236, 0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterX(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b11110000);
    CHECK_RAM(0x1237, 0b01110000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-SRE abs16, Y", "Unofficial-SRE-abs16,Y") {  // 0x5B
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x5B, 0x34, 0x12,  // Y: $00, C- Z- N-
        0x5B, 0x34, 0x12,  // Y: $01, C+ Z- N-
        0x5B, 0x34, 0x12,  // Y: $02, C- Z+ N-
        0x5B, 0x34, 0x12,  // Y: $03, C- Z- N+
        0x5B, 0x34, 0x12,  // Y: $04, C+ Z+ N-
    });

    LoadData(0x1234, {
        0b10001000, 0b00010001, 0b00011110, 0b11100000, 0b00000001,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b01001011);
    CHECK_RAM(0x1234, 0b01000100);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_RAM(0x1235, 0b00001000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1236, 0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterY(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b11110000);
    CHECK_RAM(0x1237, 0b01110000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterY(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-SRE (zp, X)", "Unofficial-SRE-(zp,X)") {  // 0x43
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0x43, 0x80,  // X: $00, C- Z- N-
        0x43, 0x80,  // X: $02, C+ Z- N-
        0x43, 0x80,  // X: $04, C- Z+ N-
        0x43, 0x80,  // X: $06, C- Z- N+
        0x43, 0x80,  // X: $08, C+ Z+ N-

        0x43, 0xF0,  // X: $20, C- Z- N-
        0x43, 0xF0,  // X: $22, C+ Z- N-
        0x43, 0xF0,  // X: $24, C- Z+ N-
        0x43, 0xF0,  // X: $26, C- Z- N+
        0x43, 0xF0,  // X: $28, C+ Z+ N-
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
        0x37, 0x12,
        0x38, 0x12,
    });

    LoadData(0x10, {
        0x39, 0x12,
        0x3A, 0x12,
        0x3B, 0x12,
        0x3C, 0x12,
        0x3D, 0x12,
    });
    
    LoadData(0x1234, {
        0b10001000, 0b00010001, 0b00011110, 0b11100000, 0b00000001,
        0b10001000, 0b00010001, 0b00011110, 0b11100000, 0b00000001,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b01001011);
    CHECK_RAM(0x1234, 0b01000100);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_RAM(0x1235, 0b00001000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1236, 0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterX(0x06);
    EXECUTE(expectedCycles);
    CHECK_A(0b11110000);
    CHECK_RAM(0x1237, 0b01110000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x08);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1238, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_A(0b01001011);
    CHECK_RAM(0x1239, 0b01000100);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x22);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_RAM(0x123A, 0b00001000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0x24);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x123B, 0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterX(0x26);
    EXECUTE(expectedCycles);
    CHECK_A(0b11110000);
    CHECK_RAM(0x123C, 0b01110000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterX(0x28);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x123D, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("Unofficial-SRE (zp), Y", "Unofficial-SRE-(zp),Y") {  // 0x53
    constexpr int expectedCycles = 8;

    LoadData(0x200, {
        0x53, 0x80,  // Y: $00, C- Z- N-
        0x53, 0x80,  // Y: $02, C+ Z- N-
        0x53, 0x80,  // Y: $04, C- Z+ N-
        0x53, 0x80,  // Y: $06, C- Z- N+
        0x53, 0x80,  // Y: $08, C+ Z+ N-

        0x53, 0x82,  // Y: $80, C- Z- N-
        0x53, 0x82,  // Y: $82, C+ Z- N-
        0x53, 0x82,  // Y: $84, C- Z+ N-
        0x53, 0x82,  // Y: $86, C- Z- N+
        0x53, 0x82,  // Y: $88, C+ Z+ N-
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC5, 0x11,
    });

    LoadData(0x1240, {
        0b10001000, 0b00010001, 0b00011110, 0b11100000, 0b00000001,
        0b10001000, 0b00010001, 0b00011110, 0b11100000, 0b00000001,
    });

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b01001011);
    CHECK_RAM(0x1240, 0b01000100);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_RAM(0x1241, 0b00001000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1242, 0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterY(0x03);
    EXECUTE(expectedCycles);
    CHECK_A(0b11110000);
    CHECK_RAM(0x1243, 0b01110000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterY(0x04);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1244, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x80);
    EXECUTE(expectedCycles);
    CHECK_A(0b01001011);
    CHECK_RAM(0x1245, 0b01000100);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x81);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_RAM(0x1246, 0b00001000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterY(0x82);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1247, 0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000000);
    cpu->SetRegisterY(0x83);
    EXECUTE(expectedCycles);
    CHECK_A(0b11110000);
    CHECK_RAM(0x1248, 0b01110000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterA(0b00000000);
    cpu->SetRegisterY(0x84);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_RAM(0x1249, 0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

//
// LAS
//
TEST("Unofficial-LAS abs16, Y", "Unofficial-LAS-abs16,Y") {  // 0xBB
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xBB, 0x34, 0x12,  // Y: $00, Z- N-
        0xBB, 0x34, 0x12,  // Y: $01, Z+ N-
        0xBB, 0x34, 0x12,  // Y: $02, Z- N+

        0xBB, 0xF0, 0x20,  // Y: $30, Z- N-
        0xBB, 0xF0, 0x20,  // Y: $31, Z+ N-
        0xBB, 0xF0, 0x20,  // Y: $32, Z- N+
    });

    LoadData(0x1234, {
        0b01010101, 0b00000000, 0b10101010,
    });

    LoadData(0x2120, {
        0b01010101, 0b00000000, 0b10101010,
    });

    cpu->SetRegisterY(0x00);
    cpu->SetRegisterS(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01010000);
    CHECK_S(0b01010000);
    CHECK_X(0b01010000);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x01);
    cpu->SetRegisterS(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_S(0b00000000);
    CHECK_X(0b00000000);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x02);
    cpu->SetRegisterS(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10100000);
    CHECK_S(0b10100000);
    CHECK_X(0b10100000);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetRegisterY(0x30);
    cpu->SetRegisterS(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01010000);
    CHECK_S(0b01010000);
    CHECK_X(0b01010000);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x31);
    cpu->SetRegisterS(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_S(0b00000000);
    CHECK_X(0b00000000);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x32);
    cpu->SetRegisterS(0b11110000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10100000);
    CHECK_S(0b10100000);
    CHECK_X(0b10100000);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// TAS
//
TEST("Unofficial-TAS abs16, Y", "Unofficial-TAS-abs16,Y") {  // 0x9B
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x9B, 0x34, 0x12,  // Y: $00 - destination: 0x1234
        0x9B, 0xF0, 0x20,  // Y: $30 - destination: 0x2120
    });

    cpu->SetRegisterA(0b01010101);
    cpu->SetRegisterX(0b11110001);
    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_A(0b01010101);
    CHECK_X(0b11110001);
    CHECK_S(0b01010001);
    CHECK_RAM(0x1234, 0b00010001);

    cpu->SetRegisterA(0b10101011);
    cpu->SetRegisterX(0b11110001);
    cpu->SetRegisterY(0x30);
    EXECUTE(expectedCycles);
    CHECK_A(0b10101011);
    CHECK_X(0b11110001);
    CHECK_S(0b10100001);
    CHECK_RAM(0x2120, 0b00100001);
}

//
// SHY
//
TEST("Unofficial-SHY abs16, X", "Unofficial-SHY-abs16,X") {  // 0x9C
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x9C, 0x34, 0x12,  // X: $00 - destination: 0x1234
        0x9C, 0xF0, 0x20,  // X: $30 - destination: 0x2120
    });

    cpu->SetRegisterY(0b01010001);
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_Y(0b01010001);
    CHECK_RAM(0x1234, 0b00010001);

    cpu->SetRegisterY(0b10100001);
    cpu->SetRegisterX(0x30);
    EXECUTE(expectedCycles);
    CHECK_Y(0b10100001);
    CHECK_RAM(0x2120, 0b00100001);
}

//
// SHX
//
TEST("Unofficial-SHX abs16, Y", "Unofficial-SHX-abs16,Y") {  // 0x9E
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x9E, 0x34, 0x12,  // Y: $00 - destination: 0x1234
        0x9E, 0xF0, 0x20,  // Y: $30 - destination: 0x2120
    });

    cpu->SetRegisterX(0b01010001);
    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_X(0b01010001);
    CHECK_RAM(0x1234, 0b00010001);

    cpu->SetRegisterX(0b10100001);
    cpu->SetRegisterY(0x30);
    EXECUTE(expectedCycles);
    CHECK_X(0b10100001);
    CHECK_RAM(0x2120, 0b00100001);
}

//
// AHX
//
TEST("Unofficial-AHX (zp), Y", "Unofficial-AHX-(zp),Y") {  // 0x93
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x93, 0x80,
        0x93, 0xFF,
    });

    LoadData(0x80, { 0x34, 0x12 });
    LoadData(0xFF, 0x34); LoadData(0x00, 0x12);

    LoadData(0x1234, 0b00111100);

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0b10101010);
    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_X(0b10101010);
    CHECK_RAM(0x1234, 0b00111100);
    CHECK_RAM(0x1235, 0b00001000);

    cpu->SetRegisterA(0b00001111);
    cpu->SetRegisterX(0b10101010);
    cpu->SetRegisterY(0x02);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_X(0b10101010);
    CHECK_RAM(0x1234, 0b00111100);
    CHECK_RAM(0x1236, 0b00001000);
}

TEST("Unofficial-AHX abs16, Y", "Unofficial-AHX-abs16,Y") {  // 0x9F
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x9F, 0x34, 0x12,
    });

    cpu->SetRegisterA(0b01010101);
    cpu->SetRegisterX(0b00110011);
    cpu->SetRegisterY(0x10);
    EXECUTE(expectedCycles);
    CHECK_A(0b01010101);
    CHECK_X(0b00110011);
    CHECK_RAM(0x1244, 0b00010001);
}
