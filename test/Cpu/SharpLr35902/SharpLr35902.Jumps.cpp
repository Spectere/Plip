/* SharpLr35902.Jumps.cpp
 *
 * Tests the CPU core's ability to perform (un)conditional jumps and calls.
 */

#include "catch2/catch_test_macros.hpp"

#include "SharpLr35902Common.h"

TEST("JR imm8s+", "JR-imm8sP") {  // 0x18
    LoadData(0x100, { 0x18, 0x7E });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0180);
    CHECK(cycles == 3);
}

TEST("JR imm8s-", "JR-imm8sN") {  // 0x18
    LoadData(0x100, { 0x18, 0x80 });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0082);
    CHECK(cycles == 3);
}

TEST("JR NZ, imm8s+ (NZ)", "JR-NZ,imm8sP-NZ") {  // 0x20
    LoadData(0x100, { 0x20, 0x7E });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0180);
    CHECK(cycles == 3);
}

TEST("JR NZ, imm8s+ (Z)", "JR-NZ,imm8sP-Z") {  // 0x20
    LoadData(0x100, { 0x20, 0x7E });
    cpu->Reset(0x100);
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0102);
    CHECK(cycles == 2);
}

TEST("JR NZ, imm8s- (NZ)", "JR-NZ,imm8sN-NZ") {  // 0x20
    LoadData(0x100, { 0x20, 0x80 });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0082);
    CHECK(cycles == 3);
}

TEST("JR NZ, imm8s- (Z)", "JR-NZ,imm8sN-Z") {  // 0x20
    LoadData(0x100, { 0x20, 0x80 });
    cpu->Reset(0x100);
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0102);
    CHECK(cycles == 2);
}

TEST("JR Z, imm8s+ (NZ)", "JR-Z,imm8sP-NZ") {  // 0x28
    LoadData(0x100, { 0x28, 0x7E });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0102);
    CHECK(cycles == 2);
}

TEST("JR Z, imm8s+ (Z)", "JR-Z,imm8sP-Z") {  // 0x28
    LoadData(0x100, { 0x28, 0x7E });
    cpu->Reset(0x100);
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0180);
    CHECK(cycles == 3);
}

TEST("JR Z, imm8s- (NZ)", "JR-Z,imm8sN-NZ") {  // 0x28
    LoadData(0x100, { 0x28, 0x80 });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0102);
    CHECK(cycles == 2);
}

TEST("JR Z, imm8s- (Z)", "JR-Z,imm8sN-Z") {  // 0x28
    LoadData(0x100, { 0x28, 0x80 });
    cpu->Reset(0x100);
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0082);
    CHECK(cycles == 3);
}

TEST("JR NC, imm8s+ (NC)", "JR-NC,imm8sP-NC") {  // 0x30
    LoadData(0x100, { 0x30, 0x7E });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0180);
    CHECK(cycles == 3);
}

TEST("JR NC, imm8s+ (C)", "JR-NC,imm8sP-C") {  // 0x30
    LoadData(0x100, { 0x30, 0x7E });
    cpu->Reset(0x100);
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0102);
    CHECK(cycles == 2);
}

TEST("JR NC, imm8s- (NC)", "JR-NC,imm8sN-NC") {  // 0x30
    LoadData(0x100, { 0x30, 0x80 });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0082);
    CHECK(cycles == 3);
}

TEST("JR NC, imm8s- (C)", "JR-NC,imm8sN-C") {  // 0x30
    LoadData(0x100, { 0x30, 0x80 });
    cpu->Reset(0x100);
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0102);
    CHECK(cycles == 2);
}

TEST("JR C, imm8s+ (NC)", "JR-C,imm8sP-NC") {  // 0x38
    LoadData(0x100, { 0x38, 0x7E });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0102);
    CHECK(cycles == 2);
}

TEST("JR C, imm8s+ (C)", "JR-C,imm8sP-C") {  // 0x38
    LoadData(0x100, { 0x38, 0x7E });
    cpu->Reset(0x100);
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0180);
    CHECK(cycles == 3);
}

TEST("JR C, imm8s- (NC)", "JR-C,imm8sN-NC") {  // 0x38
    LoadData(0x100, { 0x38, 0x80 });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0102);
    CHECK(cycles == 2);
}

TEST("JR C, imm8s- (C)", "JR-C,imm8sN-C") {  // 0x38
    LoadData(0x100, { 0x38, 0x80 });
    cpu->Reset(0x100);
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x0082);
    CHECK(cycles == 3);
}

TEST("RET", "RET") {  // 0xC9
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, { 0xC9 });
    cpu->SetSp(0x1FE);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x100);
    CHECK(cycles == 4);
}

