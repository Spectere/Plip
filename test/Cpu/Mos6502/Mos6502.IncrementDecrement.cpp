/* Mos6502.Arithmetic.cpp
 *
 * Tests the CPU core's ability to perform increments and decrements.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

//
// INC
//
TEST("INC zp", "INC-zp") {  // 0xE6
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0xE6, 0x80,  // Z- N-
        0xE6, 0x81,  // Z+ N-
        0xE6, 0x82,  // Z- N+
    });

    LoadData(0x80, {
        0x10,
        0xFF,
        0x7F,
    });

    EXECUTE(expectedCycles);
    CHECK_RAM(0x80, 0x11);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x81, 0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x82, 0x80);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("INC zp, X", "INC-zp,X") {  // 0xF6
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xF6, 0x80,  // X: $00, Z- N-
        0xF6, 0x80,  // X: $01, Z+ N-
        0xF6, 0x80,  // X: $02, Z- N+
    });

    LoadData(0x80, {
        0x10,
        0xFF,
        0x7F,
    });

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x80, 0x11);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x01);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x81, 0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x82, 0x80);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("INC abs16", "INC-abs16") {  // 0xEE
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xEE, 0x00, 0x11,  // Z- N-
        0xEE, 0x10, 0x11,  // Z+ N-
        0xEE, 0x20, 0x11,  // Z- N+
    });

    memory->SetByte(0x1100, 0x10);
    memory->SetByte(0x1110, 0xFF);
    memory->SetByte(0x1120, 0x7F);

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1100, 0x11);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1110, 0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1120, 0x80);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("INC abs16, X", "INC-abs16,X") {  // 0xFE
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0xFE, 0x00, 0x11,  // X: $00, Z- N-
        0xFE, 0x00, 0x11,  // X: $10, Z+ N-
        0xFE, 0x00, 0x11,  // X: $20, Z- N+
    });

    memory->SetByte(0x1100, 0x10);
    memory->SetByte(0x1110, 0xFF);
    memory->SetByte(0x1120, 0x7F);

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1100, 0x11);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x10);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1110, 0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1120, 0x80);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// INX
//
TEST("INX", "INX") {  // 0xE8
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0xE8,  // Z- N-
        0xE8,  // Z+ N-
        0xE8,  // Z- N+
    });

    cpu->SetRegisterX(0x10);
    EXECUTE(expectedCycles);
    CHECK_X(0x11);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0xFF);
    EXECUTE(expectedCycles);
    CHECK_X(0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x7F);
    EXECUTE(expectedCycles);
    CHECK_X(0x80);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// INY
//
TEST("INY", "INY") {  // 0xC8
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0xC8,  // Z- N-
        0xC8,  // Z+ N-
        0xC8,  // Z- N+
    });

    cpu->SetRegisterY(0x10);
    EXECUTE(expectedCycles);
    CHECK_Y(0x11);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0xFF);
    EXECUTE(expectedCycles);
    CHECK_Y(0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x7F);
    EXECUTE(expectedCycles);
    CHECK_Y(0x80);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// DEC
//
TEST("DEC zp", "DEC-zp") {  // 0xC6
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0xC6, 0x80,  // Z- N-
        0xC6, 0x81,  // Z+ N-
        0xC6, 0x82,  // Z- N+
    });

    LoadData(0x80, {
        0x10,
        0x01,
        0x00,
    });

    EXECUTE(expectedCycles);
    CHECK_RAM(0x80, 0x0F);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x81, 0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x82, 0xFF);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("DEC zp, X", "DEC-zp,X") {  // 0xD6
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xD6, 0x80,  // X: $00, Z- N-
        0xD6, 0x80,  // X: $01, Z+ N-
        0xD6, 0x80,  // X: $02, Z- N+
    });

    LoadData(0x80, {
        0x10,
        0x01,
        0x00,
    });

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x80, 0x0F);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x01);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x81, 0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x02);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x82, 0xFF);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("DEC abs16", "DEC-abs16") {  // 0xCE
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xCE, 0x00, 0x11,  // Z- N-
        0xCE, 0x10, 0x11,  // Z+ N-
        0xCE, 0x20, 0x11,  // Z- N+
    });

    memory->SetByte(0x1100, 0x10);
    memory->SetByte(0x1110, 0x01);
    memory->SetByte(0x1120, 0x00);

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1100, 0x0F);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1110, 0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    EXECUTE(expectedCycles);
    CHECK_RAM(0x1120, 0xFF);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("DEC abs16, X", "DEC-abs16,X") {  // 0xDE
    constexpr int expectedCycles = 7;

    LoadData(0x200, {
        0xDE, 0x00, 0x11,  // X: $00, Z- N-
        0xDE, 0x00, 0x11,  // X: $10, Z+ N-
        0xDE, 0x00, 0x11,  // X: $20, Z- N+
    });

    memory->SetByte(0x1100, 0x10);
    memory->SetByte(0x1110, 0x01);
    memory->SetByte(0x1120, 0x00);

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1100, 0x0F);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x10);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1110, 0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x20);
    EXECUTE(expectedCycles);
    CHECK_RAM(0x1120, 0xFF);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// DEX
//
TEST("DEX", "DEX") {  // 0xCA
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0xCA,  // Z- N-
        0xCA,  // Z+ N-
        0xCA,  // Z- N+
    });

    cpu->SetRegisterX(0x10);
    EXECUTE(expectedCycles);
    CHECK_X(0x0F);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x01);
    EXECUTE(expectedCycles);
    CHECK_X(0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterX(0x00);
    EXECUTE(expectedCycles);
    CHECK_X(0xFF);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// DEY
//
TEST("DEY", "DEY") {  // 0x88
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0x88,  // Z- N-
        0x88,  // Z+ N-
        0x88,  // Z- N+
    });

    cpu->SetRegisterY(0x10);
    EXECUTE(expectedCycles);
    CHECK_Y(0x0F);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x01);
    EXECUTE(expectedCycles);
    CHECK_Y(0x00);
    CHECK_ZERO_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetRegisterY(0x00);
    EXECUTE(expectedCycles);
    CHECK_Y(0xFF);
    CHECK_ZERO_CLEAR;
    CHECK_NEGATIVE_SET;
}
