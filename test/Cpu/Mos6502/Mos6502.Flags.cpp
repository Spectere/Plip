/* Mos6502.LoadStore.cpp
 *
 * Tests the CPU core's ability to set and clear flags programmatically.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

#define FLAG_TEST(op, setup, conditional) { \
    constexpr int expectedCycles = 2; \
    \
    LoadData(0x200, op); \
    \
    setup; \
    EXECUTE(expectedCycles); \
    conditional; \
}

TEST("CLC", "CLC") {  // 0x18
    FLAG_TEST(0x18, cpu->SetCarryFlag(), CHECK_CARRY_CLEAR);
}

TEST("CLD", "CLD") {  // 0xD8
    FLAG_TEST(0xD8, cpu->SetDecimalMode(), CHECK_DECIMAL_MODE_CLEAR);
}

TEST("CLI", "CLI") {  // 0x58
    FLAG_TEST(0x58, cpu->SetInterruptDisable(), CHECK_INTERRUPT_DISABLE_CLEAR);
}

TEST("CLV", "CLV") {  // 0xB8
    FLAG_TEST(0xB8, cpu->SetOverflowFlag(), CHECK_OVERFLOW_CLEAR);
}

TEST("SEC", "SEC") {  // 0x38
    FLAG_TEST(0x38, cpu->ClearCarryFlag(), CHECK_CARRY_SET);
}

TEST("SED", "SED") {  // 0xF8
    FLAG_TEST(0xF8, cpu->ClearDecimalMode(), CHECK_DECIMAL_MODE_SET);
}

TEST("SEI", "SEI") {  // 0x78
    FLAG_TEST(0x78, cpu->ClearInterruptDisable(), CHECK_INTERRUPT_DISABLE_SET);
}