TEST("RET NZ (NZ)", "RET-NZ-NZ") {  // 0xC0
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, { 0xC0 });
    cpu->SetSp(0x1FE);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x100);
    CHECK(cycles == 5);
}

TEST("RET NZ (Z)", "RET-NZ-Z") {  // 0xC0
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, { 0xC0 });
    cpu->SetSp(0x1FE);
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cycles == 2);
}

TEST("RET Z (NZ)", "RET-Z-NZ") {  // 0xC8
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, { 0xC8 });
    cpu->SetSp(0x1FE);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x001);
    CHECK(cycles == 2);
}

TEST("RET Z (Z)", "RET-Z-Z") {  // 0xC8
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, { 0xC8 });
    cpu->SetSp(0x1FE);
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x100);
    CHECK(cycles == 5);
}

TEST("RET NC (NC)", "RET-NC-NC") {  // 0xD0
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, { 0xD0 });
    cpu->SetSp(0x1FE);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x100);
    CHECK(cycles == 5);
}

TEST("RET NC (C)", "RET-NC-C") {  // 0xD0
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, { 0xD0 });
    cpu->SetSp(0x1FE);
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cycles == 2);
}

TEST("RET C (NC)", "RET-C-NC") {  // 0xD8
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, { 0xD8 });
    cpu->SetSp(0x1FE);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x001);
    CHECK(cycles == 2);
}

TEST("RET C (C)", "RET-C-C") {  // 0xD8
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, { 0xD8 });
    cpu->SetSp(0x1FE);
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x100);
    CHECK(cycles == 5);
}

TEST("JP imm16", "JP-imm16") {  // 0xC3
    LoadData(0x00, { 0xC3, 0x34, 0x12 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cycles == 4);
}

TEST("JP NZ, imm16 (NZ)", "JP-NZ,imm16-NZ") {  // 0xC2
    LoadData(0x00, { 0xC2, 0x34, 0x12 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cycles == 4);
}

TEST("JP NZ, imm16 (Z)", "JP-NZ,imm16-Z") {  // 0xC2
    LoadData(0x00, { 0xC2, 0x34, 0x12 });
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x03);
    CHECK(cycles == 3);
}

TEST("JP Z, imm16 (NZ)", "JP-Z,imm16-NZ") {  // 0xCA
    LoadData(0x00, { 0xCA, 0x34, 0x12 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x03);
    CHECK(cycles == 3);
}

TEST("JP Z, imm16 (Z)", "JP-Z,imm16-Z") {  // 0xCA
    LoadData(0x00, { 0xCA, 0x34, 0x12 });
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cycles == 4);
}

TEST("JP NC, imm16 (NC)", "JP-NC,imm16-NC") {  // 0xD2
    LoadData(0x00, { 0xD2, 0x34, 0x12 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cycles == 4);
}

TEST("JP NC, imm16 (C)", "JP-NC,imm16-C") {  // 0xD2
    LoadData(0x00, { 0xD2, 0x34, 0x12 });
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x03);
    CHECK(cycles == 3);
}

TEST("JP C, imm16 (NC)", "JP-C,imm16-NC") {  // 0xDA
    LoadData(0x00, { 0xDA, 0x34, 0x12 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x03);
    CHECK(cycles == 3);
}

TEST("JP C, imm16 (C)", "JP-C,imm16-C") {  // 0xDA
    LoadData(0x00, { 0xDA, 0x34, 0x12 });
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cycles == 4);
}

TEST("RST $00", "RST-00") {  // 0xC7
    LoadData(0x100, { 0xC7 });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cycles == 4);
}

TEST("RST $08", "RST-08") {  // 0xCF
    LoadData(0x100, { 0xCF });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x08);
    CHECK(cycles == 4);
}

TEST("RST $10", "RST-10") {  // 0xD7
    LoadData(0x100, { 0xD7 });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x10);
    CHECK(cycles == 4);
}

TEST("RST $18", "RST-18") {  // 0xDF
    LoadData(0x100, { 0xDF });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x18);
    CHECK(cycles == 4);
}

TEST("RST $20", "RST-20") {  // 0xE7
    LoadData(0x100, { 0xE7 });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x20);
    CHECK(cycles == 4);
}

TEST("RST $28", "RST-28") {  // 0xEF
    LoadData(0x100, { 0xEF });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x28);
    CHECK(cycles == 4);
}

TEST("RST $30", "RST-30") {  // 0xF7
    LoadData(0x100, { 0xF7 });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x30);
    CHECK(cycles == 4);
}

TEST("RST $38", "RST-38") {  // 0xFF
    LoadData(0x100, { 0xFF });
    cpu->Reset(0x100);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x38);
    CHECK(cycles == 4);
}

