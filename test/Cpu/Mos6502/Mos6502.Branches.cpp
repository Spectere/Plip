/* Mos6502.Branches.cpp
 *
 * Tests the CPU core's ability to perform branches.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

#define BRANCH_TEST(op, successSetup, failureSetup) { \
    constexpr int expectedCyclesFailure = 2; \
    constexpr int expectedCyclesSuccess = 3; \
    constexpr int expectedCyclesSuccessNewPage = 4; \
    \
    LoadData(0x200, { \
        op, 0x40, \
        op, 0x40, \
    }); \
    \
    LoadData(0x244, { \
        op, 0xF0, \
    }); \
    \
    LoadData(0x2F0, { \
        op, 0x20, \
    }); \
    \
    failureSetup; \
    EXECUTE(expectedCyclesFailure); \
    CHECK_PC(0x202); \
    \
    successSetup; \
    EXECUTE(expectedCyclesSuccess); \
    CHECK_PC(0x244); \
    \
    successSetup; \
    EXECUTE(expectedCyclesSuccess); \
    CHECK_PC(0x236); \
    \
    successSetup; \
    cpu->SetPc(0x2F0); \
    EXECUTE(expectedCyclesSuccessNewPage); \
    CHECK_PC(0x312); \
}

// Branch on carry clear
TEST("BCC", "BCC") {  // 0x90
    BRANCH_TEST(0x90, cpu->ClearCarryFlag(), cpu->SetCarryFlag());
}

// Branch on carry set
TEST("BCS", "BCS") {  // 0xB0
    BRANCH_TEST(0xB0, cpu->SetCarryFlag(), cpu->ClearCarryFlag());
}

// Branch on zero set
TEST("BEQ", "BEQ") {  // 0xF0
    BRANCH_TEST(0xF0, cpu->SetZeroFlag(), cpu->ClearZeroFlag());
}

// Branch on negative set
TEST("BMI", "BMI") {  // 0x30
    BRANCH_TEST(0x30, cpu->SetNegativeFlag(), cpu->ClearNegativeFlag());
}

// Branch on zero clear
TEST("BNE", "BNE") {  // 0xD0
    BRANCH_TEST(0xD0, cpu->ClearZeroFlag(), cpu->SetZeroFlag());
}

// Branch on negative clear
TEST("BPL", "BPL") {  // 0x10
    BRANCH_TEST(0x10, cpu->ClearNegativeFlag(), cpu->SetNegativeFlag());
}

// Branch on overflow clear
TEST("BVC", "BVC") {  // 0x50
    BRANCH_TEST(0x50, cpu->ClearOverflowFlag(), cpu->SetOverflowFlag());
}

// Branch on overflow set
TEST("BVS", "BVS") {  // 0x70
    BRANCH_TEST(0x70, cpu->SetOverflowFlag(), cpu->ClearOverflowFlag());
}
