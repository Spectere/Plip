/* SharpLr35902.Ld16.cpp
 *
 * Tests the CPU core's ability to perform 8-bit arithmetic and logical operations.
 */

#include "catch2/catch_test_macros.hpp"

#include "SharpLr35902Common.h"

TEST("INC B (NH, NZ)", "INC-B-NH-NZ") {  // 0x04
    LoadData(0x00, { 0x04 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC B (H, NZ)", "INC-B-H-NZ") {  // 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0F);
    LoadData(0x00, { 0x04 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x10);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC B (H, Z)", "INC-B-H-Z") {  // 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0xFF);
    LoadData(0x00, { 0x04 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x00);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC B (NH, NZ)", "DEC-B-NH-NZ") {  // 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x05 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("DEC B (NH, Z)", "DEC-B-NH-Z") {  // 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    LoadData(0x00, { 0x05 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x00);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC B (H, NZ)", "DEC-B-H-NZ") {  // 0x05
    LoadData(0x00, { 0x05 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0xFF);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC C (NH, NZ)", "INC-C-NH-NZ") {  // 0x0C
    LoadData(0x00, { 0x0C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC C (H, NZ)", "INC-C-H-NZ") {  // 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0F);
    LoadData(0x00, { 0x0C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x10);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC C (H, Z)", "INC-C-H-Z") {  // 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0xFF);
    LoadData(0x00, { 0x0C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x00);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC C (NH, NZ)", "DEC-C-NH-NZ") {  // 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x0D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("DEC C (NH, Z)", "DEC-C-NH-Z") {  // 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    LoadData(0x00, { 0x0D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x00);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC C (H, NZ)", "DEC-C-H-NZ") {  // 0x0D
    LoadData(0x00, { 0x0D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0xFF);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC D (NH, NZ)", "INC-D-NH-NZ") {  // 0x14
    LoadData(0x00, { 0x14 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC D (H, NZ)", "INC-D-H-NZ") {  // 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0F);
    LoadData(0x00, { 0x14 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x10);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC D (H, Z)", "INC-D-H-Z") {  // 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0xFF);
    LoadData(0x00, { 0x14 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x00);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC D (NH, NZ)", "DEC-D-NH-NZ") {  // 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x15 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("DEC D (NH, Z)", "DEC-D-NH-Z") {  // 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    LoadData(0x00, { 0x15 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x00);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC D (H, NZ)", "DEC-D-H-NZ") {  // 0x15
    LoadData(0x00, { 0x15 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0xFF);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC E (NH, NZ)", "INC-E-NH-NZ") {  // 0x1C
    LoadData(0x00, { 0x1C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC E (H, NZ)", "INC-E-H-NZ") {  // 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0F);
    LoadData(0x00, { 0x1C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x10);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC E (H, Z)", "INC-E-H-Z") {  // 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0xFF);
    LoadData(0x00, { 0x1C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x00);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC E (NH, NZ)", "DEC-E-NH-NZ") {  // 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x1D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("DEC E (NH, Z)", "DEC-E-NH-Z") {  // 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    LoadData(0x00, { 0x1D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x00);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC E (H, NZ)", "DEC-E-H-NZ") {  // 0x1D
    LoadData(0x00, { 0x1D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0xFF);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC H (NH, NZ)", "INC-H-NH-NZ") {  // 0x24
    LoadData(0x00, { 0x24 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC H (H, NZ)", "INC-H-H-NZ") {  // 0x24
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0F);
    LoadData(0x00, { 0x24 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x10);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC H (H, Z)", "INC-H-H-Z") {  // 0x24
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0xFF);
    LoadData(0x00, { 0x24 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x00);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC H (NH, NZ)", "DEC-H-NH-NZ") {  // 0x25
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x25 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("DEC H (NH, Z)", "DEC-H-NH-Z") {  // 0x25
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    LoadData(0x00, { 0x25 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x00);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC H (H, NZ)", "DEC-H-H-NZ") {  // 0x25
    LoadData(0x00, { 0x25 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0xFF);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC L (NH, NZ)", "INC-L-NH-NZ") {  // 0x2C
    LoadData(0x00, { 0x2C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC L (H, NZ)", "INC-L-H-NZ") {  // 0x2C
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0F);
    LoadData(0x00, { 0x2C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x10);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC L (H, Z)", "INC-L-H-Z") {  // 0x2C
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0xFF);
    LoadData(0x00, { 0x2C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x00);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC L (NH, NZ)", "DEC-L-NH-NZ") {  // 0x2D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x2D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("DEC L (NH, Z)", "DEC-L-NH-Z") {  // 0x2D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    LoadData(0x00, { 0x2D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x00);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC L (H, NZ)", "DEC-L-H-NZ") {  // 0x2D
    LoadData(0x00, { 0x2D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0xFF);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC <HL> (NH, NZ)", "INC-HL-NH-NZ") {  // 0x34
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x34 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);
}

TEST("INC <HL> (H, NZ)", "INC-HL-H-NZ") {  // 0x34
    memory->SetByte(0x200, 0x0F);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x34 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x10);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);
}

TEST("INC <HL> (H, Z)", "INC-HL-H-Z") {  // 0x34
    memory->SetByte(0x200, 0xFF);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x34 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x00);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 3);
}

TEST("DEC <HL> (NH, NZ)", "DEC-HL-NH-NZ") {  // 0x35
    memory->SetByte(0x200, 0x02);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x35 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);
}

TEST("DEC <HL> (NH, Z)", "DEC-HL-NH-Z") {  // 0x35
    memory->SetByte(0x200, 0x01);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x35 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x00);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 3);
}

TEST("DEC <HL> (H, NZ)", "DEC-HL-H-NZ") {  // 0x35
    memory->SetByte(0x200, 0x00);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x35 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0xFF);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 3);
}

TEST("INC A (NH, NZ)", "INC-A-NH-NZ") {  // 0x3C
    LoadData(0x00, { 0x3C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC A (H, NZ)", "INC-A-H-NZ") {  // 0x3C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x0F);
    LoadData(0x00, { 0x3C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("INC A (H, Z)", "INC-A-H-Z") {  // 0x3C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0x3C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC A (NH, NZ)", "DEC-A-NH-NZ") {  // 0x3D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x02);
    LoadData(0x00, { 0x3D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("DEC A (NH, Z)", "DEC-A-NH-Z") {  // 0x3D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0x3D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("DEC A (H, NZ)", "DEC-A-H-NZ") {  // 0x3D
    LoadData(0x00, { 0x3D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (NS, NH, NC) (NZ, NC)", "DAA-NS,NH,NC-NZ,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (NS, H, NC) (NZ, NC)", "DAA-NS,NH,NC-NZ,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHalfCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x07);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (NS, IH, NC) (NZ, NC)", "DAA-NS,IH,NC-NZ,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x0A);
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (NS, NH, C) (NZ, C)", "DAA-NS,NH,C-NZ,C") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x60);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 1);
}

TEST("DAA (NS, NH, IC) (NZ, C)", "DAA-NS,NH,IC-NZ,C") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xA1);
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 1);
}

TEST("DAA (NS, H, C) (NZ, C)", "DAA-NS,H,C-NZ,C") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x00);
    cpu->SetHalfCarryFlag();
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x66);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 1);
}

TEST("DAA (NS, IH, C) (NZ, C)", "DAA-NS,IH,C-NZ,C") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x0A);
    cpu->SetHalfCarryFlag();
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x70);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 1);
}

TEST("DAA (NS, H, IC) (NZ, C)", "DAA-NS,H,IC-NZ,C") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xA0);
    cpu->SetHalfCarryFlag();
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x06);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 1);
}

TEST("DAA (NS, IH, IC) (NZ, C)", "DAA-NS,IH,IC-NZ,C") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xAA);
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 1);
}

TEST("DAA (NS, NH, IC) (Z, C)", "DAA-NS,NH,IC-Z,C") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xA0);
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 1);
}

TEST("DAA (S, NH, NC) (NZ, NC)", "DAA-NS,NH,NC-NZ,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetSubtractFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (S, H, NC) (NZ, NC)", "DAA-S-H,NC-NZ,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x07);
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (S, H, NC) (Z, NC)", "DAA-S-H,NC-Z,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x06);
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_SET;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (S, NH, C) (NZ, NC)", "DAA-S-NH,C-NZ,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x70);
    cpu->SetSubtractFlag();
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (S, NH, C) (Z, NC)", "DAA-S-NH,C-Z,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x60);
    cpu->SetSubtractFlag();
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_SET;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (S, H, C) (NZ, NC)", "DAA-S-H,C-NZ,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x77);
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x11);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("DAA (S, H, C) (Z, NC)", "DAA-S-H,C-Z,NC") {  // 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x66);
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x27 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_SET;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("CPL (NZ, NC)", "CPL-NZ,NC") {  // 0x2F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0x2F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_HALF_SET;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 1);
}

TEST("CPL (Z, C)", "CPL-Z,C") {  // 0x2F
    cpu->SetZeroFlag();
    cpu->SetCarryFlag();
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0x2F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_SET;
    CHECK_HALF_SET;
    CHECK_CARRY_SET;
    CHECK(cycles == 1);
}

TEST("SCF (NZ)", "SCF-NZ") {  // 0x37
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    LoadData(0x00, { 0x37 });

    const auto cycles = cpu->Cycle();
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SCF (Z)", "SCF-Z") {  // 0x37
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    cpu->SetZeroFlag();
    LoadData(0x00, { 0x37 });

    const auto cycles = cpu->Cycle();
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("CCF (NC, NZ)", "CCF-NC-NZ") {  // 0x3F
    cpu->SetCarryFlag();
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    LoadData(0x00, { 0x3F });

    const auto cycles = cpu->Cycle();
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CCF (C, NZ)", "CCF-C-NZ") {  // 0x3F
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    LoadData(0x00, { 0x3F });

    const auto cycles = cpu->Cycle();
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CCF (NC, Z)", "CCF-NC-Z") {  // 0x3F
    cpu->SetCarryFlag();
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    cpu->SetZeroFlag();
    LoadData(0x00, { 0x3F });

    const auto cycles = cpu->Cycle();
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("CCF (C, Z)", "CCF-C-Z") {  // 0x3F
    cpu->SetSubtractFlag();
    cpu->SetHalfCarryFlag();
    cpu->SetZeroFlag();
    LoadData(0x00, { 0x3F });

    const auto cycles = cpu->Cycle();
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADD A, B (NC, NH, NZ)", "ADD-A,B-NC-NH-NZ") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0E);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, B (C, NH, NZ)", "ADD-A,B-C-NH-NZ") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x20);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, B (NC, H, NZ)", "ADD-A,B-NC-H-NZ") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0F);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, B (C, H, NZ)", "ADD-A,B-C-H-NZ") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, B (C, H, Z)", "ADD-A,B-C-H-Z") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADD A, C (NC, NH, NZ)", "ADD-A,C-NC-NH-NZ") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0E);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, C (C, NH, NZ)", "ADD-A,C-C-NH-NZ") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x20);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, C (NC, H, NZ)", "ADD-A,C-NC-H-NZ") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0F);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, C (C, H, NZ)", "ADD-A,C-C-H-NZ") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, C (C, H, Z)", "ADD-A,C-C-H-Z") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADD A, D (NC, NH, NZ)", "ADD-A,D-NC-NH-NZ") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0E);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, D (C, NH, NZ)", "ADD-A,D-C-NH-NZ") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x20);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, D (NC, H, NZ)", "ADD-A,D-NC-H-NZ") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0F);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, D (C, H, NZ)", "ADD-A,D-C-H-NZ") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, D (C, H, Z)", "ADD-A,D-C-H-Z") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADD A, E (NC, NH, NZ)", "ADD-A,E-NC-NH-NZ") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0E);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, E (C, NH, NZ)", "ADD-A,E-C-NH-NZ") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x20);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, E (NC, H, NZ)", "ADD-A,E-NC-H-NZ") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0F);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, E (C, H, NZ)", "ADD-A,E-C-H-NZ") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, E (C, H, Z)", "ADD-A,E-C-H-Z") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADD A, H (NC, NH, NZ)", "ADD-A,H-NC-NH-NZ") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0E);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, H (C, NH, NZ)", "ADD-A,H-C-NH-NZ") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x20);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, H (NC, H, NZ)", "ADD-A,H-NC-H-NZ") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0F);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, H (C, H, NZ)", "ADD-A,H-C-H-NZ") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, H (C, H, Z)", "ADD-A,H-C-H-Z") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADD A, L (NC, NH, NZ)", "ADD-A,L-NC-NH-NZ") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0E);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, L (C, NH, NZ)", "ADD-A,L-C-NH-NZ") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x20);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, L (NC, H, NZ)", "ADD-A,L-NC-H-NZ") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0F);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, L (C, H, NZ)", "ADD-A,L-C-H-NZ") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, L (C, H, Z)", "ADD-A,L-C-H-Z") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADD A, <HL> (NC, NH, NZ)", "ADD-A,HL-NC-NH-NZ") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x0E);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD A, <HL> (C, NH, NZ)", "ADD-A,HL-C-NH-NZ") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x20);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD A, <HL> (NC, H, NZ)", "ADD-A,HL-NC-H-NZ") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x0F);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD A, <HL> (C, H, NZ)", "ADD-A,HL-C-H-NZ") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD A, <HL> (C, H, Z)", "ADD-A,HL-C-H-Z") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x01);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("ADD A, A (NC, NH, NZ)", "ADD-A,A-NC-NH-NZ") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x02);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, A (C, NH, NZ)", "ADD-A,A-C-NH-NZ") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x90);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x20);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, A (NC, H, NZ)", "ADD-A,A-NC-H-NZ") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, A (C, H, NZ)", "ADD-A,A-C-H-NZ") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x99);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x32);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADD A, A (C, NH, Z)", "ADD-A,A-C-NH-Z") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x80);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADD A, imm8 (NC, NH, NZ)", "ADD-A,imm8-NC-NH-NZ") {  // 0xC6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0xC6, 0x0E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD A, imm8 (C, NH, NZ)", "ADD-A,imm8-C-NH-NZ") {  // 0xC6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    LoadData(0x00, { 0xC6, 0x20 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD A, imm8 (NC, H, NZ)", "ADD-A,imm8-NC-H-NZ") {  // 0xC6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0xC6, 0x0F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD A, imm8 (C, H, NZ)", "ADD-A,imm8-C-H-NZ") {  // 0xC6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0xC6, 0x02 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADD A, imm8 (C, H, Z)", "ADD-A,imm8-C-H-Z") {  // 0xC6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0xC6, 0x01 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("ADC A, B (NC, NH, NZ)", "ADC-A,B-NC-NH-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0E);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, B (C, NH, NZ)", "ADC-A,B-C-NH-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x20);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, B (NC, H, NZ)", "ADC-A,B-NC-H-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0F);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, B (C, H, NZ)", "ADC-A,B-C-H-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, B (C, H, Z)", "ADC-A,B-C-H-Z") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (NC, NH, NZ)", "ADC-A,B-C-NC-NH-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (C, NH, NZ)", "ADC-A,B-C-C-NH-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (NC, H, NZ)", "ADC-A,B-C-NC-H-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (C, H, NZ)", "ADC-A,B-C-C-H-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (C, NH, Z)", "ADC-A,B-C-C-NH-Z") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (C, H, NZ) (OF)", "ADB-A,B-C-C-H-NZ-OF") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (NC, NH, NZ)", "ADC-A,C-NC-NC-NH-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0E);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (C, NH, NZ)", "ADC-A,C-NC-C-NH-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x20);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (NC, H, NZ)", "ADC-A,C-NC-NC-H-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0F);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (C, H, NZ)", "ADC-A,C-NC-C-H-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (C, H, Z)", "ADC-A,C-NC-C-H-Z") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (NC, NH, NZ)", "ADC-A,C-C-NC-NH-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (C, NH, NZ)", "ADC-A,C-C-C-NH-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (NC, H, NZ)", "ADC-A,C-C-NC-H-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (C, H, NZ)", "ADC-A,C-C-C-H-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (C, H, Z)", "ADC-A,C-C-C-H-Z") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (C, NH, NZ) (OF)", "ADB-A,C-C-C-NH-NZ-OF") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (NC, NH, NZ)", "ADC-A,D-NC-NC-NH-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0E);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (C, NH, NZ)", "ADC-A,D-NC-C-NH-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x20);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (NC, H, NZ)", "ADC-A,D-NC-NC-H-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0F);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (C, H, NZ)", "ADC-A,D-NC-C-H-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (C, H, Z)", "ADC-A,D-NC-C-H-Z") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (NC, NH, NZ)", "ADC-A,D-C-NC-NH-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (C, NH, NZ)", "ADC-A,D-C-C-NH-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (NC, H, NZ)", "ADC-A,D-C-NC-H-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (C, H, NZ)", "ADC-A,D-C-C-H-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (C, H, Z)", "ADC-A,D-C-C-H-Z") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (C, NH, NZ) (OF)", "ADB-A,D-C-C-NH-NZ-OF") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (NC, NH, NZ)", "ADC-A,E-NC-NC-NH-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0E);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (C, NH, NZ)", "ADC-A,E-NC-C-NH-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x20);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (NC, H, NZ)", "ADC-A,E-NC-NC-H-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0F);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (C, H, NZ)", "ADC-A,E-NC-C-H-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (C, H, Z)", "ADC-A,E-NC-C-H-Z") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (NC, NH, NZ)", "ADC-A,E-C-NC-NH-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (C, NH, NZ)", "ADC-A,E-C-C-NH-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (NC, H, NZ)", "ADC-A,E-C-NC-H-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (C, H, NZ)", "ADC-A,E-C-C-H-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (C, H, Z)", "ADC-A,E-C-C-H-Z") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (C, NH, NZ) (OF)", "ADB-A,E-C-C-NH-NZ-OF") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (NC, NH, NZ)", "ADC-A,H-NC-NC-NH-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0E);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (C, NH, NZ)", "ADC-A,H-NC-C-NH-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x20);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (NC, H, NZ)", "ADC-A,H-NC-NC-H-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0F);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (C, H, NZ)", "ADC-A,H-NC-C-H-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (C, H, Z)", "ADC-A,H-NC-C-H-Z") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (NC, NH, NZ)", "ADC-A,H-C-NC-NH-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (C, NH, NZ)", "ADC-A,H-C-C-NH-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (NC, H, NZ)", "ADC-A,H-C-NC-H-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (C, H, NZ)", "ADC-A,H-C-C-H-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (C, H, Z)", "ADC-A,H-C-C-H-Z") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (C, NH, NZ) (OF)", "ADB-A,H-C-C-NH-NZ-OF") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (NC, NH, NZ)", "ADC-A,L-NC-NC-NH-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0E);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (C, NH, NZ)", "ADC-A,L-NC-C-NH-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x20);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (NC, H, NZ)", "ADC-A,L-NC-NC-H-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0F);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (C, H, NZ)", "ADC-A,L-NC-C-H-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (C, H, Z)", "ADC-A,L-NC-C-H-Z") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (NC, NH, NZ)", "ADC-A,L-C-NC-NH-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (C, NH, NZ)", "ADC-A,L-C-C-NH-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (NC, H, NZ)", "ADC-A,L-C-NC-H-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (C, H, NZ)", "ADC-A,L-C-C-H-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (C, H, Z)", "ADC-A,L-C-C-H-Z") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (C, NH, NZ) (OF)", "ADB-A,L-C-C-NH-NZ-OF") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, <HL> (NC) (NC, NH, NZ)", "ADC-A,HL-NC-NC-NH-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x0E);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (NC) (C, NH, NZ)", "ADC-A,HL-NC-C-NH-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x20);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (NC) (NC, H, NZ)", "ADC-A,HL-NC-NC-H-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x0F);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (NC) (C, H, NZ)", "ADC-A,HL-NC-C-H-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (NC) (C, H, Z)", "ADC-A,HL-NC-C-H-Z") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x01);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (C) (NC, NH, NZ)", "ADC-A,HL-C-NC-NH-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    cpu->SetCarryFlag();
    memory->SetByte(0x200, 0x0D);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (C) (C, NH, NZ)", "ADC-A,HL-C-C-NH-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetHl(0x200);
    cpu->SetCarryFlag();
    memory->SetByte(0x200, 0x1F);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (C) (NC, H, NZ)", "ADC-A,HL-C-NC-H-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    cpu->SetCarryFlag();
    memory->SetByte(0x200, 0x0E);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (C) (C, H, NZ)", "ADC-A,HL-C-C-H-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    cpu->SetCarryFlag();
    memory->SetByte(0x200, 0x01);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (C) (C, H, Z)", "ADC-A,HL-C-C-H-Z") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    cpu->SetCarryFlag();
    memory->SetByte(0x200, 0x00);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (C) (C, NH, NZ) (OF)", "ADB-A,HL-C-C-NH-NZ-OF") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    cpu->SetCarryFlag();
    memory->SetByte(0x200, 0xFF);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, A (NC) (NC, NH, NZ)", "ADC-A,A-NC-NC-NH-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x02);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, A (NC) (C, NH, NZ)", "ADC-A,A-NC-C-NH-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x90);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x20);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, A (NC) (NC, H, NZ)", "ADC-A,A-NC-NC-H-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, A (NC) (C, H, NZ)", "ADC-A,A-NC-C-H-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x99);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x32);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, A (NC) (C, NH, Z)", "ADC-A,A-NC-C-NH-Z") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x80);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("ADC A, A (C) (NC, NH, NZ)", "ADC-A,A-C-NC-NH-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x02);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x05);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, A (C) (NC, H, NZ)", "ADC-A,A-C-NC-H-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xE);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x1D);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, A (C) (C, NH, NZ)", "ADC-A,A-C-C-NH-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x81);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x03);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, A (C) (C, H, NZ)", "ADC-A,A-C-C-H-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x88);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x11);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, A (C) (C, NH, NZ) (OF)", "ADB-A,L-C-C-NH-NZ-OF") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("ADC A, imm8 (NC) (NC, NH, NZ)", "ADC-A,imm8-NC-NC-NH-NZ") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0xCE, 0x0E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (NC) (C, NH, NZ)", "ADC-A,imm8-NC-C-NH-NZ") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    LoadData(0x00, { 0xCE, 0x20 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (NC) (NC, H, NZ)", "ADC-A,imm8-NC-NC-H-NZ") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0xCE, 0x0F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (NC) (C, H, NZ)", "ADC-A,imm8-NC-C-H-NZ") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0xCE, 0x02 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (NC) (C, H, Z)", "ADC-A,imm8-NC-C-H-Z") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0xCE, 0x01 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (C) (NC, NH, NZ)", "ADC-A,imm8-C-NC-NH-NZ") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCE, 0x0D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (C) (C, NH, NZ)", "ADC-A,imm8-C-C-NH-NZ") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCE, 0x1F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (C) (NC, H, NZ)", "ADC-A,imm8-C-NC-H-NZ") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCE, 0x0E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (C) (C, H, NZ)", "ADC-A,imm8-C-C-H-NZ") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCE, 0x01 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (C) (C, H, Z)", "ADC-A,imm8-C-C-H-Z") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCE, 0x00 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("ADC A, imm8 (C) (C, NH, NZ) (OF)", "Aimm8-A,HL-C-C-NH-NZ-OF") {  // 0xCE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCE, 0xFF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SUB A, B (NC, NH, NZ)", "SUB-A,B-NC-NH-NZ") {  // 0x90
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x03);
    LoadData(0x00, { 0x90 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, B (NC, H, NZ)", "SUB-A,B-NC-H-NZ") {  // 0x90
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x90 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, B (C, NH, NZ)", "SUB-A,B-C-NH-NZ") {  // 0x90
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x10);
    LoadData(0x00, { 0x90 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, B (C, H, NZ)", "SUB-A,B-C-H-NZ") {  // 0x90
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x90 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, B (NC, NH, Z)", "SUB-A,B-NC-NH-Z") {  // 0x90
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0xFF);
    LoadData(0x00, { 0x90 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SUB A, C (NC, NH, NZ)", "SUB-A,C-NC-NH-NZ") {  // 0x91
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x03);
    LoadData(0x00, { 0x91 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, C (NC, H, NZ)", "SUB-A,C-NC-H-NZ") {  // 0x91
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x91 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, C (C, NH, NZ)", "SUB-A,C-C-NH-NZ") {  // 0x91
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x10);
    LoadData(0x00, { 0x91 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, C (C, H, NZ)", "SUB-A,C-C-H-NZ") {  // 0x91
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x91 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, C (NC, NH, Z)", "SUB-A,C-NC-NH-Z") {  // 0x91
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0xFF);
    LoadData(0x00, { 0x91 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SUB A, D (NC, NH, NZ)", "SUB-A,D-NC-NH-NZ") {  // 0x92
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x03);
    LoadData(0x00, { 0x92 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, D (NC, H, NZ)", "SUB-A,D-NC-H-NZ") {  // 0x92
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x92 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, D (C, NH, NZ)", "SUB-A,D-C-NH-NZ") {  // 0x92
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x10);
    LoadData(0x00, { 0x92 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, D (C, H, NZ)", "SUB-A,D-C-H-NZ") {  // 0x92
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x92 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, D (NC, NH, Z)", "SUB-A,D-NC-NH-Z") {  // 0x92
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0xFF);
    LoadData(0x00, { 0x92 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SUB A, E (NC, NH, NZ)", "SUB-A,E-NC-NH-NZ") {  // 0x93
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x03);
    LoadData(0x00, { 0x93 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, E (NC, H, NZ)", "SUB-A,E-NC-H-NZ") {  // 0x93
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x93 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, E (C, NH, NZ)", "SUB-A,E-C-NH-NZ") {  // 0x93
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x10);
    LoadData(0x00, { 0x93 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, E (C, H, NZ)", "SUB-A,E-C-H-NZ") {  // 0x93
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x93 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, E (NC, NH, Z)", "SUB-A,E-NC-NH-Z") {  // 0x93
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0xFF);
    LoadData(0x00, { 0x93 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SUB A, H (NC, NH, NZ)", "SUB-A,H-NC-NH-NZ") {  // 0x94
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x03);
    LoadData(0x00, { 0x94 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, H (NC, H, NZ)", "SUB-A,H-NC-H-NZ") {  // 0x94
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x94 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, H (C, NH, NZ)", "SUB-A,H-C-NH-NZ") {  // 0x94
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x10);
    LoadData(0x00, { 0x94 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, H (C, H, NZ)", "SUB-A,H-C-H-NZ") {  // 0x94
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x94 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, H (NC, NH, Z)", "SUB-A,H-NC-NH-Z") {  // 0x94
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0xFF);
    LoadData(0x00, { 0x94 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SUB A, L (NC, NH, NZ)", "SUB-A,L-NC-NH-NZ") {  // 0x95
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x03);
    LoadData(0x00, { 0x95 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, L (NC, H, NZ)", "SUB-A,L-NC-H-NZ") {  // 0x95
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x95 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, L (C, NH, NZ)", "SUB-A,L-C-NH-NZ") {  // 0x95
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x10);
    LoadData(0x00, { 0x95 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, L (C, H, NZ)", "SUB-A,L-C-H-NZ") {  // 0x95
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x95 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SUB A, L (NC, NH, Z)", "SUB-A,L-NC-NH-Z") {  // 0x95
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0xFF);
    LoadData(0x00, { 0x95 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SUB A, <HL> (NC, NH, NZ)", "SUB-A,HL-NC-NH-NZ") {  // 0x96
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x03);
    LoadData(0x00, { 0x96 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SUB A, <HL> (NC, H, NZ)", "SUB-A,HL-NC-H-NZ") {  // 0x96
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0x96 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SUB A, <HL> (C, NH, NZ)", "SUB-A,HL-C-NH-NZ") {  // 0x96
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x10);
    LoadData(0x00, { 0x96 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SUB A, <HL> (C, H, NZ)", "SUB-A,HL-C-H-NZ") {  // 0x96
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0x96 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SUB A, <HL> (NC, NH, Z)", "SUB-A,HL-NC-NH-Z") {  // 0x96
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0xFF);
    LoadData(0x00, { 0x96 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("SUB A, A", "SUB-A,A") {  // 0x97
    // This one is refreshingly simple!
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0x97 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SUB A, imm8 (NC, NH, NZ)", "SUB-A,imm8-NC-NH-NZ") {  // 0xD6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    LoadData(0x00, { 0xD6, 0x03 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SUB A, imm8 (NC, H, NZ)", "SUB-A,imm8-NC-H-NZ") {  // 0xD6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    LoadData(0x00, { 0xD6, 0x02 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SUB A, imm8 (C, NH, NZ)", "SUB-A,imm8-C-NH-NZ") {  // 0xD6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    LoadData(0x00, { 0xD6, 0x10 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SUB A, imm8 (C, H, NZ)", "SUB-A,imm8-C-H-NZ") {  // 0xD6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0xD6, 0x02 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SUB A, imm8 (NC, NH, Z)", "SUB-A,imm8-NC-NH-Z") {  // 0xD6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0xD6, 0xFF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("SBC A, B (NC) (NC, NH, NZ)", "SBC-A,B-NC-NC-NH-NZ") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x03);
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, B (NC) (NC, H, NZ)", "SBC-A,B-NC-NC-H-NZ") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, B (NC) (C, NH, NZ)", "SBC-A,B-NC-C-NH-NZ") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x10);
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, B (NC) (C, H, NZ)", "SBC-A,B-NC-C-H-NZ") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, B (NC) (NC, NH, Z)", "SBC-A,B-NC-NC-NH-Z") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0xFF);
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, B (C) (NC, NH, NZ)", "SBC-A,B-C-NC-NH-NZ") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, B (C) (NC, H, NZ)", "SBC-A,B-C-NC-H-NZ") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, B (C) (C, NH, NZ)", "SBC-A,B-C-C-NH-NZ") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, B (C) (C, H, NZ)", "SBC-A,B-C-C-H-NZ") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, B (C) (NC, NH, Z)", "SBC-A,B-C-NC-NH-Z") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0xFE);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, B (C) (C, NH, NZ) (OF)", "SBC-A,B-C-C-NH-NZ-OF") {  // 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x98 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, C (NC) (NC, NH, NZ)", "SBC-A,C-NC-NC-NH-NZ") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x03);
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, C (NC) (NC, H, NZ)", "SBC-A,C-NC-NC-H-NZ") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, C (NC) (C, NH, NZ)", "SBC-A,C-NC-C-NH-NZ") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x10);
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, C (NC) (C, H, NZ)", "SBC-A,C-NC-C-H-NZ") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, C (NC) (NC, NH, Z)", "SBC-A,C-NC-NC-NH-Z") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0xFF);
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, C (C) (NC, NH, NZ)", "SBC-A,C-C-NC-NH-NZ") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, C (C) (NC, H, NZ)", "SBC-A,C-C-NC-H-NZ") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, C (C) (C, NH, NZ)", "SBC-A,C-C-C-NH-NZ") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, C (C) (C, H, NZ)", "SBC-A,C-C-C-H-NZ") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, C (C) (NC, NH, Z)", "SBC-A,C-C-NC-NH-Z") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0xFE);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, C (C) (C, NH, NZ) (OF)", "SBC-A,C-C-C-NH-NZ-OF") {  // 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x99 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, D (NC) (NC, NH, NZ)", "SBC-A,D-NC-NC-NH-NZ") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x03);
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, D (NC) (NC, H, NZ)", "SBC-A,D-NC-NC-H-NZ") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, D (NC) (C, NH, NZ)", "SBC-A,D-NC-C-NH-NZ") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x10);
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, D (NC) (C, H, NZ)", "SBC-A,D-NC-C-H-NZ") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, D (NC) (NC, NH, Z)", "SBC-A,D-NC-NC-NH-Z") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0xFF);
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, D (C) (NC, NH, NZ)", "SBC-A,D-C-NC-NH-NZ") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, D (C) (NC, H, NZ)", "SBC-A,D-C-NC-H-NZ") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, D (C) (C, NH, NZ)", "SBC-A,D-C-C-NH-NZ") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, D (C) (C, H, NZ)", "SBC-A,D-C-C-H-NZ") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, D (C) (NC, NH, Z)", "SBC-A,D-C-NC-NH-Z") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0xFE);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, D (C) (C, NH, NZ) (OF)", "SBC-A,D-C-C-NH-NZ-OF") {  // 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, E (NC) (NC, NH, NZ)", "SBC-A,E-NC-NC-NH-NZ") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x03);
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, E (NC) (NC, H, NZ)", "SBC-A,E-NC-NC-H-NZ") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, E (NC) (C, NH, NZ)", "SBC-A,E-NC-C-NH-NZ") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x10);
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, E (NC) (C, H, NZ)", "SBC-A,E-NC-C-H-NZ") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, E (NC) (NC, NH, Z)", "SBC-A,E-NC-NC-NH-Z") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0xFF);
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, E (C) (NC, NH, NZ)", "SBC-A,E-C-NC-NH-NZ") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, E (C) (NC, H, NZ)", "SBC-A,E-C-NC-H-NZ") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, E (C) (C, NH, NZ)", "SBC-A,E-C-C-NH-NZ") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, E (C) (C, H, NZ)", "SBC-A,E-C-C-H-NZ") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, E (C) (NC, NH, Z)", "SBC-A,E-C-NC-NH-Z") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0xFE);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, E (C) (C, NH, NZ) (OF)", "SBC-A,E-C-C-NH-NZ-OF") {  // 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, H (NC) (NC, NH, NZ)", "SBC-A,H-NC-NC-NH-NZ") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x03);
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, H (NC) (NC, H, NZ)", "SBC-A,H-NC-NC-H-NZ") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, H (NC) (C, NH, NZ)", "SBC-A,H-NC-C-NH-NZ") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x10);
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, H (NC) (C, H, NZ)", "SBC-A,H-NC-C-H-NZ") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, H (NC) (NC, NH, Z)", "SBC-A,H-NC-NC-NH-Z") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0xFF);
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, H (C) (NC, NH, NZ)", "SBC-A,H-C-NC-NH-NZ") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, H (C) (NC, H, NZ)", "SBC-A,H-C-NC-H-NZ") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, H (C) (C, NH, NZ)", "SBC-A,H-C-C-NH-NZ") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, H (C) (C, H, NZ)", "SBC-A,H-C-C-H-NZ") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, H (C) (NC, NH, Z)", "SBC-A,H-C-NC-NH-Z") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0xFE);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, H (C) (C, NH, NZ) (OF)", "SBC-A,H-C-C-NH-NZ-OF") {  // 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, L (NC) (NC, NH, NZ)", "SBC-A,L-NC-NC-NH-NZ") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x03);
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, L (NC) (NC, H, NZ)", "SBC-A,L-NC-NC-H-NZ") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, L (NC) (C, NH, NZ)", "SBC-A,L-NC-C-NH-NZ") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x10);
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, L (NC) (C, H, NZ)", "SBC-A,L-NC-C-H-NZ") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, L (NC) (NC, NH, Z)", "SBC-A,L-NC-NC-NH-Z") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0xFF);
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, L (C) (NC, NH, NZ)", "SBC-A,L-C-NC-NH-NZ") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, L (C) (NC, H, NZ)", "SBC-A,L-C-NC-H-NZ") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, L (C) (C, NH, NZ)", "SBC-A,L-C-C-NH-NZ") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, L (C) (C, H, NZ)", "SBC-A,L-C-C-H-NZ") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, L (C) (NC, NH, Z)", "SBC-A,L-C-NC-NH-Z") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0xFE);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, L (C) (C, NH, NZ) (OF)", "SBC-A,L-C-C-NH-NZ-OF") {  // 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, <HL> (NC) (NC, NH, NZ)", "SBC-A,HL-NC-NC-NH-NZ") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x03);
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (NC) (NC, H, NZ)", "SBC-A,HL-NC-NC-H-NZ") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (NC) (C, NH, NZ)", "SBC-A,HL-NC-C-NH-NZ") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x10);
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (NC) (C, H, NZ)", "SBC-A,HL-NC-C-H-NZ") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (NC) (NC, NH, Z)", "SBC-A,HL-NC-NC-NH-Z") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0xFF);
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (C) (NC, NH, NZ)", "SBC-A,HL-C-NC-NH-NZ") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (C) (NC, H, NZ)", "SBC-A,HL-C-NC-H-NZ") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (C) (C, NH, NZ)", "SBC-A,HL-C-C-NH-NZ") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x0F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (C) (C, H, NZ)", "SBC-A,HL-C-C-H-NZ") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (C) (NC, NH, Z)", "SBC-A,HL-C-NC-NH-Z") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0xFE);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("SBC A, <HL> (C) (C, NH, NZ) (OF)", "SBC-A,HL-C-C-NH-NZ-OF") {  // 0x9E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, A (NC)", "SBC-A,A-NC") {  // 0x9F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0x9F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("SBC A, A (C)", "SBC-A,A-C") {  // 0x9F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x9F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("SBC A, imm8 (NC) (NC, NH, NZ)", "SBC-A,imm8-NC-NC-NH-NZ") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    LoadData(0x00, { 0xDE, 0x03 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (NC) (NC, H, NZ)", "SBC-A,imm8-NC-NC-H-NZ") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    LoadData(0x00, { 0xDE, 0x02 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (NC) (C, NH, NZ)", "SBC-A,imm8-NC-C-NH-NZ") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    LoadData(0x00, { 0xDE, 0x10 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (NC) (C, H, NZ)", "SBC-A,imm8-NC-C-H-NZ") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0xDE, 0x02 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (NC) (NC, NH, Z)", "SBC-A,imm8-NC-NC-NH-Z") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0xDE, 0xFF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (C) (NC, NH, NZ)", "SBC-A,imm8-C-NC-NH-NZ") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xDE, 0x02 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (C) (NC, H, NZ)", "SBC-A,imm8-C-NC-H-NZ") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xDE, 0x01 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0E);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (C) (C, NH, NZ)", "SBC-A,imm8-C-C-NH-NZ") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xDE, 0x0F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xF8);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (C) (C, H, NZ)", "SBC-A,imm8-C-C-H-NZ") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xDE, 0x01 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (C) (NC, NH, Z)", "SBC-A,imm8-C-NC-NH-Z") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xDE, 0xFE });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("SBC A, imm8 (C) (C, NH, NZ) (OF)", "SBC-A,imm8-C-C-NH-NZ-OF") {  // 0xDE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xDE, 0xFF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("AND A, B (NZ)", "AND-A,B-NZ") {  // 0xA0
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10100011);
    LoadData(0x00, { 0xA0 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10100011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("AND A, B (Z)", "AND-A,B-Z") {  // 0xA0
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01011100);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10100011);
    LoadData(0x00, { 0xA0 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("AND A, C (NZ)", "AND-A,C-NZ") {  // 0xA1
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10100011);
    LoadData(0x00, { 0xA1 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10100011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("AND A, C (Z)", "AND-A,C-Z") {  // 0xA1
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01011100);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10100011);
    LoadData(0x00, { 0xA1 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("AND A, D (NZ)", "AND-A,D-NZ") {  // 0xA2
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10100011);
    LoadData(0x00, { 0xA2 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10100011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("AND A, D (Z)", "AND-A,D-Z") {  // 0xA2
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01011100);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10100011);
    LoadData(0x00, { 0xA2 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("AND A, E (NZ)", "AND-A,E-NZ") {  // 0xA3
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10100011);
    LoadData(0x00, { 0xA3 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10100011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("AND A, E (Z)", "AND-A,E-Z") {  // 0xA3
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01011100);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10100011);
    LoadData(0x00, { 0xA3 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("AND A, H (NZ)", "AND-A,H-NZ") {  // 0xA4
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10100011);
    LoadData(0x00, { 0xA4 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10100011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("AND A, H (Z)", "AND-A,H-Z") {  // 0xA4
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01011100);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10100011);
    LoadData(0x00, { 0xA4 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("AND A, L (NZ)", "AND-A,L-NZ") {  // 0xA5
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10100011);
    LoadData(0x00, { 0xA5 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10100011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("AND A, L (Z)", "AND-A,L-Z") {  // 0xA5
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01011100);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10100011);
    LoadData(0x00, { 0xA5 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("AND A, <HL> (NZ)", "AND-A,HL-NZ") {  // 0xA6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0b10100011);
    LoadData(0x00, { 0xA6 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10100011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("AND A, <HL> (Z)", "AND-A,HL-Z") {  // 0xA6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01011100);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0b10100011);
    LoadData(0x00, { 0xA6 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("AND A, A (NZ)", "AND-A,A-NZ") {  // 0xA7
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01011100);
    LoadData(0x00, { 0xA7 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01011100);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("AND A, A (Z)", "AND-A,A-Z") {  // 0xA7
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0);
    LoadData(0x00, { 0xA7 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("AND A, imm8 (NZ)", "AND-A,imm8-NZ") {  // 0xE6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    LoadData(0x00, { 0xE6, 0b10100011 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10100011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("AND A, imm8 (Z)", "AND-A,imm8-Z") {  // 0xE6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01011100);
    LoadData(0x00, { 0xE6, 0b10100011 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("XOR A, B (NZ)", "XOR-A,B-NZ") {  // 0xA8
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10100011);
    LoadData(0x00, { 0xA8 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("XOR A, B (Z)", "XOR-A,B-Z") {  // 0xA8
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11110000);
    LoadData(0x00, { 0xA8 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("XOR A, C (NZ)", "XOR-A,C-NZ") {  // 0xA9
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10100011);
    LoadData(0x00, { 0xA9 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("XOR A, C (Z)", "XOR-A,C-Z") {  // 0xA9
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11110000);
    LoadData(0x00, { 0xA9 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("XOR A, D (NZ)", "XOR-A,D-NZ") {  // 0xAA
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10100011);
    LoadData(0x00, { 0xAA });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("XOR A, D (Z)", "XOR-A,D-Z") {  // 0xAA
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11110000);
    LoadData(0x00, { 0xAA });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("XOR A, E (NZ)", "XOR-A,E-NZ") {  // 0xAB
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10100011);
    LoadData(0x00, { 0xAB });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("XOR A, E (Z)", "XOR-A,E-Z") {  // 0xAB
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11110000);
    LoadData(0x00, { 0xAB });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("XOR A, H (NZ)", "XOR-A,H-NZ") {  // 0xAC
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10100011);
    LoadData(0x00, { 0xAC });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("XOR A, H (Z)", "XOR-A,H-Z") {  // 0xAC
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11110000);
    LoadData(0x00, { 0xAC });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("XOR A, L (NZ)", "XOR-A,L-NZ") {  // 0xAD
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10100011);
    LoadData(0x00, { 0xAD });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("XOR A, L (Z)", "XOR-A,L-Z") {  // 0xAD
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11110000);
    LoadData(0x00, { 0xAD });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("XOR A, <HL> (NZ)", "XOR-A,HL-NZ") {  // 0xAE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0b10100011);
    LoadData(0x00, { 0xAE });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("XOR A, <HL> (Z)", "XOR-A,HL-Z") {  // 0xAE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0b11110000);
    LoadData(0x00, { 0xAE });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("XOR A, A", "XOR-A,A") {  // 0xAF
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    LoadData(0x00, { 0xAF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("XOR A, imm8 (NZ)", "XOR-A,imm8-NZ") {  // 0xEE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    LoadData(0x00, { 0xEE, 0b10100011 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("XOR A, imm8 (Z)", "XOR-A,imm8-Z") {  // 0xEE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    LoadData(0x00, { 0xEE, 0b11110000 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("OR A, B (NZ)", "OR-A,B-NZ") {  // 0xB0
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10100011);
    LoadData(0x00, { 0xB0 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11110011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("OR A, B (Z)", "OR-A,B-Z") {  // 0xB0
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0);
    LoadData(0x00, { 0xB0 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("OR A, C (NZ)", "OR-A,C-NZ") {  // 0xB1
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10100011);
    LoadData(0x00, { 0xB1 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11110011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("OR A, C (Z)", "OR-A,C-Z") {  // 0xB1
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0);
    LoadData(0x00, { 0xB1 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("OR A, D (NZ)", "OR-A,D-NZ") {  // 0xB2
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10100011);
    LoadData(0x00, { 0xB2 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11110011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("OR A, D (Z)", "OR-A,D-Z") {  // 0xB2
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0);
    LoadData(0x00, { 0xB2 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("OR A, E (NZ)", "OR-A,E-NZ") {  // 0xB3
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10100011);
    LoadData(0x00, { 0xB3 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11110011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("OR A, E (Z)", "OR-A,E-Z") {  // 0xB3
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0);
    LoadData(0x00, { 0xB3 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("OR A, H (NZ)", "OR-A,H-NZ") {  // 0xB4
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10100011);
    LoadData(0x00, { 0xB4 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11110011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("OR A, H (Z)", "OR-A,H-Z") {  // 0xB4
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0);
    LoadData(0x00, { 0xB4 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("OR A, L (NZ)", "OR-A,L-NZ") {  // 0xB5
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10100011);
    LoadData(0x00, { 0xB5 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11110011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("OR A, L (Z)", "OR-A,L-Z") {  // 0xB5
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0);
    LoadData(0x00, { 0xB5 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("OR A, <HL> (NZ)", "OR-A,HL-NZ") {  // 0xB6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0b10100011);
    LoadData(0x00, { 0xB6 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11110011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("OR A, <HL> (Z)", "OR-A,HL-Z") {  // 0xB6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0);
    LoadData(0x00, { 0xB6 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("OR A, imm8 (NZ)", "OR-A,imm8-NZ") {  // 0xF6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110000);
    LoadData(0x00, { 0xF6, 0b10100011 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11110011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("OR A, imm8 (Z)", "OR-A,imm8-Z") {  // 0xF6
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0);
    LoadData(0x00, { 0xF6, 0 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("CP A, B (NC, NH, NZ)", "CP-A,B-NC-NH-NZ") {  // 0xB8
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x03);
    LoadData(0x00, { 0xB8 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, B (NC, H, NZ)", "CP-A,B-NC-H-NZ") {  // 0xB8
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0xB8 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, B (C, NH, NZ)", "CP-A,B-C-NH-NZ") {  // 0xB8
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x10);
    LoadData(0x00, { 0xB8 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x08);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, B (C, H, NZ)", "CP-A,B-C-H-NZ") {  // 0xB8
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0xB8 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, B (NC, NH, Z)", "CP-A,B-NC-NH-Z") {  // 0xB8
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0xFF);
    LoadData(0x00, { 0xB8 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("CP A, C (NC, NH, NZ)", "CP-A,C-NC-NH-NZ") {  // 0xB9
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x03);
    LoadData(0x00, { 0xB9 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, C (NC, H, NZ)", "CP-A,C-NC-H-NZ") {  // 0xB9
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0xB9 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, C (C, NH, NZ)", "CP-A,C-C-NH-NZ") {  // 0xB9
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x10);
    LoadData(0x00, { 0xB9 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x08);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, C (C, H, NZ)", "CP-A,C-C-H-NZ") {  // 0xB9
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0xB9 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, C (NC, NH, Z)", "CP-A,C-NC-NH-Z") {  // 0xB9
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0xFF);
    LoadData(0x00, { 0xB9 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("CP A, D (NC, NH, NZ)", "CP-A,D-NC-NH-NZ") {  // 0xBA
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x03);
    LoadData(0x00, { 0xBA });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, D (NC, H, NZ)", "CP-A,D-NC-H-NZ") {  // 0xBA
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0xBA });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, D (C, NH, NZ)", "CP-A,D-C-NH-NZ") {  // 0xBA
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x10);
    LoadData(0x00, { 0xBA });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x08);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, D (C, H, NZ)", "CP-A,D-C-H-NZ") {  // 0xBA
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0xBA });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, D (NC, NH, Z)", "CP-A,D-NC-NH-Z") {  // 0xBA
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0xFF);
    LoadData(0x00, { 0xBA });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("CP A, E (NC, NH, NZ)", "CP-A,E-NC-NH-NZ") {  // 0xBB
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x03);
    LoadData(0x00, { 0xBB });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, E (NC, H, NZ)", "CP-A,E-NC-H-NZ") {  // 0xBB
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0xBB });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, E (C, NH, NZ)", "CP-A,E-C-NH-NZ") {  // 0xBB
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x10);
    LoadData(0x00, { 0xBB });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x08);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, E (C, H, NZ)", "CP-A,E-C-H-NZ") {  // 0xBB
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0xBB });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, E (NC, NH, Z)", "CP-A,E-NC-NH-Z") {  // 0xBB
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0xFF);
    LoadData(0x00, { 0xBB });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("CP A, H (NC, NH, NZ)", "CP-A,H-NC-NH-NZ") {  // 0xBC
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x03);
    LoadData(0x00, { 0xBC });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, H (NC, H, NZ)", "CP-A,H-NC-H-NZ") {  // 0xBC
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0xBC });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, H (C, NH, NZ)", "CP-A,H-C-NH-NZ") {  // 0xBC
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x10);
    LoadData(0x00, { 0xBC });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x08);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, H (C, H, NZ)", "CP-A,H-C-H-NZ") {  // 0xBC
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0xBC });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, H (NC, NH, Z)", "CP-A,H-NC-NH-Z") {  // 0xBC
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0xFF);
    LoadData(0x00, { 0xBC });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("CP A, L (NC, NH, NZ)", "CP-A,L-NC-NH-NZ") {  // 0xBD
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x03);
    LoadData(0x00, { 0xBD });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, L (NC, H, NZ)", "CP-A,L-NC-H-NZ") {  // 0xBD
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0xBD });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, L (C, NH, NZ)", "CP-A,L-C-NH-NZ") {  // 0xBD
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x10);
    LoadData(0x00, { 0xBD });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x08);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, L (C, H, NZ)", "CP-A,L-C-H-NZ") {  // 0xBD
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0xBD });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 1);
}

TEST("CP A, L (NC, NH, Z)", "CP-A,L-NC-NH-Z") {  // 0xBD
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0xFF);
    LoadData(0x00, { 0xBD });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("CP A, <HL> (NC, NH, NZ)", "CP-A,HL-NC-NH-NZ") {  // 0xBE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x03);
    LoadData(0x00, { 0xBE });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("CP A, <HL> (NC, H, NZ)", "CP-A,HL-NC-H-NZ") {  // 0xBE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0xBE });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("CP A, <HL> (C, NH, NZ)", "CP-A,HL-C-NH-NZ") {  // 0xBE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x10);
    LoadData(0x00, { 0xBE });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x08);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("CP A, <HL> (C, H, NZ)", "CP-A,HL-C-H-NZ") {  // 0xBE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0xBE });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("CP A, <HL> (NC, NH, Z)", "CP-A,HL-NC-NH-Z") {  // 0xBE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0xFF);
    LoadData(0x00, { 0xBE });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}

TEST("CP A, A", "CP-A,A") {  // 0xBF
    // It's like that subtraction one! Wahoo!
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0xBF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 1);
}

TEST("CP A, imm8 (NC, NH, NZ)", "CP-A,imm8-NC-NH-NZ") {  // 0xFE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x04);
    LoadData(0x00, { 0xFE, 0x03 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("CP A, imm8 (NC, H, NZ)", "CP-A,imm8-NC-H-NZ") {  // 0xFE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x10);
    LoadData(0x00, { 0xFE, 0x02 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("CP A, imm8 (C, NH, NZ)", "CP-A,imm8-C-NH-NZ") {  // 0xFE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    LoadData(0x00, { 0xFE, 0x10 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x08);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("CP A, imm8 (C, H, NZ)", "CP-A,imm8-C-H-NZ") {  // 0xFE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0xFE, 0x02 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK_CARRY_SET;
    CHECK_HALF_SET;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_CLEAR;
    CHECK(cycles == 2);
}

TEST("CP A, imm8 (NC, NH, Z)", "CP-A,imm8-NC-NH-Z") {  // 0xFE
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0xFE, 0xFF });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_SUBTRACT_SET;
    CHECK_ZERO_SET;
    CHECK(cycles == 2);
}
