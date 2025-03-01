/* SharpLr35902.Ld8.cpp
 *
 * Tests the CPU core's ability to load 8-bit values.
 */

#include "catch2/catch_test_macros.hpp"

#include "SharpLr35902Common.h"

TEST("LD <BC>, A", "LD-BC,A") { // 0x02
    cpu->SetBc(0x200);
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x02 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD B, imm8", "LD-B,imm8") { // 0x06
    LoadData(0x00, { 0x06, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD A, <BC>", "LD-A,BC") { // 0x0A
    cpu->SetBc(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x0A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD C, imm8", "LD-C,imm8") { // 0x0E
    LoadData(0x00, { 0x0E, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD <DE>, A", "LD-DE,A") { // 0x12
    cpu->SetDe(0x200);
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x12 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD D, imm8", "LD-D,imm8") { // 0x16
    LoadData(0x00, { 0x16, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD A, <DE>", "LD-A,DE") { // 0x1A
    cpu->SetDe(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x1A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD E, imm8", "LD-E,imm8") { // 0x1E
    LoadData(0x00, { 0x1E, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD <HLI>, A", "LD-HLI,A") { // 0x22
    cpu->SetHl(0x200);
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x22 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cpu->GetHl() == 0x201);
    CHECK(cycles == 2);
}

TEST("LD H, imm8", "LD-H,imm8") { // 0x26
    LoadData(0x00, { 0x26, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD A, <HLI>", "LD-A,HLI") { // 0x2A
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x2A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cpu->GetHl() == 0x201);
    CHECK(cycles == 2);
}

TEST("LD L, imm8", "LD-L,imm8") { // 0x2E
    LoadData(0x00, { 0x2E, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD <HLD>, A", "LD-HLD,A") { // 0x32
    cpu->SetHl(0x200);
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x32 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cpu->GetHl() == 0x1FF);
    CHECK(cycles == 2);
}

TEST("LD <HL>, imm8", "LD-HL,imm8") { // 0x36
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x36, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cycles == 3);
}

TEST("LD A, <HLD>", "LD-A,HLD") { // 0x3A
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x3A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cpu->GetHl() == 0x1FF);
    CHECK(cycles == 2);
}

TEST("LD A, imm8", "LD-A,imm8") { // 0x3E
    LoadData(0x00, { 0x3E, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD B, B", "LD-B,B") {  // 0x40
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x34);
    LoadData(0x00, { 0x40 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD B, C", "LD-B,C") {  // 0x41
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x34);
    LoadData(0x00, { 0x41 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD B, D", "LD-B,D") {  // 0x42
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x34);
    LoadData(0x00, { 0x42 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD B, E", "LD-B,E") {  // 0x43
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x34);
    LoadData(0x00, { 0x43 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD B, H", "LD-B,H") {  // 0x44
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x34);
    LoadData(0x00, { 0x44 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD B, L", "LD-B,L") {  // 0x45
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x34);
    LoadData(0x00, { 0x45 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD B, <HL>", "LD-B,memHL") {  // 0x46
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x46 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD B, A", "LD-B,A") {  // 0x47
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x47 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD C, B", "LD-C,B") {  // 0x48
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x34);
    LoadData(0x00, { 0x48 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD C, C", "LD-C,C") {  // 0x49
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x34);
    LoadData(0x00, { 0x49 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD C, D", "LD-C,D") {  // 0x4A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x34);
    LoadData(0x00, { 0x4A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD C, E", "LD-C,E") {  // 0x4B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x34);
    LoadData(0x00, { 0x4B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD C, H", "LD-C,H") {  // 0x4C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x34);
    LoadData(0x00, { 0x4C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD C, L", "LD-C,L") {  // 0x4D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x34);
    LoadData(0x00, { 0x4D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD C, <HL>", "LD-C,memHL") {  // 0x4E
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x4E });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD C, A", "LD-C,A") {  // 0x4F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x4F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD D, B", "LD-D,B") {  // 0x50
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x34);
    LoadData(0x00, { 0x50 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD D, C", "LD-D,C") {  // 0x51
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x34);
    LoadData(0x00, { 0x51 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD D, D", "LD-D,D") {  // 0x52
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x34);
    LoadData(0x00, { 0x52 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD D, E", "LD-D,E") {  // 0x53
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x34);
    LoadData(0x00, { 0x53 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD D, H", "LD-D,H") {  // 0x54
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x34);
    LoadData(0x00, { 0x54 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD D, L", "LD-D,L") {  // 0x55
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x34);
    LoadData(0x00, { 0x55 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD D, <HL>", "LD-D,memHL") {  // 0x56
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x56 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD D, A", "LD-D,A") {  // 0x57
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x57 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD E, B", "LD-E,B") {  // 0x58
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x34);
    LoadData(0x00, { 0x58 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD E, C", "LD-E,C") {  // 0x59
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x34);
    LoadData(0x00, { 0x59 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD E, D", "LD-E,D") {  // 0x5A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x34);
    LoadData(0x00, { 0x5A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD E, E", "LD-E,E") {  // 0x5B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x34);
    LoadData(0x00, { 0x5B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD E, H", "LD-E,H") {  // 0x5C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x34);
    LoadData(0x00, { 0x5C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD E, L", "LD-E,L") {  // 0x5D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x34);
    LoadData(0x00, { 0x5D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD E, <HL>", "LD-E,memHL") {  // 0x5E
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x5E });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD E, A", "LD-E,A") {  // 0x5F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x5F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD H, B", "LD-H,B") {  // 0x60
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x34);
    LoadData(0x00, { 0x60 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD H, C", "LD-H,C") {  // 0x61
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x34);
    LoadData(0x00, { 0x61 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD H, D", "LD-H,D") {  // 0x62
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x34);
    LoadData(0x00, { 0x62 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD H, E", "LD-H,E") {  // 0x63
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x34);
    LoadData(0x00, { 0x63 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD H, H", "LD-H,H") {  // 0x64
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x34);
    LoadData(0x00, { 0x64 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD H, L", "LD-H,L") {  // 0x65
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x34);
    LoadData(0x00, { 0x65 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD H, <HL>", "LD-H,memHL") {  // 0x66
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x66 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD H, A", "LD-H,A") {  // 0x67
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x67 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD L, B", "LD-L,B") {  // 0x68
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x34);
    LoadData(0x00, { 0x68 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD L, C", "LD-L,C") {  // 0x69
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x34);
    LoadData(0x00, { 0x69 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD L, D", "LD-L,D") {  // 0x6A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x34);
    LoadData(0x00, { 0x6A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD L, E", "LD-L,E") {  // 0x6B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x34);
    LoadData(0x00, { 0x6B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD L, H", "LD-L,H") {  // 0x6C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x34);
    LoadData(0x00, { 0x6C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD L, L", "LD-L,L") {  // 0x6D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x34);
    LoadData(0x00, { 0x6D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD L, <HL>", "LD-L,memHL") {  // 0x6E
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x6E });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD L, A", "LD-L,A") {  // 0x6F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x6F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD <HL>, B", "LD-HL,B") {  // 0x70
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x34);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x70 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD <HL>, C", "LD-HL,C") {  // 0x71
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x34);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x71 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD <HL>, D", "LD-HL,D") {  // 0x72
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x34);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x72 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD <HL>, E", "LD-HL,E") {  // 0x73
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x34);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x73 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD <HL>, H", "LD-HL,H") {  // 0x74
    cpu->SetHl(0x0405);
    LoadData(0x00, { 0x74 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x0405) == 0x04);
    CHECK(cycles == 2);
}

TEST("LD <HL>, L", "LD-HL,L") {  // 0x75
    cpu->SetHl(0x0405);
    LoadData(0x00, { 0x75 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x0405) == 0x05);
    CHECK(cycles == 2);
}

TEST("LD <HL>, A", "LD-HL,A") {  // 0x77
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x77 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x200) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD A, B", "LD-A,B") {  // 0x78
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x34);
    LoadData(0x00, { 0x78 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD A, C", "LD-A,C") {  // 0x79
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x34);
    LoadData(0x00, { 0x79 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD A, D", "LD-A,D") {  // 0x7A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x34);
    LoadData(0x00, { 0x7A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD A, E", "LD-A,E") {  // 0x7B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x34);
    LoadData(0x00, { 0x7B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD A, H", "LD-A,H") {  // 0x7C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x34);
    LoadData(0x00, { 0x7C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD A, L", "LD-A,L") {  // 0x7D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x34);
    LoadData(0x00, { 0x7D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 1);
}

TEST("LD A, <HL>", "LD-A,memHL") {  // 0x7E
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x34);
    LoadData(0x00, { 0x7E });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD A, A", "LD-A,A") {  // 0x7F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0x7F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 1);
}

TEST("LDH <imm8>, A", "LD-imm8,A") {  // 0xE0
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0xE0, 0x12 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0xFF12) == 0x34);
    CHECK(cycles == 3);
}

TEST("LDH <C>, A", "LDH-C,A") {  // 0xE2
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x12);
    LoadData(0x00, { 0xE2 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0xFF12) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD <a16>, A", "LD-a16,A") {  // 0xEA
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x34);
    LoadData(0x00, { 0xEA, 0x89, 0x67 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x6789) == 0x34);
    CHECK(cycles == 4);
}

TEST("LDH A, <imm8>", "LD-A,imm8") {  // 0xF0
    memory->SetByte(0xFF12, 0x34);
    LoadData(0x00, { 0xF0, 0x12 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 3);
}

TEST("LDH A, <C>", "LD-A,C") {  // 0xF2
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x12);
    memory->SetByte(0xFF12, 0x34);
    LoadData(0x00, { 0xF2 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 2);
}

TEST("LD A, <imm16>", "LD-A,imm16") {  // 0xFA
    memory->SetByte(0x6789, 0x34);
    LoadData(0x00, { 0xFA, 0x89, 0x67 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x34);
    CHECK(cycles == 4);
}