TEST("JP HL", "JP-HL") {  // 0xE9
    cpu->SetHl(0x100);
    LoadData(0x00, { 0xE9 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x100);
    CHECK(cycles == 1);
}

TEST("CALL imm16", "CALL-imm16") {  // 0xCD
    LoadData(0x100, { 0xCD, 0x34, 0x12 });
    cpu->Reset(0x100);
    cpu->SetSp(0x200);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cpu->GetSp() == 0x1FE);
    CHECK(memory->GetByte(0x1FF) == 0x01);
    CHECK(memory->GetByte(0x1FE) == 0x03);
    CHECK(cycles == 6);
}

TEST("CALL NZ, imm16 (NZ)", "CALL-NZ,imm16-NZ") {  // 0xC4
    LoadData(0x100, { 0xC4, 0x34, 0x12 });
    cpu->Reset(0x100);
    cpu->SetSp(0x200);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cpu->GetSp() == 0x1FE);
    CHECK(memory->GetByte(0x1FF) == 0x01);
    CHECK(memory->GetByte(0x1FE) == 0x03);
    CHECK(cycles == 6);
}

TEST("CALL NZ, imm16 (Z)", "CALL-NZ,imm16-Z") {  // 0xC4
    LoadData(0x100, { 0xC4, 0x34, 0x12 });
    cpu->Reset(0x100);
    cpu->SetSp(0x200);
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x103);
    CHECK(cpu->GetSp() == 0x200);
    CHECK(memory->GetByte(0x1FF) == 0x00);
    CHECK(memory->GetByte(0x1FE) == 0x00);
    CHECK(cycles == 3);
}

TEST("CALL Z, imm16 (NZ)", "CALL-Z,imm16-NZ") {  // 0xCC
    LoadData(0x100, { 0xCC, 0x34, 0x12 });
    cpu->Reset(0x100);
    cpu->SetSp(0x200);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x103);
    CHECK(cpu->GetSp() == 0x200);
    CHECK(memory->GetByte(0x1FF) == 0x00);
    CHECK(memory->GetByte(0x1FE) == 0x00);
    CHECK(cycles == 3);
}

TEST("CALL Z, imm16 (Z)", "CALL-Z,imm16-Z") {  // 0xCC
    LoadData(0x100, { 0xCC, 0x34, 0x12 });
    cpu->Reset(0x100);
    cpu->SetSp(0x200);
    cpu->SetZeroFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cpu->GetSp() == 0x1FE);
    CHECK(memory->GetByte(0x1FF) == 0x01);
    CHECK(memory->GetByte(0x1FE) == 0x03);
    CHECK(cycles == 6);
}

TEST("CALL NC, imm16 (NC)", "CALL-NC,imm16-NC") {  // 0xD4
    LoadData(0x100, { 0xD4, 0x34, 0x12 });
    cpu->Reset(0x100);
    cpu->SetSp(0x200);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cpu->GetSp() == 0x1FE);
    CHECK(memory->GetByte(0x1FF) == 0x01);
    CHECK(memory->GetByte(0x1FE) == 0x03);
    CHECK(cycles == 6);
}

TEST("CALL NC, imm16 (C)", "CALL-NC,imm16-C") {  // 0xD4
    LoadData(0x100, { 0xD4, 0x34, 0x12 });
    cpu->Reset(0x100);
    cpu->SetSp(0x200);
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x103);
    CHECK(cpu->GetSp() == 0x200);
    CHECK(memory->GetByte(0x1FF) == 0x00);
    CHECK(memory->GetByte(0x1FE) == 0x00);
    CHECK(cycles == 3);
}

TEST("CALL C, imm16 (NC)", "CALL-C,imm16-NC") {  // 0xDC
    LoadData(0x100, { 0xDC, 0x34, 0x12 });
    cpu->Reset(0x100);
    cpu->SetSp(0x200);

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x103);
    CHECK(cpu->GetSp() == 0x200);
    CHECK(memory->GetByte(0x1FF) == 0x00);
    CHECK(memory->GetByte(0x1FE) == 0x00);
    CHECK(cycles == 3);
}

TEST("CALL C, imm16 (C)", "CALL-C,imm16-C") {  // 0xDC
    LoadData(0x100, { 0xDC, 0x34, 0x12 });
    cpu->Reset(0x100);
    cpu->SetSp(0x200);
    cpu->SetCarryFlag();

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetPc() == 0x1234);
    CHECK(cpu->GetSp() == 0x1FE);
    CHECK(memory->GetByte(0x1FF) == 0x01);
    CHECK(memory->GetByte(0x1FE) == 0x03);
    CHECK(cycles == 6);
}
