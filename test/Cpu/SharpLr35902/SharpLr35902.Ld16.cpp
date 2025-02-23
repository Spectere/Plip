/* SharpLr35902.Ld16.cpp
 *
 * Tests the CPU core's ability to load 16-bit values.
 */

#include "catch2/catch_test_macros.hpp"

#include "SharpLr35902Common.h"

TEST("LD BC, imm16", "LD-BC,imm16") {  // 0x01
    LoadData(0x00, { 0x01, 0x34, 0x12 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetBc() == 0x1234);
    CHECK(cycles == 3);
}

TEST("LD <imm16>, SP", "LD-imm16,SP") {  // 0x08
    cpu->SetSp(0x1234);
    LoadData(0x00, { 0x08, 0x89, 0x67 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x6789) == 0x34);
    CHECK(memory->GetByte(0x678A) == 0x12);
    CHECK(cycles == 5);
}

TEST("LD DE, imm16", "LD-DE,imm16") {  // 0x11
    LoadData(0x00, { 0x11, 0x34, 0x12 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetDe() == 0x1234);
    CHECK(cycles == 3);
}

TEST("LD HL, imm16", "LD-HL,imm16") {  // 0x21
    LoadData(0x00, { 0x21, 0x34, 0x12 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1234);
    CHECK(cycles == 3);
}

TEST("LD SP, imm16", "LD-SP,imm16") {  // 0x31
    LoadData(0x00, { 0x31, 0x34, 0x12 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x1234);
    CHECK(cycles == 3);
}

TEST("POP BC", "POP-BC") {  // 0xC1
    memory->SetByte(0x0FFF, 0x12);
    memory->SetByte(0x0FFE, 0x34);
    cpu->SetSp(0x0FFE);
    LoadData(0x00, { 0xC1 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x12);
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cpu->GetSp() == 0x1000);
    CHECK(cycles == 3);
}

TEST("PUSH BC", "PUSH-BC") {  // 0xC5
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x12);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x34);
    cpu->SetSp(0x1000);
    LoadData(0x00, { 0xC5 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x0FFF) == 0x12);
    CHECK(memory->GetByte(0x0FFE) == 0x34);
    CHECK(cpu->GetSp() == 0x0FFE);
    CHECK(cycles == 3);
}

TEST("POP DE", "POP-DE") {  // 0xD1
    memory->SetByte(0x0FFF, 0x12);
    memory->SetByte(0x0FFE, 0x34);
    cpu->SetSp(0x0FFE);
    LoadData(0x00, { 0xD1 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x12);
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cpu->GetSp() == 0x1000);
    CHECK(cycles == 3);
}

TEST("PUSH DE", "PUSH-DE") {  // 0xD5
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x12);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x34);
    cpu->SetSp(0x1000);
    LoadData(0x00, { 0xD5 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x0FFF) == 0x12);
    CHECK(memory->GetByte(0x0FFE) == 0x34);
    CHECK(cpu->GetSp() == 0x0FFE);
    CHECK(cycles == 3);
}

TEST("POP HL", "POP-HL") {  // 0xE1
    memory->SetByte(0x0FFF, 0x12);
    memory->SetByte(0x0FFE, 0x34);
    cpu->SetSp(0x0FFE);
    LoadData(0x00, { 0xE1 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x12);
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cpu->GetSp() == 0x1000);
    CHECK(cycles == 3);
}

TEST("PUSH HL", "PUSH-HL") {  // 0xE5
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x12);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x34);
    cpu->SetSp(0x1000);
    LoadData(0x00, { 0xE5 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x0FFF) == 0x12);
    CHECK(memory->GetByte(0x0FFE) == 0x34);
    CHECK(cpu->GetSp() == 0x0FFE);
    CHECK(cycles == 3);
}

TEST("POP AF", "POP-AF") {  // 0xF1
    memory->SetByte(0x0FFF, 0x12);
    memory->SetByte(0x0FFE, 0xF0);
    cpu->SetSp(0x0FFE);
    LoadData(0x00, { 0xF1 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x12);
    CHECK(cpu->GetCarryFlag());
    CHECK(cpu->GetHalfCarryFlag());
    CHECK(cpu->GetSubtractFlag());
    CHECK(cpu->GetZeroFlag());
    CHECK(cpu->GetSp() == 0x1000);
    CHECK(cycles == 3);
}

TEST("PUSH AF", "PUSH-AF") {  // 0xF5
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x12);
    cpu->SetCarryFlag();
    cpu->SetHalfCarryFlag();
    cpu->SetSubtractFlag();
    cpu->SetZeroFlag();
    cpu->SetSp(0x1000);
    LoadData(0x00, { 0xF5 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x0FFF) == 0x12);
    CHECK(memory->GetByte(0x0FFE) == 0xF0);
    CHECK(cpu->GetSp() == 0x0FFE);
    CHECK(cycles == 3);
}

TEST("LD HL, SP + imm8s (zero)", "LD-SP,imm8s (zero)") {  // 0xF8
    cpu->SetSp(0x150);
    LoadData(0x00, { 0xF8, 0x00 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x150);
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 3);
}

TEST("LD HL, SP + imm8s (pos, NH, NC)", "LD-SP,imm8s (pos, NH, NC)") {  // 0xF8
    cpu->SetSp(0x150);
    LoadData(0x00, { 0xF8, 0x01 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x151);
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 3);
}

TEST("LD HL, SP + imm8s (pos, H, NC)", "LD-SP,imm8s (pos, H, NC)") {  // 0xF8
    cpu->SetSp(0x15F);
    LoadData(0x00, { 0xF8, 0x01 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x160);
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 3);
}

TEST("LD HL, SP + imm8s (pos, NH, C)", "LD-SP,imm8s (pos, H, C)") {  // 0xF8
    cpu->SetSp(0x1FF);
    LoadData(0x00, { 0xF8, 0x10 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x20F);
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 3);
}

TEST("LD HL, SP + imm8s (pos, H, C)", "LD-SP,imm8s (pos, H, C)") {  // 0xF8
    cpu->SetSp(0x1FF);
    LoadData(0x00, { 0xF8, 0x01 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x200);
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_SET;
    CHECK(cycles == 3);
}

TEST("LD HL, SP + imm8s (neg, H, C)", "LD-SP,imm8s (neg, H, C)") {  // 0xF8
    cpu->SetSp(0x151);
    LoadData(0x00, { 0xF8, 0xFF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x150);
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_SET;
    CHECK(cycles == 3);
}

TEST("LD HL, SP + imm8s (neg, NH, C)", "LD-SP,imm8s (neg, NH, C)") {  // 0xF8
    cpu->SetSp(0x150);
    LoadData(0x00, { 0xF8, 0xFF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x14F);
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 3);
}

TEST("LD HL, SP + imm8s (neg, H, NC)", "LD-SP,imm8s (neg, H, NC)") {  // 0xF8
    cpu->SetSp(0x208);
    LoadData(0x00, { 0xF8, 0xE8 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1F0);
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 3);
}

TEST("LD HL, SP + imm8s (neg, NH, NC)", "LD-SP,imm8s (neg, NH, NC)") {  // 0xF8
    cpu->SetSp(0x200);
    LoadData(0x00, { 0xF8, 0xFF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1FF);
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 3);
}

TEST("LD SP, HL", "LD-SP,HL") {  // 0xF9
    cpu->SetHl(0x1234);
    LoadData(0x00, { 0xF9 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x1234);
    CHECK(cycles == 2);
}
