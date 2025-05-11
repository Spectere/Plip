/* Mos6502.Logical.cpp
*
 * Tests the CPU core's ability to perform logical bitwise operations.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

TEST("AND imm8", "AND-imm8") {  // 0x29
    LoadData(0x200, {
        0x29, 0b01010101,  // N- Z-
        0x29, 0b10101010,  // N+ Z-
        0x29, 0b01010101,  // N- Z+
    });

    cpu->SetRegisterA(0b11110000);
    EXECUTE(2);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(2);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b10101010);
    EXECUTE(2);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("AND zp", "AND-zp") {  // 0x25
    LoadData(0x200, {
        0x25, 0x80,  // N- Z-
        0x25, 0x82,  // N+ Z-
        0x25, 0x84,  // N- Z+
    });

    LoadData(0x80, {
        0b01010101, 0,
        0b10101010, 0,
        0b01010101,
    });

    cpu->SetRegisterA(0b11110000);
    EXECUTE(3);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(3);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b10101010);
    EXECUTE(3);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("AND zp, X", "AND-zp,X") {  // 0x35
    LoadData(0x200, {
        0x35, 0x80,  // X: $00, N- Z-
        0x35, 0x80,  // X: $02, N+ Z-
        0x35, 0x80,  // X: $04, N- Z+
    });

    LoadData(0x80, {
        0b01010101, 0,
        0b10101010, 0,
        0b01010101,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b10101010);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("AND abs16", "AND-abs16") {  // 0x2D
    LoadData(0x200, {
        0x2D, 0x34, 0x12,  // N- Z-
        0x2D, 0x36, 0x12,  // N+ Z-
        0x2D, 0x38, 0x12,  // N- Z+
    });

    LoadData(0x1234, {
        0b01010101, 0,
        0b10101010, 0,
        0b01010101,
    });

    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b10101010);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("AND abs16, X", "AND-abs16, X") {  // 0x3D
    LoadData(0x200, {
        0x3D, 0x34, 0x12,  // X: $00, N- Z-
        0x3D, 0x34, 0x12,  // X: $02, N+ Z-
        0x3D, 0x34, 0x12,  // X: $04, N- Z+

        0x3D, 0xF0, 0x20,  // +1 X: $30, N- Z-
        0x3D, 0xF0, 0x20,  // +1 X: $32, N+ Z-
        0x3D, 0xF0, 0x20,  // +1 X: $34, N- Z+
    });

    LoadData(0x1234, {
        0b01010101, 0,
        0b10101010, 0,
        0b01010101,
    });

    LoadData(0x2120, {
        0b01010101, 0,
        0b10101010, 0,
        0b01010101,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b10101010);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x34);
    cpu->SetRegisterA(0b10101010);
    EXECUTE(5);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("AND abs16, Y", "AND-abs16, Y") {  // 0x39
    LoadData(0x200, {
        0x39, 0x34, 0x12,  // X: $00, N- Z-
        0x39, 0x34, 0x12,  // X: $02, N+ Z-
        0x39, 0x34, 0x12,  // X: $04, N- Z+

        0x39, 0xF0, 0x20,  // +1 X: $30, N- Z-
        0x39, 0xF0, 0x20,  // +1 X: $32, N+ Z-
        0x39, 0xF0, 0x20,  // +1 X: $34, N- Z+
    });

    LoadData(0x1234, {
        0b01010101, 0,
        0b10101010, 0,
        0b01010101,
    });

    LoadData(0x2120, {
        0b01010101, 0,
        0b10101010, 0,
        0b01010101,
    });

    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0b10101010);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x34);
    cpu->SetRegisterA(0b10101010);
    EXECUTE(5);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("AND (imm8, X)", "AND-(imm8, X)") {  // 0x21
    LoadData(0x200, {
        0x21, 0x80,  // X: $00, N- Z-
        0x21, 0x80,  // X: $02, N+ Z-
        0x21, 0x80,  // X: $04, N- Z+

        0x21, 0xF0,  // X: $20, N- Z-
        0x21, 0xF0,  // X: $22, N+ Z-
        0x21, 0xF0,  // X: $24, N- Z+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x36, 0x12,
        0x38, 0x12,
    });

    LoadData(0x10, {
        0x34, 0x12,
        0x36, 0x12,
        0x38, 0x12,
    });

    LoadData(0x1234, {
        0b01010101, 0,
        0b10101010, 0,
        0b01010101,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b10101010);
    EXECUTE(6);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b10101010);
    EXECUTE(6);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("AND (imm8), Y", "AND-(imm8),Y") {  // 0x31
    LoadData(0x200, {
        0x31, 0x80,  // Y: $00, N- Z-
        0x31, 0x80,  // Y: $02, N+ Z-
        0x31, 0x80,  // Y: $04, N- Z+

        0x31, 0x82,  // +1 Y: $80, N- Z-
        0x31, 0x82,  // +1 Y: $82, N+ Z-
        0x31, 0x82,  // +1 Y: $84, N- Z+
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC0, 0x11,
    });

    LoadData(0x1240, {
        0b01010101, 0,
        0b10101010, 0,
        0b01010101,
    });

    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0b10101010);
    EXECUTE(5);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b01010000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b10100000);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x84);
    cpu->SetRegisterA(0b10101010);
    EXECUTE(6);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("EOR imm8", "EOR-imm8") {  // 0x49
    LoadData(0x200, {
        0x49, 0b10101010,  // N- Z-
        0x49, 0b01010101,  // N+ Z-
        0x49, 0b01010101,  // N- Z+
    });

    cpu->SetRegisterA(0b11110000);
    EXECUTE(2);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(2);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b01010101);
    EXECUTE(2);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("EOR zp", "EOR-zp") {  // 0x45
    LoadData(0x200, {
        0x45, 0x80,  // N- Z-
        0x45, 0x82,  // N+ Z-
        0x45, 0x84,  // N- Z+
    });

    LoadData(0x80, {
        0b10101010, 0,
        0b01010101, 0,
        0b01010101,
    });

    cpu->SetRegisterA(0b11110000);
    EXECUTE(3);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(3);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b01010101);
    EXECUTE(3);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("EOR zp, X", "EOR-zp,X") {  // 0x55
    LoadData(0x200, {
        0x55, 0x80,  // X: $00, N- Z-
        0x55, 0x80,  // X: $02, N+ Z-
        0x55, 0x80,  // X: $04, N- Z+
    });

    LoadData(0x80, {
        0b10101010, 0,
        0b01010101, 0,
        0b01010101,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b01010101);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("EOR abs16", "EOR-abs16") {  // 0x4D
    LoadData(0x200, {
        0x4D, 0x34, 0x12,  // N- Z-
        0x4D, 0x36, 0x12,  // N+ Z-
        0x4D, 0x38, 0x12,  // N- Z+
    });

    LoadData(0x1234, {
        0b10101010, 0,
        0b01010101, 0,
        0b01010101,
    });

    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b01010101);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("EOR abs16, X", "EOR-abs16, X") {  // 0x5D
    LoadData(0x200, {
        0x5D, 0x34, 0x12,  // X: $00, N- Z-
        0x5D, 0x34, 0x12,  // X: $02, N+ Z-
        0x5D, 0x34, 0x12,  // X: $04, N- Z+

        0x5D, 0xF0, 0x20,  // +1 X: $30, N- Z-
        0x5D, 0xF0, 0x20,  // +1 X: $32, N+ Z-
        0x5D, 0xF0, 0x20,  // +1 X: $34, N- Z+
    });

    LoadData(0x1234, {
        0b10101010, 0,
        0b01010101, 0,
        0b01010101,
    });

    LoadData(0x2120, {
        0b10101010, 0,
        0b01010101, 0,
        0b01010101,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b01010101);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x34);
    cpu->SetRegisterA(0b01010101);
    EXECUTE(5);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("EOR abs16, Y", "EOR-abs16, Y") {  // 0x59
    LoadData(0x200, {
        0x59, 0x34, 0x12,  // Y: $00, N- Z-
        0x59, 0x34, 0x12,  // Y: $02, N+ Z-
        0x59, 0x34, 0x12,  // Y: $04, N- Z+

        0x59, 0xF0, 0x20,  // +1 Y: $30, N- Z-
        0x59, 0xF0, 0x20,  // +1 Y: $32, N+ Z-
        0x59, 0xF0, 0x20,  // +1 Y: $34, N- Z+
    });

    LoadData(0x1234, {
        0b10101010, 0,
        0b01010101, 0,
        0b01010101,
    });

    LoadData(0x2120, {
        0b10101010, 0,
        0b01010101, 0,
        0b01010101,
    });

    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0b01010101);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x34);
    cpu->SetRegisterA(0b01010101);
    EXECUTE(5);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("EOR (imm8, X)", "EOR-(imm8, X)") {  // 0x41
    LoadData(0x200, {
        0x41, 0x80,  // X: $00, N- Z-
        0x41, 0x80,  // X: $02, N+ Z-
        0x41, 0x80,  // X: $04, N- Z+

        0x41, 0xF0,  // X: $20, N- Z-
        0x41, 0xF0,  // X: $22, N+ Z-
        0x41, 0xF0,  // X: $24, N- Z+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x36, 0x12,
        0x38, 0x12,
    });

    LoadData(0x10, {
        0x34, 0x12,
        0x36, 0x12,
        0x38, 0x12,
    });

    LoadData(0x1234, {
        0b10101010, 0,
        0b01010101, 0,
        0b01010101,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b01010101);
    EXECUTE(6);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b01010101);
    EXECUTE(6);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("EOR (imm8), Y", "EOR-(imm8),Y") {  // 0x51
    LoadData(0x200, {
        0x51, 0x80,  // Y: $00, N- Z-
        0x51, 0x80,  // Y: $02, N+ Z-
        0x51, 0x80,  // Y: $04, N- Z+

        0x51, 0x82,  // +1 Y: $80, N- Z-
        0x51, 0x82,  // +1 Y: $82, N+ Z-
        0x51, 0x82,  // +1 Y: $84, N- Z+
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC0, 0x11,
    });

    LoadData(0x1240, {
        0b10101010, 0,
        0b01010101, 0,
        0b01010101,
    });

    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0b01010101);
    EXECUTE(5);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b01011010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b10100101);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x84);
    cpu->SetRegisterA(0b01010101);
    EXECUTE(6);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("ORA imm8", "ORA-imm8") {  // 0x09
    LoadData(0x200, {
        0x09, 0b01010101,  // N- Z-
        0x09, 0b10101010,  // N+ Z-
        0x09, 0b00000000,  // N- Z+
    });

    cpu->SetRegisterA(0b01110000);
    EXECUTE(2);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(2);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b00000000);
    EXECUTE(2);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("ORA zp", "ORA-zp") {  // 0x05
    LoadData(0x200, {
        0x05, 0x80,  // N- Z-
        0x05, 0x82,  // N+ Z-
        0x05, 0x84,  // N- Z+
    });

    LoadData(0x80, {
        0b01010101, 0,
        0b10101010, 0,
        0b00000000,
    });

    cpu->SetRegisterA(0b01110000);
    EXECUTE(3);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(3);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b00000000);
    EXECUTE(3);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("ORA zp, X", "ORA-zp,X") {  // 0x15
    LoadData(0x200, {
        0x15, 0x80,  // X: $00, N- Z-
        0x15, 0x80,  // X: $02, N+ Z-
        0x15, 0x80,  // X: $04, N- Z+
    });

    LoadData(0x80, {
        0b01010101, 0,
        0b10101010, 0,
        0b00000000,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b01110000);
    EXECUTE(4);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("ORA abs16", "ORA-abs16") {  // 0x0D
    LoadData(0x200, {
        0x0D, 0x34, 0x12,  // N- Z-
        0x0D, 0x36, 0x12,  // N+ Z-
        0x0D, 0x38, 0x12,  // N- Z+
    });

    LoadData(0x1234, {
        0b01010101, 0,
        0b10101010, 0,
        0b00000000,
    });

    cpu->SetRegisterA(0b01110000);
    EXECUTE(4);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b00000000);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("ORA abs16, X", "ORA-abs16, X") {  // 0x1D
    LoadData(0x200, {
        0x1D, 0x34, 0x12,  // X: $00, N- Z-
        0x1D, 0x34, 0x12,  // X: $02, N+ Z-
        0x1D, 0x34, 0x12,  // X: $04, N- Z+

        0x1D, 0xF0, 0x20,  // +1 X: $30, N- Z-
        0x1D, 0xF0, 0x20,  // +1 X: $32, N+ Z-
        0x1D, 0xF0, 0x20,  // +1 X: $34, N- Z+
    });

    LoadData(0x1234, {
        0b01010101, 0,
        0b10101010, 0,
        0b00000000,
    });

    LoadData(0x2120, {
        0b01010101, 0,
        0b10101010, 0,
        0b00000000,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b01110000);
    EXECUTE(4);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b01110000);
    EXECUTE(5);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x34);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(5);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("ORA abs16, Y", "ORA-abs16, Y") {  // 0x19
    LoadData(0x200, {
        0x19, 0x34, 0x12,  // X: $00, N- Z-
        0x19, 0x34, 0x12,  // X: $02, N+ Z-
        0x19, 0x34, 0x12,  // X: $04, N- Z+

        0x19, 0xF0, 0x20,  // +1 X: $30, N- Z-
        0x19, 0xF0, 0x20,  // +1 X: $32, N+ Z-
        0x19, 0xF0, 0x20,  // +1 X: $34, N- Z+
    });

    LoadData(0x1234, {
        0b01010101, 0,
        0b10101010, 0,
        0b00000000,
    });

    LoadData(0x2120, {
        0b01010101, 0,
        0b10101010, 0,
        0b00000000,
    });

    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b01110000);
    EXECUTE(4);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(4);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(4);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b01110000);
    EXECUTE(5);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x34);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(5);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("ORA (imm8, X)", "ORA-(imm8, X)") {  // 0x01
    LoadData(0x200, {
        0x01, 0x80,  // X: $00, N- Z-
        0x01, 0x80,  // X: $02, N+ Z-
        0x01, 0x80,  // X: $04, N- Z+

        0x01, 0xF0,  // X: $20, N- Z-
        0x01, 0xF0,  // X: $22, N+ Z-
        0x01, 0xF0,  // X: $24, N- Z+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x36, 0x12,
        0x38, 0x12,
    });

    LoadData(0x10, {
        0x34, 0x12,
        0x36, 0x12,
        0x38, 0x12,
    });

    LoadData(0x1234, {
        0b01010101, 0,
        0b10101010, 0,
        0b00000000,
    });

    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b01110000);
    EXECUTE(6);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(6);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b01110000);
    EXECUTE(6);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(6);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("ORA (imm8), Y", "ORA-(imm8),Y") {  // 0x11
    LoadData(0x200, {
        0x11, 0x80,  // Y: $00, N- Z-
        0x11, 0x80,  // Y: $02, N+ Z-
        0x11, 0x80,  // Y: $04, N- Z+

        0x11, 0x82,  // +1 Y: $80, N- Z-
        0x11, 0x82,  // +1 Y: $82, N+ Z-
        0x11, 0x82,  // +1 Y: $84, N- Z+
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC0, 0x11,
    });

    LoadData(0x1240, {
        0b01010101, 0,
        0b10101010, 0,
        0b00000000,
    });

    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b01110000);
    EXECUTE(5);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(5);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x04);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(5);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b01110000);
    EXECUTE(6);
    CHECK_A(0b01110101);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b11110000);
    EXECUTE(6);
    CHECK_A(0b11111010);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterY(0x84);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(6);
    CHECK_A(0);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
}

TEST("BIT zp", "BIT-zp") {  // 0x24
    LoadData(0x200, {
        0x24, 0x80,  // N- V- Z-
        0x24, 0x82,  // N+ V- Z-
        0x24, 0x84,  // N- V+ Z-
        0x24, 0x86,  // N+ V+ Z-
        0x24, 0x88,  // N- V- Z+
    });

    LoadData(0x80, {
        0b00010101, 0,
        0b10101010, 0,
        0b01010101, 0,
        0b11010101, 0,
        0b01010101, 0,
    });

    cpu->SetRegisterA(0b11110000);
    EXECUTE(2);
    CHECK_A(0b11110000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(2);
    CHECK_A(0b11110000);
    CHECK_NEGATIVE_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(2);
    CHECK_A(0b11110000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(2);
    CHECK_A(0b11110000);
    CHECK_NEGATIVE_SET;
    CHECK_OVERFLOW_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b10101010);
    EXECUTE(2);
    CHECK_A(0b10101010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_ZERO_SET;
}

TEST("BIT abs16", "BIT-abs16") {  // 0x2C
    LoadData(0x200, {
        0x2C, 0x34, 0x12,  // N- V- Z-
        0x2C, 0x36, 0x12,  // N+ V- Z-
        0x2C, 0x38, 0x12,  // N- V+ Z-
        0x2C, 0x3A, 0x12,  // N+ V+ Z-
        0x2C, 0x3C, 0x12,  // N- V- Z+
    });

    LoadData(0x1234, {
        0b00010101, 0,
        0b10101010, 0,
        0b01010101, 0,
        0b11010101, 0,
        0b01010101, 0,
    });

    cpu->SetRegisterA(0b11110000);
    EXECUTE(3);
    CHECK_A(0b11110000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(3);
    CHECK_A(0b11110000);
    CHECK_NEGATIVE_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(3);
    CHECK_A(0b11110000);
    CHECK_NEGATIVE_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b11110000);
    EXECUTE(3);
    CHECK_A(0b11110000);
    CHECK_NEGATIVE_SET;
    CHECK_OVERFLOW_SET;
    CHECK_ZERO_CLEAR;

    cpu->SetRegisterA(0b10101010);
    EXECUTE(3);
    CHECK_A(0b10101010);
    CHECK_NEGATIVE_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_ZERO_SET;
}
