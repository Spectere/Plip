/* Mos6502.System.cpp
 *
 * Tests the CPU core's system/miscellaneous instructions.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

TEST("BRK", "BRK") {  // 0x00
    constexpr int expectedCycles = 7;
    
    LoadData(0xFFFE, { 0x34, 0x12 });
    LoadData(0x200, 0x00);
    
    const uint8_t expectedStatus = cpu->GetRegisterF();

    EXECUTE(expectedCycles);
    CHECK_PC(0x1234);
    CHECK_S(0xFC);
    CHECK_BREAK_SET;
    CHECK_RAM(0x1FF, 0x02);
    CHECK_RAM(0x1FE, 0x01);
    CHECK_RAM(0x1FD, expectedStatus);
}

TEST("NOP", "NOP") {  // 0xEA
    constexpr int expectedCycles = 2;

    LoadData(0x200, 0xEA);
    EXECUTE(expectedCycles);
}

TEST("RTI", "RTI") {  // 0x40
    constexpr int expectedCycles = 6;

    LoadData(0x1FD, {
        0b00000000,  // Flags
        0x80, 0x40,  // Jump Destination
        0x40,        // Op
    });

    cpu->SetRegisterS(0xFC);

    EXECUTE(expectedCycles);
    CHECK_PC(0x4080);
    CHECK_F(0b00100000);  // Bit 5 should always be set.
    CHECK_S(0xFF);
}
