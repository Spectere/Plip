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
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_X(0x12);

    // N+ Z-
    cpu->SetRegisterA(0x8A);
    cpu->SetRegisterX(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK_X(0x8A);

    // N- Z+
    cpu->SetRegisterA(0x00);
    cpu->SetRegisterX(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK_X(0x00);
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
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_Y(0x12);

    // N+ Z-
    cpu->SetRegisterA(0x8A);
    cpu->SetRegisterY(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK_Y(0x8A);

    // N- Z+
    cpu->SetRegisterA(0x00);
    cpu->SetRegisterY(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK_Y(0x00);
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
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_A(0x12);

    // N+ Z-
    cpu->SetRegisterX(0x8A);
    cpu->SetRegisterA(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK_A(0x8A);

    // N- Z+
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK_A(0x00);
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
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_Y(0x12);

    // N+ Z-
    cpu->SetRegisterY(0x8A);
    cpu->SetRegisterA(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_SET;
    CHECK_ZERO_CLEAR;
    CHECK_Y(0x8A);

    // N- Z+
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0xFF);
    EXECUTE(2);
    CHECK_NEGATIVE_CLEAR;
    CHECK_ZERO_SET;
    CHECK_Y(0x00);
}
