/* Mos6502.LoadStore.cpp
 *
 * Tests the CPU core's ability to loads and store values into registers.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

TEST("LDA imm8", "LDA-imm8") {  // 0xA9
    LoadData(0x200, {
        0xA9, 0x12,  // N- Z-
        0xA9, 0x00,  // N- Z+
        0xA9, 0x88,  // N+ Z-
    });

    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("LDA zp", "LDA-zp") {  // 0xA5
    LoadData(0x200, {
        0xA5, 0x01,  // N- Z-
        0xA5, 0x02,  // N- Z+
        0xA5, 0x03,  // N+ Z-
    });

    LoadData(0x01, {
        0x12,
        0x00,
        0x88,
    });

    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 3);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);
}

TEST("LDA zp, X", "LDA-zp,X") {  // 0xB5
    LoadData(0x200, {
        0xB5, 0x40,  // X = 0x41, N- Z-
        0xB5, 0x40,  // X = 0x42, N- Z+
        0xB5, 0x40,  // X = 0x43, N+ Z-
        0xB5, 0xA0,  // X = 0xE1, N- Z-
        0xB5, 0xA0,  // X = 0xE2, N- Z+
        0xB5, 0xA0,  // X = 0xE3, N+ Z-
    });

    LoadData(0x81, {
        0x12,
        0x00,
        0x88,
    });

    cpu->SetRegisterX(0x41);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x42);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x43);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0xE1);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0xE2);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0xE3);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);
}

TEST("LDA abs16", "LDA-abs16") {  // 0xAD
    LoadData(0x200, {
        0xAD, 0x34, 0x12,  // N- Z-
        0xAD, 0x35, 0x12,  // N- Z+
        0xAD, 0x36, 0x12,  // N+ Z-
    });

    LoadData(0x1234, {
        0x12,
        0x00,
        0x88,
    });

    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);
}

TEST("LDA abs16, X", "LDA-abs16,X") {  // 0xBD
    LoadData(0x200, {
        0xBD, 0x34, 0x12,  // X = 0x02, N- Z-
        0xBD, 0x34, 0x12,  // X = 0x04, N- Z+
        0xBD, 0x34, 0x12,  // X = 0x06, N+ Z-
        0xBD, 0xF0, 0x1F,  // X = 0x12, N- Z-
        0xBD, 0xF0, 0x1F,  // X = 0x14, N- Z+
        0xBD, 0xF0, 0x1F,  // X = 0x16, N+ Z-
    });

    LoadData(0x1236, {
        0x12, 0xFF,
        0x00, 0xFF,
        0x88, 0xFF,
    });

    LoadData(0x2002, {
        0x24, 0xFF,
        0x00, 0xFF,
        0x99, 0xFF,
    });

    cpu->SetRegisterX(0x02);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x04);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x06);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x12);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x24);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);

    cpu->SetRegisterX(0x14);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 5);

    cpu->SetRegisterX(0x16);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x99);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);
}

TEST("LDA abs16, Y", "LDA-abs16,Y") {  // 0xB9
    LoadData(0x200, {
        0xB9, 0x34, 0x12,  // Y = 0x02, N- Z-
        0xB9, 0x34, 0x12,  // Y = 0x04, N- Z+
        0xB9, 0x34, 0x12,  // Y = 0x06, N+ Z-
        0xB9, 0xF0, 0x1F,  // Y = 0x12, N- Z-
        0xB9, 0xF0, 0x1F,  // Y = 0x14, N- Z+
        0xB9, 0xF0, 0x1F,  // Y = 0x16, N+ Z-
    });

    LoadData(0x1236, {
        0x12, 0xFF,
        0x00, 0xFF,
        0x88, 0xFF,
    });

    LoadData(0x2002, {
        0x24, 0xFF,
        0x00, 0xFF,
        0x99, 0xFF,
    });

    cpu->SetRegisterY(0x02);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0x04);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0x06);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0x12);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x24);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);

    cpu->SetRegisterY(0x14);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 5);

    cpu->SetRegisterY(0x16);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x99);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);
}

TEST("LDA (imm8, X)", "LDA-(imm8,X)") {  // 0xA1
    LoadData(0x200, {
        0xA1, 0xFA,  // X = 0x00, N- Z-
        0xA1, 0xFA,  // X = 0x02, N- Z+
        0xA1, 0xFA,  // X = 0x04, N+ Z-
        0xA1, 0xFA,  // X = 0x06, N- Z-
        0xA1, 0xFA,  // X = 0x08, N- Z+
        0xA1, 0xFA,  // X = 0x0A, N+ Z-
    });

    LoadData(0x1234, {
        0x12, 0xFF,
        0x00, 0xFF,
        0x88, 0xFF,
    });

    LoadData(0x1064, {
        0x24, 0xFF,
        0x00, 0xFF,
        0x99, 0xFF,
    });

    LoadData(0x00, {
        0x64, 0x10,
        0x66, 0x10,
        0x68, 0x10,
    });

    LoadData(0xFA, {
        0x34, 0x12,
        0x36, 0x12,
        0x38, 0x12,
    });

    cpu->SetRegisterX(0x00);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 6);

    cpu->SetRegisterX(0x02);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 6);

    cpu->SetRegisterX(0x04);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 6);

    cpu->SetRegisterX(0x06);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x24);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 6);

    cpu->SetRegisterX(0x08);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 6);

    cpu->SetRegisterX(0x0A);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x99);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 6);
}

TEST("LDA (imm8), Y", "LDA-(imm8),Y") {  // 0xB1
    LoadData(0x200, {
        0xB1, 0x10,  // Y = 0x02, N- Z-
        0xB1, 0x10,  // Y = 0x04, N- Z+
        0xB1, 0x10,  // Y = 0x06, N+ Z-
        0xB1, 0x20,  // Y = 0x12, N- Z-
        0xB1, 0x20,  // Y = 0x14, N- Z+
        0xB1, 0x20,  // Y = 0x16, N+ Z-
    });

    LoadData(0x10, { 0x34, 0x12 });
    LoadData(0x20, { 0xF0, 0x1F });

    LoadData(0x1236, {
        0x12, 0xFF,
        0x00, 0xFF,
        0x88, 0xFF,
    });

    LoadData(0x2002, {
        0x24, 0xFF,
        0x00, 0xFF,
        0x99, 0xFF,
    });

    cpu->SetRegisterY(0x02);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);

    cpu->SetRegisterY(0x04);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 5);

    cpu->SetRegisterY(0x06);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);

    cpu->SetRegisterY(0x12);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x24);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 6);

    cpu->SetRegisterY(0x14);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 6);

    cpu->SetRegisterY(0x16);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x99);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 6);
}

TEST("LDX imm8", "LDX-imm8") {  // 0xA2
    LoadData(0x200, {
        0xA2, 0x12,  // N- Z-
        0xA2, 0x00,  // N- Z+
        0xA2, 0x88,  // N+ Z-
    });

    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("LDX zp", "LDX-zp") {  // 0xA6
    LoadData(0x200, {
        0xA6, 0x01,  // N- Z-
        0xA6, 0x02,  // N- Z+
        0xA6, 0x03,  // N+ Z-
    });

    LoadData(0x01, {
        0x12,
        0x00,
        0x88,
    });

    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 3);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);
}

TEST("LDX zp, Y", "LDX-zp,y") {  // 0xB6
    LoadData(0x200, {
        0xB6, 0x40,  // Y = 0x41, N- Z-
        0xB6, 0x40,  // Y = 0x42, N- Z+
        0xB6, 0x40,  // Y = 0x43, N+ Z-
        0xB6, 0xA0,  // Y = 0xE1, N- Z-
        0xB6, 0xA0,  // Y = 0xE2, N- Z+
        0xB6, 0xA0,  // Y = 0xE3, N+ Z-
    });

    LoadData(0x81, {
        0x12,
        0x00,
        0x88,
    });

    cpu->SetRegisterY(0x41);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0x42);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0x43);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0xE1);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0xE2);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0xE3);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);
}

TEST("LDX abs16", "LDX-abs16") {  // 0xAE
    LoadData(0x200, {
        0xAE, 0x34, 0x12,  // N- Z-
        0xAE, 0x35, 0x12,  // N- Z+
        0xAE, 0x36, 0x12,  // N+ Z-
    });

    LoadData(0x1234, {
        0x12,
        0x00,
        0x88,
    });

    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);
}

TEST("LDX abs16, Y", "LDX-abs16,Y") {  // 0xBE
    LoadData(0x200, {
        0xBE, 0x34, 0x12,  // Y = 0x02, N- Z-
        0xBE, 0x34, 0x12,  // Y = 0x04, N- Z+
        0xBE, 0x34, 0x12,  // Y = 0x06, N+ Z-
        0xBE, 0xF0, 0x1F,  // Y = 0x12, N- Z-
        0xBE, 0xF0, 0x1F,  // Y = 0x14, N- Z+
        0xBE, 0xF0, 0x1F,  // Y = 0x16, N+ Z-
    });

    LoadData(0x1236, {
        0x12, 0xFF,
        0x00, 0xFF,
        0x88, 0xFF,
    });

    LoadData(0x2002, {
        0x24, 0xFF,
        0x00, 0xFF,
        0x99, 0xFF,
    });

    cpu->SetRegisterY(0x02);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0x04);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0x06);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterY(0x12);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x24);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);

    cpu->SetRegisterY(0x14);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 5);

    cpu->SetRegisterY(0x16);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterX() == 0x99);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);
}

TEST("LDY imm8", "LDY-imm8") {  // 0xA0
    LoadData(0x200, {
        0xA0, 0x12,  // N- Z-
        0xA0, 0x00,  // N- Z+
        0xA0, 0x88,  // N+ Z-
    });

    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("LDY zp", "LDY-zp") {  // 0xA4
    LoadData(0x200, {
        0xA4, 0x01,  // N- Z-
        0xA4, 0x02,  // N- Z+
        0xA4, 0x03,  // N+ Z-
    });

    LoadData(0x01, {
        0x12,
        0x00,
        0x88,
    });

    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 3);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);
}

TEST("LDY zp, X", "LDY-zp,X") {  // 0xB4
    LoadData(0x200, {
        0xB4, 0x40,  // X = 0x41, N- Z-
        0xB4, 0x40,  // X = 0x42, N- Z+
        0xB4, 0x40,  // X = 0x43, N+ Z-
        0xB4, 0xA0,  // X = 0xE1, N- Z-
        0xB4, 0xA0,  // X = 0xE2, N- Z+
        0xB4, 0xA0,  // X = 0xE3, N+ Z-
    });

    LoadData(0x81, {
        0x12,
        0x00,
        0x88,
    });

    cpu->SetRegisterX(0x41);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x42);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x43);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0xE1);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0xE2);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0xE3);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);
}

TEST("LDY abs16", "LDY-abs16") {  // 0xAC
    LoadData(0x200, {
        0xAC, 0x34, 0x12,  // N- Z-
        0xAC, 0x35, 0x12,  // N- Z+
        0xAC, 0x36, 0x12,  // N+ Z-
    });

    LoadData(0x1234, {
        0x12,
        0x00,
        0x88,
    });

    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);
}

TEST("LDY abs16, X", "LDY-abs16,X") {  // 0xBC
    LoadData(0x200, {
        0xBC, 0x34, 0x12,  // X = 0x02, N- Z-
        0xBC, 0x34, 0x12,  // X = 0x04, N- Z+
        0xBC, 0x34, 0x12,  // X = 0x06, N+ Z-
        0xBC, 0xF0, 0x1F,  // X = 0x12, N- Z-
        0xBC, 0xF0, 0x1F,  // X = 0x14, N- Z+
        0xBC, 0xF0, 0x1F,  // X = 0x16, N+ Z-
    });

    LoadData(0x1236, {
        0x12, 0xFF,
        0x00, 0xFF,
        0x88, 0xFF,
    });

    LoadData(0x2002, {
        0x24, 0xFF,
        0x00, 0xFF,
        0x99, 0xFF,
    });

    cpu->SetRegisterX(0x02);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x12);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x04);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x06);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x88);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    cpu->SetRegisterX(0x12);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x24);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);

    cpu->SetRegisterX(0x14);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x00);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 5);

    cpu->SetRegisterX(0x16);
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterY() == 0x99);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 5);
}

TEST("STA zp", "STA-zp") {  // 0x85
    LoadData(0x200, {
        0x85, 0x80,
    });

    cpu->SetRegisterA(0x12);
    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x80) == 0x12);
    CHECK(cycles == 3);
}

TEST("STA zp, X", "STA-zp,X") {  // 0x95
    LoadData(0x200, {
        0x95, 0x80,
    });

    cpu->SetRegisterA(0x12);
    cpu->SetRegisterX(0x10);
    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x90) == 0x12);
    CHECK(cycles == 4);
}

TEST("STA abs16", "STA-abs16") {  // 0x8D
    LoadData(0x200, {
        0x8D, 0x34, 0x12,
    });

    cpu->SetRegisterA(0x8A);
    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x1234) == 0x8A);
    CHECK(cycles == 4);
}

TEST("STA abs16, X", "STA-abs16,X") {  // 0x9D
    LoadData(0x200, {
        0x9D, 0x34, 0x12,  // X: 0x10, addr: 0x1244
        0x9D, 0xFF, 0x12,  // X: 0x02, addr: 0x1301
    });

    cpu->SetRegisterA(0x8A);
    cpu->SetRegisterX(0x10);
    auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x1244) == 0x8A);
    CHECK(cycles == 5);

    cpu->SetRegisterX(0x02);
    cycles = cpu->Step();
    CHECK(memory->GetByte(0x1301) == 0x8A);
    CHECK(cycles == 5);
}

TEST("STA abs16, Y", "STA-abs16,Y") {  // 0x99
    LoadData(0x200, {
        0x99, 0x34, 0x12,  // Y: 0x10, addr: 0x1244
        0x99, 0xFF, 0x12,  // Y: 0x02, addr: 0x1301
    });

    cpu->SetRegisterA(0xA8);
    cpu->SetRegisterY(0x10);
    auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x1244) == 0xA8);
    CHECK(cycles == 5);

    cpu->SetRegisterY(0x02);
    cycles = cpu->Step();
    CHECK(memory->GetByte(0x1301) == 0xA8);
    CHECK(cycles == 5);
}

TEST("STA (imm8, X)", "STA-(imm8,X)") {  // 0x81
    LoadData(0x200, {
        0x81, 0xA0,  // X: 0x10, zp: 0xB0
        0x81, 0xF0,  // X: 0x20, zp: 0x10
    });

    LoadData(0xB0, { 0x56, 0x34 });
    LoadData(0x10, { 0x34, 0x12 });

    cpu->SetRegisterA(0x8A);
    cpu->SetRegisterX(0x10);
    auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x3456) == 0x8A);
    CHECK(cycles == 6);

    cpu->SetRegisterA(0x8B);
    cpu->SetRegisterX(0x20);
    cycles = cpu->Step();
    CHECK(memory->GetByte(0x1234) == 0x8B);
    CHECK(cycles == 6);
}

TEST("STA (imm8), Y", "STA-(imm8),Y") {  // 0x91
    LoadData(0x200, {
        0x91, 0xA0,  // Y: 0x00
        0x91, 0xA0,  // Y: 0x02
    });

    LoadData(0xA0, { 0x34, 0x12 });

    cpu->SetRegisterA(0xA8);
    cpu->SetRegisterY(0x00);
    auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x1234) == 0xA8);
    CHECK(cycles == 6);

    cpu->SetRegisterA(0xA9);
    cpu->SetRegisterY(0x02);
    cycles = cpu->Step();
    CHECK(memory->GetByte(0x1236) == 0xA9);
    CHECK(cycles == 6);
}

TEST("STX zp", "STX-zp") {  // 0x86
    LoadData(0x200, {
        0x86, 0x80,
    });

    cpu->SetRegisterX(0x8A);
    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x80) == 0x8A);
    CHECK(cycles == 3);
}

TEST("STX zp, Y", "STX-zp,Y") {  // 0x96
    LoadData(0x200, {
        0x96, 0x80,
    });

    cpu->SetRegisterX(0x8A);
    cpu->SetRegisterY(0x10);
    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x90) == 0x8A);
    CHECK(cycles == 4);
}

TEST("STX abs16", "STX-abs16") {  // 0x8E
    LoadData(0x200, {
        0x8E, 0x34, 0x12,
    });

    cpu->SetRegisterX(0x8A);
    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x1234) == 0x8A);
    CHECK(cycles == 4);
}

TEST("STY zp", "STY-zp") {  // 0x84
    LoadData(0x200, {
        0x84, 0x80,
    });

    cpu->SetRegisterY(0x8A);
    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x80) == 0x8A);
    CHECK(cycles == 3);
}

TEST("STY zp, X", "STY-zp,X") {  // 0x94
    LoadData(0x200, {
        0x94, 0x80,
    });

    cpu->SetRegisterY(0x8A);
    cpu->SetRegisterX(0x10);
    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x90) == 0x8A);
    CHECK(cycles == 4);
}

TEST("STY abs16", "STY-abs16") {  // 0x8C
    LoadData(0x200, {
        0x8C, 0x34, 0x12,
    });

    cpu->SetRegisterY(0x8A);
    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x1234) == 0x8A);
    CHECK(cycles == 4);
}
