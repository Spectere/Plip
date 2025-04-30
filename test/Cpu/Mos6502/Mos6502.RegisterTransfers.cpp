/* Mos6502.LoadStore.cpp
 *
 * Tests the CPU core's ability to transfer data between registers.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

TEST("TAX", "TAX") {  // 0xAA
    LoadData(0x200, {
        0xAA,  // N- Z-
        0xAA,  // N+ Z-
        0xAA,  // N- Z+
    });

    // N- Z-
    cpu->SetRegisterA(0x12);
    cpu->SetRegisterX(0xFF);
    auto cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterX() == 0x12);
    CHECK(cycles == 2);

    // N+ Z-
    cpu->SetRegisterA(0x8A);
    cpu->SetRegisterX(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterX() == 0x8A);
    CHECK(cycles == 2);

    // N- Z+
    cpu->SetRegisterA(0x00);
    cpu->SetRegisterX(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cpu->GetRegisterX() == 0x00);
    CHECK(cycles == 2);
}

TEST("TAY", "TAY") {  // 0xA8
    LoadData(0x200, {
        0xA8,  // N- Z-
        0xA8,  // N+ Z-
        0xA8,  // N- Z+
    });

    // N- Z-
    cpu->SetRegisterA(0x12);
    cpu->SetRegisterY(0xFF);
    auto cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterY() == 0x12);
    CHECK(cycles == 2);

    // N+ Z-
    cpu->SetRegisterA(0x8A);
    cpu->SetRegisterY(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterY() == 0x8A);
    CHECK(cycles == 2);

    // N- Z+
    cpu->SetRegisterA(0x00);
    cpu->SetRegisterY(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cpu->GetRegisterY() == 0x00);
    CHECK(cycles == 2);
}

TEST("TXA", "TXA") {  // 0x8A
    LoadData(0x200, {
        0x8A,  // N- Z-
        0x8A,  // N+ Z-
        0x8A,  // N- Z+
    });

    // N- Z-
    cpu->SetRegisterX(0x12);
    cpu->SetRegisterA(0xFF);
    auto cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterA() == 0x12);
    CHECK(cycles == 2);

    // N+ Z-
    cpu->SetRegisterX(0x8A);
    cpu->SetRegisterA(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterA() == 0x8A);
    CHECK(cycles == 2);

    // N- Z+
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cpu->GetRegisterA() == 0x00);
    CHECK(cycles == 2);
}

TEST("TYA", "TYA") {  // 0x98
    LoadData(0x200, {
        0x98,  // N- Z-
        0x98,  // N+ Z-
        0x98,  // N- Z+
    });

    // N- Z-
    cpu->SetRegisterY(0x12);
    cpu->SetRegisterA(0xFF);
    auto cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterY() == 0x12);
    CHECK(cycles == 2);

    // N+ Z-
    cpu->SetRegisterY(0x8A);
    cpu->SetRegisterA(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cpu->GetRegisterY() == 0x8A);
    CHECK(cycles == 2);

    // N- Z+
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0xFF);
    cycles = cpu->Step();
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cpu->GetRegisterY() == 0x00);
    CHECK(cycles == 2);
}
