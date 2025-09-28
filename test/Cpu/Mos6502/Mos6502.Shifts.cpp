/* Mos6502.Shifts.cpp
 *
 * Tests the CPU core's ability to perform shifts and rotates.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

//
// ASL
//
TEST("ASL A", "ASL-A") {  // 0x0A
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0x0A,  // C- Z- N-
        0x0A,  // C+ Z- N-
        0x0A,  // C- Z+ N-
        0x0A,  // C- Z- N+
    });

    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b01000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("ASL zp", "ASL-zp") {  // 0x06
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x06, 0x80,  // C- Z- N-
        0x06, 0x82,  // C+ Z- N-
        0x06, 0x84,  // C- Z+ N-
        0x06, 0x86,  // C- Z- N+
    });

    LoadData(0x80, {
        0x00, 0x12,
        0x10, 0x12,
        0x20, 0x12,
        0x30, 0x12,
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("ASL zp, X", "ASL-zp,X") {  // 0x16
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x16, 0x80,  // X: $00, C- Z- N-
        0x16, 0x80,  // X: $02, C+ Z- N-
        0x16, 0x80,  // X: $04, C- Z+ N-
        0x16, 0x80,  // X: $06, C- Z- N+
    });

    LoadData(0x80, {
        0x00, 0x12,
        0x10, 0x12,
        0x20, 0x12,
        0x30, 0x12,
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x06);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("ASL abs16", "ASL-abs16") {  // 0x0E
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x0E, 0x00, 0x12,  // C- Z- N-
        0x0E, 0x10, 0x12,  // C+ Z- N-
        0x0E, 0x20, 0x12,  // C- Z+ N-
        0x0E, 0x30, 0x12,  // C- Z- N+
    });
    
    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("ASL abs16, X", "ASL-abs16,X") {  // 0x1E
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x1E, 0x00, 0x12,  // X: $00, C- Z- N-
        0x1E, 0x00, 0x12,  // X: $10, C+ Z- N-
        0x1E, 0x00, 0x12,  // X: $20, C- Z+ N-
        0x1E, 0x00, 0x12,  // X: $30, C- Z- N+
    });
    
    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x10);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x30);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// LSR
//
TEST("LSR A", "LSR-A") {  // 0x4A
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0x4A,  // C- Z- N-
        0x4A,  // C+ Z- N-
        0x4A,  // C- Z+ N-
    });

    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("LSR zp", "LSR-zp") {  // 0x46
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x46, 0x80,  // C- Z- N-
        0x46, 0x82,  // C+ Z- N-
        0x46, 0x84,  // C- Z+ N-
    });

    LoadData(0x80, {
        0x00, 0x12,
        0x10, 0x12,
        0x20, 0x12,
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("LSR zp, X", "LSR-zp,X") {  // 0x56
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x56, 0x80,  // X: $00, C- Z- N-
        0x56, 0x80,  // X: $02, C+ Z- N-
        0x56, 0x80,  // X: $04, C- Z+ N-
    });

    LoadData(0x80, {
        0x00, 0x12,
        0x10, 0x12,
        0x20, 0x12,
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("LSR abs16", "LSR-abs16") {  // 0x4E
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x4E, 0x00, 0x12,  // C- Z- N-
        0x4E, 0x10, 0x12,  // C+ Z- N-
        0x4E, 0x20, 0x12,  // C- Z+ N-
    });
    
    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

TEST("LSR abs16, X", "LSR-abs16,X") {  // 0x5E
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x5E, 0x00, 0x12,  // X: $00, C- Z- N-
        0x5E, 0x00, 0x12,  // X: $10, C+ Z- N-
        0x5E, 0x00, 0x12,  // X: $20, C- Z+ N-
    });
    
    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x10);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;
}

//
// ROL
//
TEST("ROL A", "ROL-A") {  // 0x2A
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0x2A,  // C- Z- N-
        0x2A,  // C+ Z- N-
        0x2A,  // C- Z+ N-
        0x2A,  // C- Z- N-
        0x2A,  // C- Z- N-
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b01000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ROL zp", "ROL-zp") {  // 0x26
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x26, 0x80,  // C- Z- N-
        0x26, 0x82,  // C+ Z- N-
        0x26, 0x84,  // C- Z+ N-
        0x26, 0x86,  // C- Z- N-
        0x26, 0x88,  // C- Z- N-
    });

    LoadData(0x80, {
        0x00, 0x12,
        0x10, 0x12,
        0x20, 0x12,
        0x30, 0x12,
        0x40, 0x12,
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);
    memory->SetByte(0x1240, 0b00000000);

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1240, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ROL zp, X", "ROL-zp,X") {  // 0x36
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x36, 0x80,  // X: $00, C- Z- N-
        0x36, 0x80,  // X: $02, C+ Z- N-
        0x36, 0x80,  // X: $04, C- Z+ N-
        0x36, 0x80,  // X: $06, C- Z- N-
        0x36, 0x80,  // X: $08, C- Z- N-
    });

    LoadData(0x80, {
        0x00, 0x12,
        0x10, 0x12,
        0x20, 0x12,
        0x30, 0x12,
        0x40, 0x12,
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);
    memory->SetByte(0x1240, 0b00000000);

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x06);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x08);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1240, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ROL abs16", "ROL-abs16") {  // 0x2E
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x2E, 0x00, 0x12,  // C- Z- N-
        0x2E, 0x10, 0x12,  // C+ Z- N-
        0x2E, 0x20, 0x12,  // C- Z+ N-
        0x2E, 0x30, 0x12,  // C- Z- N-
        0x2E, 0x40, 0x12,  // C- Z- N-
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);
    memory->SetByte(0x1240, 0b00000000);

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1240, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ROL abs16, X", "ROL-abs16,X") {  // 0x3E
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x3E, 0x00, 0x12,  // X: $00, C- Z- N-
        0x3E, 0x00, 0x12,  // X: $10, C+ Z- N-
        0x3E, 0x00, 0x12,  // X: $20, C- Z+ N-
        0x3E, 0x00, 0x12,  // X: $30, C- Z- N-
        0x3E, 0x00, 0x12,  // X: $40, C- Z- N-
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b01000000);
    memory->SetByte(0x1240, 0b00000000);

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b01000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x10);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b00000010);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x30);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x40);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1240, 0b00000001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

//
// ROR
//
TEST("ROR A", "ROR-A") {  // 0x6A
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0x6A,  // C- Z- N-
        0x6A,  // C+ Z- N-
        0x6A,  // C- Z+ N-
        0x6A,  // C- Z- N+
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00100000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("ROR zp", "ROR-zp") {  // 0x66
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x66, 0x80,  // C- Z- N-
        0x66, 0x82,  // C+ Z- N-
        0x66, 0x84,  // C- Z+ N-
        0x66, 0x86,  // C- Z- N+
    });

    LoadData(0x80, {
        0x00, 0x12,
        0x10, 0x12,
        0x20, 0x12,
        0x30, 0x12,
        0x40, 0x12,
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b00000000);

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("ROR zp, X", "ROR-zp,X") {  // 0x76
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x76, 0x80,  // X: $00, C- Z- N-
        0x76, 0x80,  // X: $02, C+ Z- N-
        0x76, 0x80,  // X: $04, C- Z+ N-
        0x76, 0x80,  // X: $06, C- Z- N+
    });

    LoadData(0x80, {
        0x00, 0x12,
        0x10, 0x12,
        0x20, 0x12,
        0x30, 0x12,
        0x40, 0x12,
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b00000000);

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x04);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x06);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("ROR abs16", "ROR-abs16") {  // 0x6E
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x6E, 0x00, 0x12,  // C- Z- N-
        0x6E, 0x10, 0x12,  // C+ Z- N-
        0x6E, 0x20, 0x12,  // C- Z+ N-
        0x6E, 0x30, 0x12,  // C- Z- N+
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b00000000);

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("ROR abs16, X", "ROR-abs16,X") {  // 0x7E
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0x7E, 0x00, 0x12,  // X: $00, C- Z- N-
        0x7E, 0x00, 0x12,  // X: $10, C+ Z- N-
        0x7E, 0x00, 0x12,  // X: $20, C- Z+ N-
        0x7E, 0x00, 0x12,  // X: $30, C- Z- N+
    });

    memory->SetByte(0x1200, 0b00100000);
    memory->SetByte(0x1210, 0b10000001);
    memory->SetByte(0x1220, 0b00000000);
    memory->SetByte(0x1230, 0b00000000);

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1200, 0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x10);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1210, 0b01000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1220, 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1230, 0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}
