/* Mos6502.Shifts.cpp
 *
 * Tests the CPU core's ability to perform jumps and calls.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

//
// JMP
//
TEST("JMP abs16", "JMP-abs16") {  // 0x4C
    constexpr int expectedCycles = 3;

    LoadData(0x200, {
        0x4C, 0x34, 0x12,
    });

    EXECUTE(expectedCycles);
    CHECK_PC(0x1234);
}

TEST("JMP (ind16)", "JMP-(ind16)") {  // 0x6C
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x6C, 0x34, 0x12,
    });

    LoadData(0x1234, {
        0x68, 0x24,
    });

    EXECUTE(expectedCycles);
    CHECK_PC(0x2468);
}

//
// JSR
//
TEST("JSR", "JSR") {  // 0x20
    constexpr int expectedCycles = 3;

    LoadData(0x200, {
        0x20, 0x34, 0x12,
    });
    
    EXECUTE(expectedCycles);
    CHECK_PC(0x1234);
    CHECK_S(0xFD);
    CHECK_RAM(0x01FF, 0x02);
    CHECK_RAM(0x01FE, 0x02);
}

//
// RTS
//
TEST("RTS", "RTS") {  // 0x60
    constexpr int expectedCycles = 6;

    LoadData(0x200, 0x60);

    cpu->SetRegisterS(0xFD);
    LoadData(0x1FE, {
        0x34, 0x12,
    });

    EXECUTE(expectedCycles);
    CHECK_S(0xFF);
    CHECK_PC(0x1234);
}
