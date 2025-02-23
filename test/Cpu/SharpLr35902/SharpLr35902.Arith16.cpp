/* SharpLr35902.Arith16.cpp
*
 * Tests the CPU core's ability to perform 16-bit arithmetic and logical operations.
 */

#include "catch2/catch_test_macros.hpp"

#include "SharpLr35902Common.h"

TEST("INC BC", "INC-BC") {  // 0x03
    cpu->SetBc(0xFF);
    LoadData(0x00, { 0x03 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetBc() == 0x0100);
    CHECK(cycles == 2);
}

TEST("INC DE", "INC-DE") {  // 0x13
    cpu->SetDe(0xFF);
    LoadData(0x00, { 0x13 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetDe() == 0x0100);
    CHECK(cycles == 2);
}

TEST("INC HL", "INC-HL") {  // 0x23
    cpu->SetHl(0xFF);
    LoadData(0x00, { 0x23 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x0100);
    CHECK(cycles == 2);
}

TEST("INC SP", "INC-SP") {  // 0x33
    cpu->SetSp(0xFF);
    LoadData(0x00, { 0x33 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x0100);
    CHECK(cycles == 2);
}

TEST("DEC BC", "DEC-BC") {  // 0x0B
    cpu->SetBc(0x100);
    LoadData(0x00, { 0x0B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetBc() == 0x00FF);
    CHECK(cycles == 2);
}

TEST("DEC DE", "DEC-DE") {  // 0x1B
    cpu->SetDe(0x100);
    LoadData(0x00, { 0x1B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetDe() == 0x00FF);
    CHECK(cycles == 2);
}

TEST("DEC HL", "DEC-HL") {  // 0x2B
    cpu->SetHl(0x100);
    LoadData(0x00, { 0x2B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x00FF);
    CHECK(cycles == 2);
}

TEST("DEC SP", "DEC-SP") {  // 0x3B
    cpu->SetSp(0x100);
    LoadData(0x00, { 0x3B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x00FF);
    CHECK(cycles == 2);
}

TEST("ADD HL, BC (NH, NC)", "ADD-HL,BC-NH,NC") {  // 0x09
    cpu->SetHl(0x1234);
    cpu->SetBc(0x4321);
    LoadData(0x00, { 0x09 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x5555);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD HL, BC (H, NC)", "ADD-HL,BC-H,NC") {  // 0x09
    cpu->SetHl(0x0A00);
    cpu->SetBc(0x0700);
    LoadData(0x00, { 0x09 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1100);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD HL, BC (NH, C)", "ADD-HL,BC-NH,C") {  // 0x09
    cpu->SetHl(0xF000);
    cpu->SetBc(0x2000);
    LoadData(0x00, { 0x09 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1000);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("ADD HL, BC (H, C)", "ADD-HL,BC-H,C") {  // 0x09
    cpu->SetHl(0xFFFF);
    cpu->SetBc(0x0001);
    LoadData(0x00, { 0x09 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x0000);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("ADD HL, DE (NH, NC)", "ADD-HL,DE-NH,NC") {  // 0x19
    cpu->SetHl(0x1234);
    cpu->SetDe(0x4321);
    LoadData(0x00, { 0x19 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x5555);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD HL, DE (H, NC)", "ADD-HL,DE-H,NC") {  // 0x19
    cpu->SetHl(0x0A00);
    cpu->SetDe(0x0700);
    LoadData(0x00, { 0x19 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1100);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD HL, DE (NH, C)", "ADD-HL,DE-NH,C") {  // 0x19
    cpu->SetHl(0xF000);
    cpu->SetDe(0x2000);
    LoadData(0x00, { 0x19 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1000);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("ADD HL, DE (H, C)", "ADD-HL,DE-H,C") {  // 0x19
    cpu->SetHl(0xFFFF);
    cpu->SetDe(0x0001);
    LoadData(0x00, { 0x19 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x0000);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("ADD HL, HL (NH, NC)", "ADD-HL,HL-NH,NC") {  // 0x29
    cpu->SetHl(0x1234);
    LoadData(0x00, { 0x29 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x2468);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD HL, HL (H, NC)", "ADD-HL,HL-H,NC") {  // 0x29
    cpu->SetHl(0x0900);
    LoadData(0x00, { 0x29 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1200);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD HL, HL (NH, C)", "ADD-HL,HL-NH,C") {  // 0x29
    cpu->SetHl(0x9000);
    LoadData(0x00, { 0x29 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x2000);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("ADD HL, HL (H, C)", "ADD-HL,HL-H,C") {  // 0x29
    cpu->SetHl(0x8888);
    LoadData(0x00, { 0x29 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1110);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("ADD HL, SP (NH, NC)", "ADD-HL,SP-NH,NC") {  // 0x39
    cpu->SetHl(0x1234);
    cpu->SetSp(0x4321);
    LoadData(0x00, { 0x39 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x5555);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD HL, SP (H, NC)", "ADD-HL,SP-H,NC") {  // 0x39
    cpu->SetHl(0x0A00);
    cpu->SetSp(0x0700);
    LoadData(0x00, { 0x39 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1100);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD HL, SP (NH, C)", "ADD-HL,SP-NH,C") {  // 0x39
    cpu->SetHl(0xF000);
    cpu->SetSp(0x2000);
    LoadData(0x00, { 0x39 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x1000);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("ADD HL, SP (H, C)", "ADD-HL,SP-H,C") {  // 0x39
    cpu->SetHl(0xFFFF);
    cpu->SetSp(0x0001);
    LoadData(0x00, { 0x39 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetHl() == 0x0000);
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("ADD SP, imm8s+ (NH, NC)", "ADD-SP,imm8sP-NH-NC") {  // 0xE8
    cpu->SetSp(0x100);
    LoadData(0x00, { 0xE8, 0x23 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x0123);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("ADD SP, imm8s+ (H, NC)", "ADD-SP,imm8sP-H-NC") {  // 0xE8
    cpu->SetSp(0x10F);
    LoadData(0x00, { 0xE8, 0x23 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x0132);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("ADD SP, imm8s+ (NH, C)", "ADD-SP,imm8sP-NH-C") {  // 0xE8
    cpu->SetSp(0x1F0);
    LoadData(0x00, { 0xE8, 0x23 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x0213);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("ADD SP, imm8s+ (H, C)", "ADD-SP,imm8sP-H-C") {  // 0xE8
    cpu->SetSp(0x1FF);
    LoadData(0x00, { 0xE8, 0x23 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x0222);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("ADD SP, imm8s- (H, C)", "ADD-SP,imm8sN-H-C") {  // 0xE8
    cpu->SetSp(0x123);
    LoadData(0x00, { 0xE8, 0b11011101 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x0100);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("ADD SP, imm8s- (NH, C)", "ADD-SP,imm8sN-NH-C") {  // 0xE8
    cpu->SetSp(0x131);
    LoadData(0x00, { 0xE8, 0b11011101 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x010E);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("ADD SP, imm8s- (H, NC)", "ADD-SP,imm8sN-H-NC") {  // 0xE8
    cpu->SetSp(0x213);
    LoadData(0x00, { 0xE8, 0b11011101 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x01F0);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("ADD SP, imm8s- (NH, NC)", "ADD-SP,imm8sN-NH-NC") {  // 0xE8
    cpu->SetSp(0x212);
    LoadData(0x00, { 0xE8, 0b11011101 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetSp() == 0x01EF);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}
