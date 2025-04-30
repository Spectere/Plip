/* Mos6502.LoadStore.cpp
 *
 * Tests the CPU core's ability to manipulate the stack.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

TEST("TSX", "TSX") {  // 0xBA
    LoadData(0x200, {
        0xBA,  // N- Z-
        0xBA,  // N+ Z-
        0xBA,  // N- Z+
    });

    // N- Z-
    cpu->SetRegisterS(0x12);
    cpu->SetRegisterX(0xFF);
    auto cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterX() == 0x12);
    CHECK(cycles == 2);

    // N+ Z-
    cpu->SetRegisterS(0x8A);
    cpu->SetRegisterX(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterX() == 0x8A);
    CHECK(cycles == 2);

    // N- Z+
    cpu->SetRegisterS(0x00);
    cpu->SetRegisterX(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cpu->GetRegisterX() == 0x00);
    CHECK(cycles == 2);
}

TEST("TXS", "TXS") {  // 0x9A
    LoadData(0x200, {
        0x9A,  // N- Z-
        0x9A,  // N+ Z-
        0x9A,  // N- Z+
    });

    // N- Z-
    cpu->SetRegisterX(0x12);
    cpu->SetRegisterS(0xFF);
    auto cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterS() == 0x12);
    CHECK(cycles == 2);

    // N+ Z-
    cpu->SetRegisterX(0x8A);
    cpu->SetRegisterS(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterS() == 0x8A);
    CHECK(cycles == 2);

    // N- Z+
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterS(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cpu->GetRegisterS() == 0x00);
    CHECK(cycles == 2);
}

TEST("PHA", "PHA") {  // 0x48
    LoadData(0x200, {
        0x48,
    });

    cpu->SetRegisterA(0x12);
    CHECK(cpu->GetRegisterS() == 0xFF);
    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterS() == 0xFE);
    CHECK_RAM(0x01FF, 0x12);
    CHECK(cycles == 3);
}

TEST("PHP", "PHP") {  // 0x08
    LoadData(0x200, {
        0x08,  // 0b00110100
        0x08,  // 0b00100000
        0x08,  // 0b11111111
    });

    CHECK(cpu->GetRegisterS() == 0xFF);
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterS() == 0xFE);
    CHECK_RAM(0x01FF, 0b00110100);
    CHECK(cycles == 3);

    cpu->ClearBreakCommand();
    cpu->ClearCarryFlag();
    cpu->ClearDecimalMode();
    cpu->ClearInterruptDisable();
    cpu->ClearNegativeFlag();
    cpu->ClearOverflowFlag();
    cpu->ClearZeroFlag();
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterS() == 0xFD);
    CHECK_RAM(0x01FE, 0b00100000);
    CHECK(cycles == 3);

    cpu->SetBreakCommand();
    cpu->SetCarryFlag();
    cpu->SetDecimalMode();
    cpu->SetInterruptDisable();
    cpu->SetNegativeFlag();
    cpu->SetOverflowFlag();
    cpu->SetZeroFlag();
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterS() == 0xFC);
    CHECK_RAM(0x01FD, 0b11111111);
    CHECK(cycles == 3);
}

TEST("PLA", "PLA") {  // 0x68
    LoadData(0x200, {
        0x68,  // N- Z-
        0x68,  // N- Z+
        0x68,  // N+ Z-
    });

    LoadData(0x1FD, {
        0x12,
        0x00,
        0x8A,
    });

    cpu->SetRegisterS(0xFC);

    // N- Z-
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK(cpu->GetRegisterS() == 0xFD);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    // N- Z+
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK(cpu->GetRegisterS() == 0xFE);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);

    // N+ Z-
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterA() == 0x8A);
    CHECK(cpu->GetRegisterS() == 0xFF);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);
}

TEST("PLP", "PLP") {  // 0x28
    LoadData(0x200, {
        0x28,  // All-
        0x28,  // All+
    });

    LoadData(0x1FE, {
        0x00,
        0xFF,
    });

    cpu->SetRegisterS(0xFD);

    // All-
    auto cycles = cpu->Step();
    CHECK(cpu->GetRegisterS() == 0xFE);
    CHECK_BREAK_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK_DECIMAL_MODE_DISABLED;
    CHECK_INTERRUPT_DISABLED;
    CHECK_NEGATIVE_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 4);

    // All+
    cycles = cpu->Step();
    CHECK(cpu->GetRegisterS() == 0xFF);
    CHECK_BREAK_SET;
    CHECK_CARRY_SET;
    CHECK_DECIMAL_MODE_ENABLED;
    CHECK_INTERRUPT_ENABLED;
    CHECK_NEGATIVE_SET;
    CHECK_OVERFLOW_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 4);
}

TEST("PLP-Zero", "PLP-Zero") {  // 0x28
    // Ensures that using PLP on a zero value will still result in the flags register reading 0b0010000.
    LoadData(0x200, {
        0x28,  // PLP
        0x08,  // PHP
    });

    LoadData(0x1FF, { 0x00 });

    cpu->SetRegisterS(0xFE);

    cpu->Step();
    cpu->Step();
    CHECK_RAM(0x01FF, 0b00100000);
}
