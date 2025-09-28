/* Mos6502.Arithmetic.cpp
 *
 * Tests the CPU core's ability to perform arithmetic operations.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

//
// ADC
//
TEST("ADC imm8", "ADC-imm8") {  // 0x69
    constexpr int expectedCycles = 2;
    
    LoadData(0x200, {
        0x69, 0b00000001,  // C- Z- V- N-
        0x69, 0b00000010,  // C+ Z- V- N-
        0x69, 0b00000001,  // C+ Z+ V- N-
        0x69, 0b00000001,  // C- Z- V+ N+
        0x69, 0b00000000,  // C- Z- V- N-
        0x69, 0b00000001,  // C+ Z- V- N-
        0x69, 0b00000000,  // C+ Z+ V- N-
        0x69, 0b00000000,  // C- Z- V+ N+ 
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;
}

TEST("ADC zp", "ADC-zp") {  // 0x65
    constexpr int expectedCycles = 3;
    
    LoadData(0x200, {
        0x65, 0x80,  // C- Z- V- N-
        0x65, 0x81,  // C+ Z- V- N-
        0x65, 0x80,  // C+ Z+ V- N-
        0x65, 0x80,  // C- Z- V+ N+
        0x65, 0x82,  // C- Z- V- N-
        0x65, 0x80,  // C+ Z- V- N-
        0x65, 0x82,  // C+ Z+ V- N-
        0x65, 0x82,  // C- Z- V+ N+ 
    });

    LoadData(0x80, {
        0b00000001, 0b00000010, 0b00000000,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;
}

TEST("ADC zp, X", "ADC-zp,X") {  // 0x75
    constexpr int expectedCycles = 4;
    
    LoadData(0x200, {
        0x75, 0x80,  // X: $00, C- Z- V- N-
        0x75, 0x80,  // X: $01, C+ Z- V- N-
        0x75, 0x80,  // X: $00, C+ Z+ V- N-
        0x75, 0x80,  // X: $00, C- Z- V+ N+
        0x75, 0x80,  // X: $02, C- Z- V- N-
        0x75, 0x80,  // X: $00, C+ Z- V- N-
        0x75, 0x80,  // X: $02, C+ Z+ V- N-
        0x75, 0x80,  // X: $02, C- Z- V+ N+ 
    });

    LoadData(0x80, {
        0b00000001, 0b00000010, 0b00000000,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;
}

TEST("ADC abs16", "ADC-abs16") {  // 0x6D
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0x6D, 0x34, 0x12,  // C- Z- V- N-
        0x6D, 0x35, 0x12,  // C+ Z- V- N-
        0x6D, 0x34, 0x12,  // C+ Z+ V- N-
        0x6D, 0x34, 0x12,  // C- Z- V+ N+
        0x6D, 0x36, 0x12,  // C- Z- V- N-
        0x6D, 0x34, 0x12,  // C+ Z- V- N-
        0x6D, 0x36, 0x12,  // C+ Z+ V- N-
        0x6D, 0x36, 0x12,  // C- Z- V+ N+ 
    });

    LoadData(0x1234, {
        0b00000001,
        0b00000010,
        0b00000000,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;    
}

TEST("ADC abs16, X", "ADC-abs16,X") {  // 0x7D
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0x7D, 0x34, 0x12,  // X: $00, C- Z- V- N-
        0x7D, 0x34, 0x12,  // X: $01, C+ Z- V- N-
        0x7D, 0x34, 0x12,  // X: $00, C+ Z+ V- N-
        0x7D, 0x34, 0x12,  // X: $00, C- Z- V+ N+
        0x7D, 0x34, 0x12,  // X: $02, C- Z- V- N-
        0x7D, 0x34, 0x12,  // X: $00, C+ Z- V- N-
        0x7D, 0x34, 0x12,  // X: $02, C+ Z+ V- N-
        0x7D, 0x34, 0x12,  // X: $02, C- Z- V+ N+

        0x7D, 0xF0, 0x20,  // +1 X: $30, C- Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $31, C+ Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $30, C+ Z+ V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $30, C- Z- V+ N+
        0x7D, 0xF0, 0x20,  // +1 X: $32, C- Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $30, C+ Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $32, C+ Z+ V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $32, C- Z- V+ N+ 
    });

    LoadData(0x1234, {
        0b00000001,
        0b00000010,
        0b00000000,
    });

    LoadData(0x2120, {
        0b00000001,
        0b00000010,
        0b00000000,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;
}

TEST("ADC abs16, Y", "ADC-abs16,Y") {  // 0x79
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0x79, 0x34, 0x12,  // Y: $00, C- Z- V- N-
        0x79, 0x34, 0x12,  // Y: $01, C+ Z- V- N-
        0x79, 0x34, 0x12,  // Y: $00, C+ Z+ V- N-
        0x79, 0x34, 0x12,  // Y: $00, C- Z- V+ N+
        0x79, 0x34, 0x12,  // Y: $02, C- Z- V- N-
        0x79, 0x34, 0x12,  // Y: $00, C+ Z- V- N-
        0x79, 0x34, 0x12,  // Y: $02, C+ Z+ V- N-
        0x79, 0x34, 0x12,  // Y: $02, C- Z- V+ N+

        0x79, 0xF0, 0x20,  // +1 Y: $30, C- Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $31, C+ Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $30, C+ Z+ V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $30, C- Z- V+ N+
        0x79, 0xF0, 0x20,  // +1 Y: $32, C- Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $30, C+ Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $32, C+ Z+ V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $32, C- Z- V+ N+ 
    });

    LoadData(0x1234, {
        0b00000001,
        0b00000010,
        0b00000000,
    });

    LoadData(0x2120, {
        0b00000001,
        0b00000010,
        0b00000000,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;
}

TEST("ADC (imm8, X)", "ADC-(imm8, X)") {  // 0x61
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0x61, 0x80,  // X: $00, C- Z- V- N-
        0x61, 0x80,  // X: $02, C+ Z- V- N-
        0x61, 0x80,  // X: $00, C+ Z+ V- N-
        0x61, 0x80,  // X: $00, C- Z- V+ N+
        0x61, 0x80,  // X: $04, C- Z- V- N-
        0x61, 0x80,  // X: $00, C+ Z- V- N-
        0x61, 0x80,  // X: $04, C+ Z+ V- N-
        0x61, 0x80,  // X: $04, C- Z- V+ N+

        0x61, 0xF0,  // X: $20, C- Z- V- N-
        0x61, 0xF0,  // X: $22, C+ Z- V- N-
        0x61, 0xF0,  // X: $20, C+ Z+ V- N-
        0x61, 0xF0,  // X: $20, C- Z- V+ N+
        0x61, 0xF0,  // X: $24, C- Z- V- N-
        0x61, 0xF0,  // X: $20, C+ Z- V- N-
        0x61, 0xF0,  // X: $24, C+ Z+ V- N-
        0x61, 0xF0,  // X: $24, C- Z- V+ N+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
    });

    LoadData(0x10, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
    });

    LoadData(0x1234, {
        0b00000001,
        0b00000010,
        0b00000000,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;
}

TEST("ADC (imm8), Y", "ADC-(imm8),Y") {  // 0x71
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0x71, 0x80,  // Y: $00, C- Z- V- N-
        0x71, 0x80,  // Y: $01, C+ Z- V- N-
        0x71, 0x80,  // Y: $00, C+ Z+ V- N-
        0x71, 0x80,  // Y: $00, C- Z- V+ N+
        0x71, 0x80,  // Y: $02, C- Z- V- N-
        0x71, 0x80,  // Y: $00, C+ Z- V- N-
        0x71, 0x80,  // Y: $02, C+ Z+ V- N-
        0x71, 0x80,  // Y: $02, C- Z- V+ N+

        0x71, 0x82,  // +1 Y: $80, C- Z- V- N-
        0x71, 0x82,  // +1 Y: $81, C+ Z- V- N-
        0x71, 0x82,  // +1 Y: $80, C+ Z+ V- N-
        0x71, 0x82,  // +1 Y: $80, C- Z- V+ N+
        0x71, 0x82,  // +1 Y: $82, C- Z- V- N-
        0x71, 0x82,  // +1 Y: $80, C+ Z- V- N-
        0x71, 0x82,  // +1 Y: $82, C+ Z+ V- N-
        0x71, 0x82,  // +1 Y: $82, C- Z- V+ N+
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC0, 0x11,
    });

    LoadData(0x1240, {
        0b00000001,
        0b00000010,
        0b00000000,
    });

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b01111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;
}

//
// ADC (BCD)
//
TEST("ADC imm8-BCD-2A03", "ADC-imm-BCD-2A03") {  // 0x69, with no BCD support
    constexpr int expectedCycles = 2;

    cpu->SetVersion(Cpu::Mos6502Version::Ricoh2A03);

    LoadData(0x200, {
        0x69, 0b00000001,  // C- Z- V- N-
        0x69, 0b00000001,  // C- Z- V- N-
        0x69, 0b00000001,  // C- Z- V- N+
        0x69, 0b00000001,  // C- Z- V- N+
        0x69, 0b00000001,  // C+ Z+ V- N-
        0x69, 0b00001111,  // C- Z- V- N-
        0x69, 0b00000000,  // C- Z- V- N-
        0x69, 0b00000000,  // C- Z- V- N-
        0x69, 0b00000000,  // C- Z- V- N+
        0x69, 0b00000000,  // C- Z- V- N+
        0x69, 0b00000000,  // C+ Z+ V- N-
        0x69, 0b00001110,  // C- Z- V- N-
    });

    cpu->SetDecimalMode();

    CHECK_DECIMAL_MODE_ENABLED;  // The Ricoh 2A03 still has the BCD flag, but it's unused.
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00001111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ADC imm8-BCD-65C02", "ADC-imm8-BCD-65C02") {  // 0x69, with correct zero flag behavior
    constexpr int expectedCycles = 3;

    cpu->SetVersion(Cpu::Mos6502Version::Wdc65C02);

    LoadData(0x200, {
        0x69, 0b00000001,  // C- Z- V- N-
        0x69, 0b00000001,  // C- Z- V- N-
        0x69, 0b00000001,  // C+ Z+ V- N-
        0x69, 0b00000001,  // C- Z- V+ N+
        0x69, 0b00000001,  // C+ Z- V- N-
        0x69, 0b00001111,  // C- Z- V- N-
        0x69, 0b00000000,  // C- Z- V- N-
        0x69, 0b00000000,  // C+ Z- V- N-
        0x69, 0b00000000,  // C+ Z+ V- N-
        0x69, 0b00000000,  // C- Z- V+ N+
        0x69, 0b00000000,  // C+ Z- V- N-
        0x69, 0b00001110,  // C- Z- V- N-
    });

    cpu->SetDecimalMode();
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ADC imm8-BCD", "ADC-imm8-BCD") {  // 0x69
    constexpr int expectedCycles = 2;
    
    LoadData(0x200, {
        0x69, 0b00000001,  // C- Z- V- N-
        0x69, 0b00000001,  // C- Z- V- N-
        0x69, 0b00000001,  // C+ Z+ V- N-
        0x69, 0b00000001,  // C- Z- V+ N+
        0x69, 0b00000001,  // C+ Z- V- N-
        0x69, 0b00001111,  // C- Z- V- N-
        0x69, 0b00000000,  // C- Z- V- N-
        0x69, 0b00000000,  // C+ Z- V- N-
        0x69, 0b00000000,  // C+ Z+ V- N-
        0x69, 0b00000000,  // C- Z- V+ N+
        0x69, 0b00000000,  // C+ Z- V- N-
        0x69, 0b00001110,  // C- Z- V- N-
    });

    cpu->SetDecimalMode();
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ADC zp-BCD", "ADC-zp-BCD") {  // 0x65
    constexpr int expectedCycles = 3;
    
    LoadData(0x200, {
        0x65, 0x80,  // C- Z- V- N-
        0x65, 0x80,  // C- Z- V- N-
        0x65, 0x80,  // C+ Z+ V- N-
        0x65, 0x80,  // C- Z- V+ N+
        0x65, 0x80,  // C+ Z- V- N-
        0x65, 0x81,  // C- Z- V- N-
        0x65, 0x82,  // C- Z- V- N-
        0x65, 0x82,  // C+ Z- V- N-
        0x65, 0x82,  // C+ Z+ V- N-
        0x65, 0x82,  // C- Z- V+ N+
        0x65, 0x82,  // C+ Z- V- N-
        0x65, 0x83,  // C- Z- V- N-
    });

    LoadData(0x80, {
        0b00000001,
        0b00001111,
        0b00000000,
        0b00001110,
    });

    cpu->SetDecimalMode();
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ADC zp, X-BCD", "ADC-zp,X-BCD") {  // 0x75
    constexpr int expectedCycles = 4;
    
    LoadData(0x200, {
        0x75, 0x80,  // X: $00, C- Z- V- N-
        0x75, 0x80,  // X: $00, C- Z- V- N-
        0x75, 0x80,  // X: $00, C+ Z+ V- N-
        0x75, 0x80,  // X: $00, C- Z- V+ N+
        0x75, 0x80,  // X: $00, C+ Z- V- N-
        0x75, 0x80,  // X: $01, C- Z- V- N-
        0x75, 0x80,  // X: $02, C- Z- V- N-
        0x75, 0x80,  // X: $02, C+ Z- V- N-
        0x75, 0x80,  // X: $02, C+ Z+ V- N-
        0x75, 0x80,  // X: $02, C- Z- V+ N+
        0x75, 0x80,  // X: $02, C+ Z- V- N-
        0x75, 0x80,  // X: $03, C- Z- V- N-
    });

    LoadData(0x80, {
        0b00000001,
        0b00001111,
        0b00000000,
        0b00001110,
    });

    cpu->SetDecimalMode();
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x03);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ADC abs16-BCD", "ADC-abs16-BCD") {  // 0x6D
    constexpr int expectedCycles = 4;
    
    LoadData(0x200, {
        0x6D, 0x34, 0x12,  // C- Z- V- N-
        0x6D, 0x34, 0x12,  // C- Z- V- N-
        0x6D, 0x34, 0x12,  // C+ Z+ V- N-
        0x6D, 0x34, 0x12,  // C- Z- V+ N+
        0x6D, 0x34, 0x12,  // C+ Z- V- N-
        0x6D, 0x35, 0x12,  // C- Z- V- N-
        0x6D, 0x36, 0x12,  // C- Z- V- N-
        0x6D, 0x36, 0x12,  // C+ Z- V- N-
        0x6D, 0x36, 0x12,  // C+ Z+ V- N-
        0x6D, 0x36, 0x12,  // C- Z- V+ N+
        0x6D, 0x36, 0x12,  // C+ Z- V- N-
        0x6D, 0x37, 0x12,  // C- Z- V- N-
    });

    LoadData(0x1234, {
        0b00000001,
        0b00001111,
        0b00000000,
        0b00001110,
    });

    cpu->SetDecimalMode();
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ADC abs16, X-BCD", "ADC-abs16,X-BCD") {  // 0x7D
    constexpr int expectedCycles = 4;
    
    LoadData(0x200, {
        0x7D, 0x34, 0x12,  // X: $00, C- Z- V- N-
        0x7D, 0x34, 0x12,  // X: $00, C- Z- V- N-
        0x7D, 0x34, 0x12,  // X: $00, C+ Z+ V- N-
        0x7D, 0x34, 0x12,  // X: $00, C- Z- V+ N+
        0x7D, 0x34, 0x12,  // X: $00, C+ Z- V- N-
        0x7D, 0x34, 0x12,  // X: $01, C- Z- V- N-
        0x7D, 0x34, 0x12,  // X: $02, C- Z- V- N-
        0x7D, 0x34, 0x12,  // X: $02, C+ Z- V- N-
        0x7D, 0x34, 0x12,  // X: $02, C+ Z+ V- N-
        0x7D, 0x34, 0x12,  // X: $02, C- Z- V+ N+
        0x7D, 0x34, 0x12,  // X: $02, C+ Z- V- N-
        0x7D, 0x34, 0x12,  // X: $03, C- Z- V- N-

        0x7D, 0xF0, 0x20,  // +1 X: $30, C- Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $30, C- Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $30, C+ Z+ V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $30, C- Z- V+ N+
        0x7D, 0xF0, 0x20,  // +1 X: $30, C+ Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $31, C- Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $32, C- Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $32, C+ Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $32, C+ Z+ V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $32, C- Z- V+ N+
        0x7D, 0xF0, 0x20,  // +1 X: $32, C+ Z- V- N-
        0x7D, 0xF0, 0x20,  // +1 X: $33, C- Z- V- N-
    });

    LoadData(0x1234, {
        0b00000001,
        0b00001111,
        0b00000000,
        0b00001110,
    });

    LoadData(0x2120, {
        0b00000001,
        0b00001111,
        0b00000000,
        0b00001110,
    });

    cpu->SetDecimalMode();
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x03);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x33);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ADC abs16, Y-BCD", "ADC-abs16,Y-BCD") {  // 0x79
    constexpr int expectedCycles = 4;
    
    LoadData(0x200, {
        0x79, 0x34, 0x12,  // Y: $00, C- Z- V- N-
        0x79, 0x34, 0x12,  // Y: $00, C- Z- V- N-
        0x79, 0x34, 0x12,  // Y: $00, C+ Z+ V- N-
        0x79, 0x34, 0x12,  // Y: $00, C- Z- V+ N+
        0x79, 0x34, 0x12,  // Y: $00, C+ Z- V- N-
        0x79, 0x34, 0x12,  // Y: $01, C- Z- V- N-
        0x79, 0x34, 0x12,  // Y: $02, C- Z- V- N-
        0x79, 0x34, 0x12,  // Y: $02, C+ Z- V- N-
        0x79, 0x34, 0x12,  // Y: $02, C+ Z+ V- N-
        0x79, 0x34, 0x12,  // Y: $02, C- Z- V+ N+
        0x79, 0x34, 0x12,  // Y: $02, C+ Z- V- N-
        0x79, 0x34, 0x12,  // Y: $03, C- Z- V- N-

        0x79, 0xF0, 0x20,  // +1 Y: $30, C- Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $30, C- Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $30, C+ Z+ V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $30, C- Z- V+ N+
        0x79, 0xF0, 0x20,  // +1 Y: $30, C+ Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $31, C- Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $32, C- Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $32, C+ Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $32, C+ Z+ V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $32, C- Z- V+ N+
        0x79, 0xF0, 0x20,  // +1 Y: $32, C+ Z- V- N-
        0x79, 0xF0, 0x20,  // +1 Y: $33, C- Z- V- N-
    });

    LoadData(0x1234, {
        0b00000001,
        0b00001111,
        0b00000000,
        0b00001110,
    });

    LoadData(0x2120, {
        0b00000001,
        0b00001111,
        0b00000000,
        0b00001110,
    });

    cpu->SetDecimalMode();
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x03);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x33);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ADC (imm8, X)-BCD", "ADC-(imm8,X)-BCD") {  // 0x61
    constexpr int expectedCycles = 6;
    
    LoadData(0x200, {
        0x61, 0x80,  // X: $00, C- Z- V- N-
        0x61, 0x80,  // X: $00, C- Z- V- N-
        0x61, 0x80,  // X: $00, C+ Z+ V- N-
        0x61, 0x80,  // X: $00, C- Z- V+ N+
        0x61, 0x80,  // X: $00, C+ Z- V- N-
        0x61, 0x80,  // X: $02, C- Z- V- N-
        0x61, 0x80,  // X: $04, C- Z- V- N-
        0x61, 0x80,  // X: $04, C+ Z- V- N-
        0x61, 0x80,  // X: $04, C+ Z+ V- N-
        0x61, 0x80,  // X: $04, C- Z- V+ N+
        0x61, 0x80,  // X: $04, C+ Z- V- N-
        0x61, 0x80,  // X: $06, C- Z- V- N-

        0x61, 0xF0,  // X: $20, C- Z- V- N-
        0x61, 0xF0,  // X: $20, C- Z- V- N-
        0x61, 0xF0,  // X: $20, C+ Z+ V- N-
        0x61, 0xF0,  // X: $20, C- Z- V+ N+
        0x61, 0xF0,  // X: $20, C+ Z- V- N-
        0x61, 0xF0,  // X: $22, C- Z- V- N-
        0x61, 0xF0,  // X: $24, C- Z- V- N-
        0x61, 0xF0,  // X: $24, C+ Z- V- N-
        0x61, 0xF0,  // X: $24, C+ Z+ V- N-
        0x61, 0xF0,  // X: $24, C- Z- V+ N+
        0x61, 0xF0,  // X: $24, C+ Z- V- N-
        0x61, 0xF0,  // X: $26, C- Z- V- N-
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
        0x37, 0x12,
    });

    LoadData(0x10, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
        0x37, 0x12,
    });

    LoadData(0x1234, {
        0b00000001,
        0b00001111,
        0b00000000,
        0b00001110,
    });

    cpu->SetDecimalMode();
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x06);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x26);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

TEST("ADC (imm8), Y-BCD", "ADC-(imm8),Y-BCD") {  // 0x71
    constexpr int expectedCycles = 5;
    
    LoadData(0x200, {
        0x71, 0x80,  // Y: $00, C- Z- V- N-
        0x71, 0x80,  // Y: $00, C- Z- V- N-
        0x71, 0x80,  // Y: $00, C+ Z+ V- N-
        0x71, 0x80,  // Y: $00, C- Z- V+ N+
        0x71, 0x80,  // Y: $00, C+ Z- V- N-
        0x71, 0x80,  // Y: $01, C- Z- V- N-
        0x71, 0x80,  // Y: $02, C- Z- V- N-
        0x71, 0x80,  // Y: $02, C+ Z- V- N-
        0x71, 0x80,  // Y: $02, C+ Z+ V- N-
        0x71, 0x80,  // Y: $02, C- Z- V+ N+
        0x71, 0x80,  // Y: $02, C+ Z- V- N-
        0x71, 0x80,  // Y: $03, C- Z- V- N-

        0x71, 0x82,  // +1 Y: $80, C- Z- V- N-
        0x71, 0x82,  // +1 Y: $80, C- Z- V- N-
        0x71, 0x82,  // +1 Y: $80, C+ Z+ V- N-
        0x71, 0x82,  // +1 Y: $80, C- Z- V+ N+
        0x71, 0x82,  // +1 Y: $80, C+ Z- V- N-
        0x71, 0x82,  // +1 Y: $81, C- Z- V- N-
        0x71, 0x82,  // +1 Y: $82, C- Z- V- N-
        0x71, 0x82,  // +1 Y: $82, C+ Z- V- N-
        0x71, 0x82,  // +1 Y: $82, C+ Z+ V- N-
        0x71, 0x82,  // +1 Y: $82, C- Z- V+ N+
        0x71, 0x82,  // +1 Y: $82, C+ Z- V- N-
        0x71, 0x82,  // +1 Y: $83, C- Z- V- N-
    });

    LoadData(0x80, {
       0x40, 0x12,
       0xC0, 0x11,
    });

    LoadData(0x1240, {
        0b00000001,
        0b00001111,
        0b00000000,
        0b00001110,
    });

    cpu->SetDecimalMode();
    
    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x03);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000010);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b00001001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b10011001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;  // Test for buggy NMOS 6502 behavior.
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b01111001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01100110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x83);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00010101);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;
}

//
// SBC
//
TEST("SBC imm8", "SBC-imm8") {  // 0xE9
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0xE9, 0b00000001,  // C+ Z- V- N-
        0xE9, 0b00000001,  // C- Z- V- N+
        0xE9, 0b00000001,  // C+ Z- V+ N-
        0xE9, 0b00000001,  // C+ Z+ V- N-
        0xE9, 0b11111111,  // C+ Z+ V- N-
        0xE9, 0b00000000,  // C+ Z- V- N-
        0xE9, 0b00000000,  // C- Z- V- N+
        0xE9, 0b00000000,  // C+ Z- V+ N-
        0xE9, 0b00000000,  // C+ Z+ V- N-
        0xE9, 0b11111111,  // C- Z- V- N+
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC zp", "SBC-zp") {  // 0xE5
    constexpr int expectedCycles = 3;

    LoadData(0x200, {
        0xE5, 0x80,  // C+ Z- V- N-
        0xE5, 0x80,  // C- Z- V- N+
        0xE5, 0x80,  // C+ Z- V+ N-
        0xE5, 0x80,  // C+ Z+ V- N-
        0xE5, 0x81,  // C+ Z+ V- N-
        0xE5, 0x82,  // C+ Z- V- N-
        0xE5, 0x82,  // C- Z- V- N+
        0xE5, 0x82,  // C+ Z- V+ N-
        0xE5, 0x82,  // C+ Z+ V- N-
        0xE5, 0x81,  // C- Z- V- N+
    });

    LoadData(0x80, {
        0b00000001, 0b11111111, 0b00000000,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC zp, X", "SBC-zp,X") {  // 0xF5
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xF5, 0x80,  // X: $00, C+ Z- V- N-
        0xF5, 0x80,  // X: $00, C- Z- V- N+
        0xF5, 0x80,  // X: $00, C+ Z- V+ N-
        0xF5, 0x80,  // X: $00, C+ Z+ V- N-
        0xF5, 0x80,  // X: $01, C+ Z+ V- N-
        0xF5, 0x80,  // X: $02, C+ Z- V- N-
        0xF5, 0x80,  // X: $02, C- Z- V- N+
        0xF5, 0x80,  // X: $02, C+ Z- V+ N-
        0xF5, 0x80,  // X: $02, C+ Z+ V- N-
        0xF5, 0x80,  // X: $01, C- Z- V- N+
    });

    LoadData(0x80, {
        0b00000001, 0b11111111, 0b00000000,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC abs16", "SBC-abs16") {  // 0xED
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xED, 0x34, 0x12,  // C+ Z- V- N-
        0xED, 0x34, 0x12,  // C- Z- V- N+
        0xED, 0x34, 0x12,  // C+ Z- V+ N-
        0xED, 0x34, 0x12,  // C+ Z+ V- N-
        0xED, 0x35, 0x12,  // C+ Z+ V- N-
        0xED, 0x36, 0x12,  // C+ Z- V- N-
        0xED, 0x36, 0x12,  // C- Z- V- N+
        0xED, 0x36, 0x12,  // C+ Z- V+ N-
        0xED, 0x36, 0x12,  // C+ Z+ V- N-
        0xED, 0x35, 0x12,  // C- Z- V- N+
    });

    LoadData(0x1234, {
        0b00000001, 0b11111111, 0b00000000,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC abs16, X", "SBC-abs16,X") {  // 0xFD
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xFD, 0x34, 0x12,  // X: $00, C+ Z- V- N-
        0xFD, 0x34, 0x12,  // X: $00, C- Z- V- N+
        0xFD, 0x34, 0x12,  // X: $00, C+ Z- V+ N-
        0xFD, 0x34, 0x12,  // X: $00, C+ Z+ V- N-
        0xFD, 0x34, 0x12,  // X: $01, C+ Z+ V- N-
        0xFD, 0x34, 0x12,  // X: $02, C+ Z- V- N-
        0xFD, 0x34, 0x12,  // X: $02, C- Z- V- N+
        0xFD, 0x34, 0x12,  // X: $02, C+ Z- V+ N-
        0xFD, 0x34, 0x12,  // X: $02, C+ Z+ V- N-
        0xFD, 0x34, 0x12,  // X: $01, C- Z- V- N+

        0xFD, 0xF0, 0x20,  // +1 X: $30, C+ Z- V- N-
        0xFD, 0xF0, 0x20,  // +1 X: $30, C- Z- V- N+
        0xFD, 0xF0, 0x20,  // +1 X: $30, C+ Z- V+ N-
        0xFD, 0xF0, 0x20,  // +1 X: $30, C+ Z+ V- N-
        0xFD, 0xF0, 0x20,  // +1 X: $31, C+ Z+ V- N-
        0xFD, 0xF0, 0x20,  // +1 X: $32, C+ Z- V- N-
        0xFD, 0xF0, 0x20,  // +1 X: $32, C- Z- V- N+
        0xFD, 0xF0, 0x20,  // +1 X: $32, C+ Z- V+ N-
        0xFD, 0xF0, 0x20,  // +1 X: $32, C+ Z+ V- N-
        0xFD, 0xF0, 0x20,  // +1 X: $31, C- Z- V- N+
    });

    LoadData(0x1234, {
        0b00000001, 0b11111111, 0b00000000,
    });

    LoadData(0x2120, {
        0b00000001, 0b11111111, 0b00000000,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x32);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC abs16, Y", "SBC-abs16,Y") {  // 0xF9
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xF9, 0x34, 0x12,  // Y: $00, C+ Z- V- N-
        0xF9, 0x34, 0x12,  // Y: $00, C- Z- V- N+
        0xF9, 0x34, 0x12,  // Y: $00, C+ Z- V+ N-
        0xF9, 0x34, 0x12,  // Y: $00, C+ Z+ V- N-
        0xF9, 0x34, 0x12,  // Y: $01, C+ Z+ V- N-
        0xF9, 0x34, 0x12,  // Y: $02, C+ Z- V- N-
        0xF9, 0x34, 0x12,  // Y: $02, C- Z- V- N+
        0xF9, 0x34, 0x12,  // Y: $02, C+ Z- V+ N-
        0xF9, 0x34, 0x12,  // Y: $02, C+ Z+ V- N-
        0xF9, 0x34, 0x12,  // Y: $01, C- Z- V- N+

        0xF9, 0xF0, 0x20,  // +1 Y: $30, C+ Z- V- N-
        0xF9, 0xF0, 0x20,  // +1 Y: $30, C- Z- V- N+
        0xF9, 0xF0, 0x20,  // +1 Y: $30, C+ Z- V+ N-
        0xF9, 0xF0, 0x20,  // +1 Y: $30, C+ Z+ V- N-
        0xF9, 0xF0, 0x20,  // +1 Y: $31, C+ Z+ V- N-
        0xF9, 0xF0, 0x20,  // +1 Y: $32, C+ Z- V- N-
        0xF9, 0xF0, 0x20,  // +1 Y: $32, C- Z- V- N+
        0xF9, 0xF0, 0x20,  // +1 Y: $32, C+ Z- V+ N-
        0xF9, 0xF0, 0x20,  // +1 Y: $32, C+ Z+ V- N-
        0xF9, 0xF0, 0x20,  // +1 Y: $31, C- Z- V- N+
    });

    LoadData(0x1234, {
        0b00000001, 0b11111111, 0b00000000,
    });

    LoadData(0x2120, {
        0b00000001, 0b11111111, 0b00000000,
    });

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x32);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC (imm8, X)", "SBC-(imm8,X)") {  // 0xE1
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xE1, 0x80,  // X: $00, C+ Z- V- N-
        0xE1, 0x80,  // X: $00, C- Z- V- N+
        0xE1, 0x80,  // X: $00, C+ Z- V+ N-
        0xE1, 0x80,  // X: $00, C+ Z+ V- N-
        0xE1, 0x80,  // X: $02, C+ Z+ V- N-
        0xE1, 0x80,  // X: $04, C+ Z- V- N-
        0xE1, 0x80,  // X: $04, C- Z- V- N+
        0xE1, 0x80,  // X: $04, C+ Z- V+ N-
        0xE1, 0x80,  // X: $04, C+ Z+ V- N-
        0xE1, 0x80,  // X: $02, C- Z- V- N+

        0xE1, 0xF0,  // X: $20, C+ Z- V- N-
        0xE1, 0xF0,  // X: $20, C- Z- V- N+
        0xE1, 0xF0,  // X: $20, C+ Z- V+ N-
        0xE1, 0xF0,  // X: $20, C+ Z+ V- N-
        0xE1, 0xF0,  // X: $22, C+ Z+ V- N-
        0xE1, 0xF0,  // X: $24, C+ Z- V- N-
        0xE1, 0xF0,  // X: $24, C- Z- V- N+
        0xE1, 0xF0,  // X: $24, C+ Z- V+ N-
        0xE1, 0xF0,  // X: $24, C+ Z+ V- N-
        0xE1, 0xF0,  // X: $22, C- Z- V- N+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
    });

    LoadData(0x10, {
        0x34, 0x12,
        0x35, 0x12,
        0x36, 0x12,
    });

    LoadData(0x1234, {
        0b00000001, 0b11111111, 0b00000000,
    });
    
    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x04);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x24);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC (imm8), Y", "SBC-(imm8),Y") {  // 0xF1
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0xF1, 0x80,  // Y: $00, C+ Z- V- N-
        0xF1, 0x80,  // Y: $00, C- Z- V- N+
        0xF1, 0x80,  // Y: $00, C+ Z- V+ N-
        0xF1, 0x80,  // Y: $00, C+ Z+ V- N-
        0xF1, 0x80,  // Y: $01, C+ Z+ V- N-
        0xF1, 0x80,  // Y: $02, C+ Z- V- N-
        0xF1, 0x80,  // Y: $02, C- Z- V- N+
        0xF1, 0x80,  // Y: $02, C+ Z- V+ N-
        0xF1, 0x80,  // Y: $02, C+ Z+ V- N-
        0xF1, 0x80,  // Y: $01, C- Z- V- N+

        0xF1, 0x82,  // Y: $20, C+ Z- V- N-
        0xF1, 0x82,  // Y: $20, C- Z- V- N+
        0xF1, 0x82,  // Y: $20, C+ Z- V+ N-
        0xF1, 0x82,  // Y: $20, C+ Z+ V- N-
        0xF1, 0x82,  // Y: $21, C+ Z+ V- N-
        0xF1, 0x82,  // Y: $22, C+ Z- V- N-
        0xF1, 0x82,  // Y: $22, C- Z- V- N+
        0xF1, 0x82,  // Y: $22, C+ Z- V+ N-
        0xF1, 0x82,  // Y: $22, C+ Z+ V- N-
        0xF1, 0x82,  // Y: $21, C- Z- V- N+
    });

    LoadData(0x80, {
        0x40, 0x12,
        0xC0, 0x11,
    });
    
    LoadData(0x1240, {
        0b00000001, 0b11111111, 0b00000000,
    });
    
    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x02);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b00000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_SET;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

//
// SBC (BCD)
//
TEST("SBC imm8-BCD-2A03", "SBC-imm8-BCD-2A03") {  // 0xE9, with no BCD support
    constexpr int expectedCycles = 2;

    cpu->SetVersion(Cpu::Mos6502Version::Ricoh2A03);

    LoadData(0x200, {
        0xE9, 0b00000001,  // C+ Z- V- N-
        0xE9, 0b00000001,  // C+ Z- V+ N-
        0xE9, 0b00000001,  // C+ Z- V- N+
        0xE9, 0b00000001,  // C- Z- V- N+
        0xE9, 0b00000001,  // C+ Z- V- N+
        0xE9, 0b00000000,  // C+ Z- V- N-
        0xE9, 0b00000000,  // C+ Z- V+ N-
        0xE9, 0b00000000,  // C+ Z- V- N+
        0xE9, 0b00000000,  // C- Z- V- N+
        0xE9, 0b00000000,  // C+ Z- V- N+
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111111);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC imm8-BCD-65C02", "SBC-imm8-BCD-65C02") {  // 0xE9, with the 65C02
    constexpr int expectedCycles = 3;

    cpu->SetVersion(Cpu::Mos6502Version::Wdc65C02);

    LoadData(0x200, {
        0xE9, 0b00000001,  // C+ Z- V- N-
        0xE9, 0b00000001,  // C+ Z- V+ N-
        0xE9, 0b00000001,  // C+ Z- V- N+
        0xE9, 0b00000001,  // C- Z- V- N+
        0xE9, 0b00000001,  // C+ Z- V- N+
        0xE9, 0b00000000,  // C+ Z- V- N-
        0xE9, 0b00000000,  // C+ Z- V+ N-
        0xE9, 0b00000000,  // C+ Z- V- N+
        0xE9, 0b00000000,  // C- Z- V- N+
        0xE9, 0b00000000,  // C+ Z- V- N+
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC imm8-BCD", "SBC-imm8-BCD") {  // 0xE9
    constexpr int expectedCycles = 2;

    LoadData(0x200, {
        0xE9, 0b00000001,  // C+ Z- V- N-
        0xE9, 0b00000001,  // C+ Z- V+ N-
        0xE9, 0b00000001,  // C+ Z- V- N+
        0xE9, 0b00000001,  // C- Z- V- N+
        0xE9, 0b00000001,  // C+ Z- V- N+
        0xE9, 0b00000000,  // C+ Z- V- N-
        0xE9, 0b00000000,  // C+ Z- V+ N-
        0xE9, 0b00000000,  // C+ Z- V- N+
        0xE9, 0b00000000,  // C- Z- V- N+
        0xE9, 0b00000000,  // C+ Z- V- N+
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC zp-BCD", "SBC-zp-BCD") {  // 0xE5
    constexpr int expectedCycles = 3;

    LoadData(0x200, {
        0xE5, 0x80,  // C+ Z- V- N-
        0xE5, 0x80,  // C+ Z- V+ N-
        0xE5, 0x80,  // C+ Z- V- N+
        0xE5, 0x80,  // C- Z- V- N+
        0xE5, 0x80,  // C+ Z- V- N+
        0xE5, 0x81,  // C+ Z- V- N-
        0xE5, 0x81,  // C+ Z- V+ N-
        0xE5, 0x81,  // C+ Z- V- N+
        0xE5, 0x81,  // C- Z- V- N+
        0xE5, 0x81,  // C+ Z- V- N+
    });

    LoadData(0x80, {
        0b00000001, 0b00000000,
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC zp, X-BCD", "SBC-zp,X-BCD") {  // 0xF5
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xF5, 0x80,  // X: $00, C+ Z- V- N-
        0xF5, 0x80,  // X: $00, C+ Z- V+ N-
        0xF5, 0x80,  // X: $00, C+ Z- V- N+
        0xF5, 0x80,  // X: $00, C- Z- V- N+
        0xF5, 0x80,  // X: $00, C+ Z- V- N+
        0xF5, 0x80,  // X: $01, C+ Z- V- N-
        0xF5, 0x80,  // X: $01, C+ Z- V+ N-
        0xF5, 0x80,  // X: $01, C+ Z- V- N+
        0xF5, 0x80,  // X: $01, C- Z- V- N+
        0xF5, 0x80,  // X: $01, C+ Z- V- N+
    });

    LoadData(0x80, {
        0b00000001, 0b00000000,
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC abs16-BCD", "SBC-abs16-BCD") {  // 0xED
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xED, 0x34, 0x12,  // C+ Z- V- N-
        0xED, 0x34, 0x12,  // C+ Z- V+ N-
        0xED, 0x34, 0x12,  // C+ Z- V- N+
        0xED, 0x34, 0x12,  // C- Z- V- N+
        0xED, 0x34, 0x12,  // C+ Z- V- N+
        0xED, 0x35, 0x12,  // C+ Z- V- N-
        0xED, 0x35, 0x12,  // C+ Z- V+ N-
        0xED, 0x35, 0x12,  // C+ Z- V- N+
        0xED, 0x35, 0x12,  // C- Z- V- N+
        0xED, 0x35, 0x12,  // C+ Z- V- N+
    });

    LoadData(0x1234, {
        0b00000001, 0b00000000,
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC abs16, X-BCD", "SBC-abs16,X-BCD") {  // 0xFD
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xFD, 0x34, 0x12,  // X: $00, C+ Z- V- N-
        0xFD, 0x34, 0x12,  // X: $00, C+ Z- V+ N-
        0xFD, 0x34, 0x12,  // X: $00, C+ Z- V- N+
        0xFD, 0x34, 0x12,  // X: $00, C- Z- V- N+
        0xFD, 0x34, 0x12,  // X: $00, C+ Z- V- N+
        0xFD, 0x35, 0x12,  // X: $01, C+ Z- V- N-
        0xFD, 0x35, 0x12,  // X: $01, C+ Z- V+ N-
        0xFD, 0x35, 0x12,  // X: $01, C+ Z- V- N+
        0xFD, 0x35, 0x12,  // X: $01, C- Z- V- N+
        0xFD, 0x35, 0x12,  // X: $01, C+ Z- V- N+

        0xFD, 0xF0, 0x20,  // +1 X: $30, C+ Z- V- N-
        0xFD, 0xF0, 0x20,  // +1 X: $30, C+ Z- V+ N-
        0xFD, 0xF0, 0x20,  // +1 X: $30, C+ Z- V- N+
        0xFD, 0xF0, 0x20,  // +1 X: $30, C- Z- V- N+
        0xFD, 0xF0, 0x20,  // +1 X: $30, C+ Z- V- N+
        0xFD, 0xF0, 0x20,  // +1 X: $31, C+ Z- V- N-
        0xFD, 0xF0, 0x20,  // +1 X: $31, C+ Z- V+ N-
        0xFD, 0xF0, 0x20,  // +1 X: $31, C+ Z- V- N+
        0xFD, 0xF0, 0x20,  // +1 X: $31, C- Z- V- N+
        0xFD, 0xF0, 0x20,  // +1 X: $31, C+ Z- V- N+
    });

    LoadData(0x1234, {
        0b00000001, 0b00000000,
    });

    LoadData(0x2120, {
        0b00000001, 0b00000000,
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x30);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x31);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC abs16, Y-BCD", "SBC-abs16,Y-BCD") {  // 0xF9
    constexpr int expectedCycles = 4;

    LoadData(0x200, {
        0xF9, 0x34, 0x12,  // Y: $00, C+ Z- V- N-
        0xF9, 0x34, 0x12,  // Y: $00, C+ Z- V+ N-
        0xF9, 0x34, 0x12,  // Y: $00, C+ Z- V- N+
        0xF9, 0x34, 0x12,  // Y: $00, C- Z- V- N+
        0xF9, 0x34, 0x12,  // Y: $00, C+ Z- V- N+
        0xF9, 0x35, 0x12,  // Y: $01, C+ Z- V- N-
        0xF9, 0x35, 0x12,  // Y: $01, C+ Z- V+ N-
        0xF9, 0x35, 0x12,  // Y: $01, C+ Z- V- N+
        0xF9, 0x35, 0x12,  // Y: $01, C- Z- V- N+
        0xF9, 0x35, 0x12,  // Y: $01, C+ Z- V- N+

        0xF9, 0xF0, 0x20,  // +1 Y: $30, C+ Z- V- N-
        0xF9, 0xF0, 0x20,  // +1 Y: $30, C+ Z- V+ N-
        0xF9, 0xF0, 0x20,  // +1 Y: $30, C+ Z- V- N+
        0xF9, 0xF0, 0x20,  // +1 Y: $30, C- Z- V- N+
        0xF9, 0xF0, 0x20,  // +1 Y: $30, C+ Z- V- N+
        0xF9, 0xF0, 0x20,  // +1 Y: $31, C+ Z- V- N-
        0xF9, 0xF0, 0x20,  // +1 Y: $31, C+ Z- V+ N-
        0xF9, 0xF0, 0x20,  // +1 Y: $31, C+ Z- V- N+
        0xF9, 0xF0, 0x20,  // +1 Y: $31, C- Z- V- N+
        0xF9, 0xF0, 0x20,  // +1 Y: $31, C+ Z- V- N+
    });

    LoadData(0x1234, {
        0b00000001, 0b00000000,
    });

    LoadData(0x2120, {
        0b00000001, 0b00000000,
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x30);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x31);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC (imm8, X)-BCD", "SBC-(imm8,X)-BCD") {  // 0xE1
    constexpr int expectedCycles = 6;

    LoadData(0x200, {
        0xE1, 0x80,  // X: $00, C+ Z- V- N-
        0xE1, 0x80,  // X: $00, C+ Z- V+ N-
        0xE1, 0x80,  // X: $00, C+ Z- V- N+
        0xE1, 0x80,  // X: $00, C- Z- V- N+
        0xE1, 0x80,  // X: $00, C+ Z- V- N+
        0xE1, 0x80,  // X: $02, C+ Z- V- N-
        0xE1, 0x80,  // X: $02, C+ Z- V+ N-
        0xE1, 0x80,  // X: $02, C+ Z- V- N+
        0xE1, 0x80,  // X: $02, C- Z- V- N+
        0xE1, 0x80,  // X: $02, C+ Z- V- N+

        0xE1, 0xF0,  // X: $20, C+ Z- V- N-
        0xE1, 0xF0,  // X: $20, C+ Z- V+ N-
        0xE1, 0xF0,  // X: $20, C+ Z- V- N+
        0xE1, 0xF0,  // X: $20, C- Z- V- N+
        0xE1, 0xF0,  // X: $20, C+ Z- V- N+
        0xE1, 0xF0,  // X: $22, C+ Z- V- N-
        0xE1, 0xF0,  // X: $22, C+ Z- V+ N-
        0xE1, 0xF0,  // X: $22, C+ Z- V- N+
        0xE1, 0xF0,  // X: $22, C- Z- V- N+
        0xE1, 0xF0,  // X: $22, C+ Z- V- N+
    });

    LoadData(0x80, {
        0x34, 0x12,
        0x35, 0x12,
    });

    LoadData(0x10, {
        0x34, 0x12,
        0x35, 0x12,
    });
    
    LoadData(0x1234, {
        0b00000001, 0b00000000,
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x02);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterX(0x20);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterX(0x22);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}

TEST("SBC (imm8), Y-BCD", "SBC-(imm8),Y-BCD") {  // 0xF1
    constexpr int expectedCycles = 5;

    LoadData(0x200, {
        0xF1, 0x80,  // Y: $00, C+ Z- V- N-
        0xF1, 0x80,  // Y: $00, C+ Z- V+ N-
        0xF1, 0x80,  // Y: $00, C+ Z- V- N+
        0xF1, 0x80,  // Y: $00, C- Z- V- N+
        0xF1, 0x80,  // Y: $00, C+ Z- V- N+
        0xF1, 0x80,  // Y: $01, C+ Z- V- N-
        0xF1, 0x80,  // Y: $01, C+ Z- V+ N-
        0xF1, 0x80,  // Y: $01, C+ Z- V- N+
        0xF1, 0x80,  // Y: $01, C- Z- V- N+
        0xF1, 0x80,  // Y: $01, C+ Z- V- N+

        0xF1, 0x82,  // +1 Y: $80, C+ Z- V- N-
        0xF1, 0x82,  // +1 Y: $80, C+ Z- V+ N-
        0xF1, 0x82,  // +1 Y: $80, C+ Z- V- N+
        0xF1, 0x82,  // +1 Y: $80, C- Z- V- N+
        0xF1, 0x82,  // +1 Y: $80, C+ Z- V- N+
        0xF1, 0x82,  // +1 Y: $81, C+ Z- V- N-
        0xF1, 0x82,  // +1 Y: $81, C+ Z- V+ N-
        0xF1, 0x82,  // +1 Y: $81, C+ Z- V- N+
        0xF1, 0x82,  // +1 Y: $81, C- Z- V- N+
        0xF1, 0x82,  // +1 Y: $81, C+ Z- V- N+
    });

    LoadData(0x80, {
       0x40, 0x12,
       0xC0, 0x11,
    });
    
    LoadData(0x1240, {
        0b00000001, 0b00000000,
    });

    cpu->SetDecimalMode();

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x00);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x01);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->SetCarryFlag();
    cpu->SetRegisterY(0x80);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x82);
    cpu->SetRegisterA(0b00001000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b00000111);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b10000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b01111001);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_SET;
    CHECK_NEGATIVE_CLEAR;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b10000001);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10000000);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b00000000);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b10011001);
    CHECK_CARRY_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;

    cpu->ClearCarryFlag();
    cpu->SetRegisterY(0x81);
    cpu->SetRegisterA(0b11111111);
    EXECUTE(expectedCycles + 1);
    CHECK_A(0b11111110);
    CHECK_CARRY_SET;
    CHECK_ZERO_CLEAR;
    CHECK_OVERFLOW_CLEAR;
    CHECK_NEGATIVE_SET;
}
