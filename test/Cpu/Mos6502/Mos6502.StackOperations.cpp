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
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_X(0x12);

    // N+ Z-
    cpu->SetRegisterS(0x8A);
    cpu->SetRegisterX(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK_X(0x8A);

    // N- Z+
    cpu->SetRegisterS(0x00);
    cpu->SetRegisterX(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK_X(0x00);
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
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_S(0x12);

    // N+ Z-
    cpu->SetRegisterX(0x8A);
    cpu->SetRegisterS(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK_S(0x8A);

    // N- Z+
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterS(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK_S(0x00);
}

TEST("PHA", "PHA") {  // 0x48
    LoadData(0x200, {
        0x48,
    });

    cpu->SetRegisterA(0x12);
    CHECK_S(0xFF);
    EXECUTE(3);
    CHECK_S(0xFE);
    CHECK_RAM(0x01FF, 0x12);
}

TEST("PHP", "PHP") {  // 0x08
    LoadData(0x200, {
        0x08,  // 0b00110100
        0x08,  // 0b00100000
        0x08,  // 0b11111111
    });

    CHECK_S(0xFF);
    EXECUTE(3);
    CHECK_S(0xFE);
    CHECK_RAM(0x01FF, 0b00110100);

    cpu->ClearBreakCommand();
    cpu->ClearCarryFlag();
    cpu->ClearDecimalMode();
    cpu->ClearInterruptDisable();
    cpu->ClearNegativeFlag();
    cpu->ClearOverflowFlag();
    cpu->ClearZeroFlag();
    EXECUTE(3);
    CHECK_S(0xFD);
    CHECK_RAM(0x01FE, 0b00100000);

    cpu->SetBreakCommand();
    cpu->SetCarryFlag();
    cpu->SetDecimalMode();
    cpu->SetInterruptDisable();
    cpu->SetNegativeFlag();
    cpu->SetOverflowFlag();
    cpu->SetZeroFlag();
    EXECUTE(3);
    CHECK_S(0xFC);
    CHECK_RAM(0x01FD, 0b11111111);
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
    EXECUTE(4);
    CHECK_A(0x12);
    CHECK_S(0xFD);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;

    // N- Z+
    EXECUTE(4);
    CHECK_A(0x00);
    CHECK_S(0xFE);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;

    // N+ Z-
    EXECUTE(4);
    CHECK_A(0x8A);
    CHECK_S(0xFF);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
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
    EXECUTE(4);
    CHECK_S(0xFE);
    CHECK_BREAK_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK_DECIMAL_MODE_DISABLED;
    CHECK_INTERRUPT_DISABLED;
    CHECK_NEGATIVE_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_ZERO_CLEAR;

    // All+
    EXECUTE(4);
    CHECK_S(0xFF);
    CHECK_BREAK_SET;
    CHECK_CARRY_SET;
    CHECK_DECIMAL_MODE_ENABLED;
    CHECK_INTERRUPT_ENABLED;
    CHECK_NEGATIVE_SET;
    CHECK_OVERFLOW_SET;
    CHECK_ZERO_SET;
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
