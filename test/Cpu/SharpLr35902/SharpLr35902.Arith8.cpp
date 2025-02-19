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
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC B (H, NZ)", "INC-B-H-NZ") {  // 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0F);
    LoadData(0x00, { 0x04 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x10);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC B (H, Z)", "INC-B-H-Z") {  // 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0xFF);
    LoadData(0x00, { 0x04 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC B (NH, NZ)", "DEC-B-NH-NZ") {  // 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x05 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("DEC B (NH, Z)", "DEC-B-NH-Z") {  // 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    LoadData(0x00, { 0x05 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC B (H, NZ)", "DEC-B-H-NZ") {  // 0x05
    LoadData(0x00, { 0x05 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0xFF);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC C (NH, NZ)", "INC-C-NH-NZ") {  // 0x0C
    LoadData(0x00, { 0x0C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC C (H, NZ)", "INC-C-H-NZ") {  // 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0F);
    LoadData(0x00, { 0x0C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x10);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC C (H, Z)", "INC-C-H-Z") {  // 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0xFF);
    LoadData(0x00, { 0x0C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC C (NH, NZ)", "DEC-C-NH-NZ") {  // 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x0D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("DEC C (NH, Z)", "DEC-C-NH-Z") {  // 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    LoadData(0x00, { 0x0D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC C (H, NZ)", "DEC-C-H-NZ") {  // 0x0D
    LoadData(0x00, { 0x0D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0xFF);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC D (NH, NZ)", "INC-D-NH-NZ") {  // 0x14
    LoadData(0x00, { 0x14 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC D (H, NZ)", "INC-D-H-NZ") {  // 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0F);
    LoadData(0x00, { 0x14 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x10);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC D (H, Z)", "INC-D-H-Z") {  // 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0xFF);
    LoadData(0x00, { 0x14 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC D (NH, NZ)", "DEC-D-NH-NZ") {  // 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x15 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("DEC D (NH, Z)", "DEC-D-NH-Z") {  // 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    LoadData(0x00, { 0x15 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC D (H, NZ)", "DEC-D-H-NZ") {  // 0x15
    LoadData(0x00, { 0x15 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0xFF);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC E (NH, NZ)", "INC-E-NH-NZ") {  // 0x1C
    LoadData(0x00, { 0x1C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC E (H, NZ)", "INC-E-H-NZ") {  // 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0F);
    LoadData(0x00, { 0x1C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x10);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC E (H, Z)", "INC-E-H-Z") {  // 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0xFF);
    LoadData(0x00, { 0x1C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC E (NH, NZ)", "DEC-E-NH-NZ") {  // 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x1D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("DEC E (NH, Z)", "DEC-E-NH-Z") {  // 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    LoadData(0x00, { 0x1D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC E (H, NZ)", "DEC-E-H-NZ") {  // 0x1D
    LoadData(0x00, { 0x1D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0xFF);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC H (NH, NZ)", "INC-H-NH-NZ") {  // 0x24
    LoadData(0x00, { 0x24 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC H (H, NZ)", "INC-H-H-NZ") {  // 0x24
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0F);
    LoadData(0x00, { 0x24 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x10);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC H (H, Z)", "INC-H-H-Z") {  // 0x24
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0xFF);
    LoadData(0x00, { 0x24 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC H (NH, NZ)", "DEC-H-NH-NZ") {  // 0x25
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x25 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("DEC H (NH, Z)", "DEC-H-NH-Z") {  // 0x25
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    LoadData(0x00, { 0x25 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC H (H, NZ)", "DEC-H-H-NZ") {  // 0x25
    LoadData(0x00, { 0x25 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0xFF);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC L (NH, NZ)", "INC-L-NH-NZ") {  // 0x2C
    LoadData(0x00, { 0x2C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC L (H, NZ)", "INC-L-H-NZ") {  // 0x2C
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0F);
    LoadData(0x00, { 0x2C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x10);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC L (H, Z)", "INC-L-H-Z") {  // 0x2C
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0xFF);
    LoadData(0x00, { 0x2C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC L (NH, NZ)", "DEC-L-NH-NZ") {  // 0x2D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x2D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("DEC L (NH, Z)", "DEC-L-NH-Z") {  // 0x2D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    LoadData(0x00, { 0x2D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC L (H, NZ)", "DEC-L-H-NZ") {  // 0x2D
    LoadData(0x00, { 0x2D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0xFF);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC <HL> (NH, NZ)", "INC-HL-NH-NZ") {  // 0x34
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x34 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 3);
}

TEST("INC <HL> (H, NZ)", "INC-HL-H-NZ") {  // 0x34
    memory->SetByte(0x200, 0x0F);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x34 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x10);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 3);
}

TEST("INC <HL> (H, Z)", "INC-HL-H-Z") {  // 0x34
    memory->SetByte(0x200, 0xFF);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x34 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 3);
}

TEST("DEC <HL> (NH, NZ)", "DEC-HL-NH-NZ") {  // 0x35
    memory->SetByte(0x200, 0x02);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x35 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 3);
}

TEST("DEC <HL> (NH, Z)", "DEC-HL-NH-Z") {  // 0x35
    memory->SetByte(0x200, 0x01);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x35 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 3);
}

TEST("DEC <HL> (H, NZ)", "DEC-HL-H-NZ") {  // 0x35
    memory->SetByte(0x200, 0x00);
    cpu->SetHl(0x200);
    LoadData(0x00, { 0x35 });

    const auto cycles = cpu->Cycle();
    CHECK(memory->GetByte(0x200) == 0xFF);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 3);
}

TEST("INC A (NH, NZ)", "INC-A-NH-NZ") {  // 0x3C
    LoadData(0x00, { 0x3C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC A (H, NZ)", "INC-A-H-NZ") {  // 0x3C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x0F);
    LoadData(0x00, { 0x3C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("INC A (H, Z)", "INC-A-H-Z") {  // 0x3C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    LoadData(0x00, { 0x3C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC A (NH, NZ)", "DEC-A-NH-NZ") {  // 0x3D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x02);
    LoadData(0x00, { 0x3D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("DEC A (NH, Z)", "DEC-A-NH-Z") {  // 0x3D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    LoadData(0x00, { 0x3D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("DEC A (H, NZ)", "DEC-A-H-NZ") {  // 0x3D
    LoadData(0x00, { 0x3D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0xFF);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == true);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, B (NC, NH, NZ)", "ADD-A,B-NC-NH-NZ") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0E);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, B (C, NH, NZ)", "ADD-A,B-C-NH-NZ") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x20);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, B (NC, H, NZ)", "ADD-A,B-NC-H-NZ") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0F);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, B (C, H, NZ)", "ADD-A,B-C-H-NZ") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, B (C, H, Z)", "ADD-A,B-C-H-Z") {  // 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    LoadData(0x00, { 0x80 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADD A, C (NC, NH, NZ)", "ADD-A,C-NC-NH-NZ") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0E);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, C (C, NH, NZ)", "ADD-A,C-C-NH-NZ") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x20);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, C (NC, H, NZ)", "ADD-A,C-NC-H-NZ") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0F);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, C (C, H, NZ)", "ADD-A,C-C-H-NZ") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, C (C, H, Z)", "ADD-A,C-C-H-Z") {  // 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    LoadData(0x00, { 0x81 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADD A, D (NC, NH, NZ)", "ADD-A,D-NC-NH-NZ") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0E);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, D (C, NH, NZ)", "ADD-A,D-C-NH-NZ") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x20);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, D (NC, H, NZ)", "ADD-A,D-NC-H-NZ") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0F);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, D (C, H, NZ)", "ADD-A,D-C-H-NZ") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, D (C, H, Z)", "ADD-A,D-C-H-Z") {  // 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    LoadData(0x00, { 0x82 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADD A, E (NC, NH, NZ)", "ADD-A,E-NC-NH-NZ") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0E);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, E (C, NH, NZ)", "ADD-A,E-C-NH-NZ") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x20);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, E (NC, H, NZ)", "ADD-A,E-NC-H-NZ") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0F);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, E (C, H, NZ)", "ADD-A,E-C-H-NZ") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, E (C, H, Z)", "ADD-A,E-C-H-Z") {  // 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    LoadData(0x00, { 0x83 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADD A, H (NC, NH, NZ)", "ADD-A,H-NC-NH-NZ") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0E);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, H (C, NH, NZ)", "ADD-A,H-C-NH-NZ") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x20);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, H (NC, H, NZ)", "ADD-A,H-NC-H-NZ") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0F);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, H (C, H, NZ)", "ADD-A,H-C-H-NZ") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, H (C, H, Z)", "ADD-A,H-C-H-Z") {  // 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    LoadData(0x00, { 0x84 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADD A, L (NC, NH, NZ)", "ADD-A,L-NC-NH-NZ") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0E);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, L (C, NH, NZ)", "ADD-A,L-C-NH-NZ") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x20);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, L (NC, H, NZ)", "ADD-A,L-NC-H-NZ") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0F);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, L (C, H, NZ)", "ADD-A,L-C-H-NZ") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, L (C, H, Z)", "ADD-A,L-C-H-Z") {  // 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    LoadData(0x00, { 0x85 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADD A, <HL> (NC, NH, NZ)", "ADD-A,HL-NC-NH-NZ") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x0E);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 2);
}

TEST("ADD A, <HL> (C, NH, NZ)", "ADD-A,HL-C-NH-NZ") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x20);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 2);
}

TEST("ADD A, <HL> (NC, H, NZ)", "ADD-A,HL-NC-H-NZ") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x0F);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 2);
}

TEST("ADD A, <HL> (C, H, NZ)", "ADD-A,HL-C-H-NZ") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 2);
}

TEST("ADD A, <HL> (C, H, Z)", "ADD-A,HL-C-H-Z") {  // 0x86
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x01);
    LoadData(0x00, { 0x86 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 2);
}

TEST("ADD A, A (NC, NH, NZ)", "ADD-A,A-NC-NH-NZ") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x02);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, A (C, NH, NZ)", "ADD-A,A-C-NH-NZ") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x90);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x20);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, A (NC, H, NZ)", "ADD-A,A-NC-H-NZ") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, A (C, H, NZ)", "ADD-A,A-C-H-NZ") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x99);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x32);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADD A, A (C, NH, Z)", "ADD-A,A-C-NH-Z") {  // 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x80);
    LoadData(0x00, { 0x87 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, B (NC, NH, NZ)", "ADC-A,B-NC-NH-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0E);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, B (C, NH, NZ)", "ADC-A,B-C-NH-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x20);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, B (NC, H, NZ)", "ADC-A,B-NC-H-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0F);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, B (C, H, NZ)", "ADC-A,B-C-H-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x02);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, B (C, H, Z)", "ADC-A,B-C-H-Z") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (NC, NH, NZ)", "ADC-A,B-C-NC-NH-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (C, NH, NZ)", "ADC-A,B-C-C-NH-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (NC, H, NZ)", "ADC-A,B-C-NC-H-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (C, H, NZ)", "ADC-A,B-C-C-H-NZ") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, B (C) (C, H, Z)", "ADC-A,B-C-C-H-Z") {  // 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x88 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (NC, NH, NZ)", "ADC-A,C-NC-NC-NH-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0E);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (C, NH, NZ)", "ADC-A,C-NC-C-NH-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x20);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (NC, H, NZ)", "ADC-A,C-NC-NC-H-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0F);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (C, H, NZ)", "ADC-A,C-NC-C-H-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x02);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, C (NC) (C, H, Z)", "ADC-A,C-NC-C-H-Z") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (NC, NH, NZ)", "ADC-A,C-C-NC-NH-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (C, NH, NZ)", "ADC-A,C-C-C-NH-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (NC, H, NZ)", "ADC-A,C-C-NC-H-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (C, H, NZ)", "ADC-A,C-C-C-H-NZ") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, C (C) (C, H, Z)", "ADC-A,C-C-C-H-Z") {  // 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x89 });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (NC, NH, NZ)", "ADC-A,D-NC-NC-NH-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0E);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (C, NH, NZ)", "ADC-A,D-NC-C-NH-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x20);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (NC, H, NZ)", "ADC-A,D-NC-NC-H-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0F);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (C, H, NZ)", "ADC-A,D-NC-C-H-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x02);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, D (NC) (C, H, Z)", "ADC-A,D-NC-C-H-Z") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (NC, NH, NZ)", "ADC-A,D-C-NC-NH-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (C, NH, NZ)", "ADC-A,D-C-C-NH-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (NC, H, NZ)", "ADC-A,D-C-NC-H-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (C, H, NZ)", "ADC-A,D-C-C-H-NZ") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, D (C) (C, H, Z)", "ADC-A,D-C-C-H-Z") {  // 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8A });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (NC, NH, NZ)", "ADC-A,E-NC-NC-NH-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0E);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (C, NH, NZ)", "ADC-A,E-NC-C-NH-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x20);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (NC, H, NZ)", "ADC-A,E-NC-NC-H-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0F);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (C, H, NZ)", "ADC-A,E-NC-C-H-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x02);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, E (NC) (C, H, Z)", "ADC-A,E-NC-C-H-Z") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (NC, NH, NZ)", "ADC-A,E-C-NC-NH-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (C, NH, NZ)", "ADC-A,E-C-C-NH-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (NC, H, NZ)", "ADC-A,E-C-NC-H-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (C, H, NZ)", "ADC-A,E-C-C-H-NZ") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, E (C) (C, H, Z)", "ADC-A,E-C-C-H-Z") {  // 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8B });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (NC, NH, NZ)", "ADC-A,H-NC-NC-NH-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0E);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (C, NH, NZ)", "ADC-A,H-NC-C-NH-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x20);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (NC, H, NZ)", "ADC-A,H-NC-NC-H-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0F);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (C, H, NZ)", "ADC-A,H-NC-C-H-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x02);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, H (NC) (C, H, Z)", "ADC-A,H-NC-C-H-Z") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (NC, NH, NZ)", "ADC-A,H-C-NC-NH-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (C, NH, NZ)", "ADC-A,H-C-C-NH-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (NC, H, NZ)", "ADC-A,H-C-NC-H-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (C, H, NZ)", "ADC-A,H-C-C-H-NZ") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, H (C) (C, H, Z)", "ADC-A,H-C-C-H-Z") {  // 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8C });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (NC, NH, NZ)", "ADC-A,L-NC-NC-NH-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0E);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (C, NH, NZ)", "ADC-A,L-NC-C-NH-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x20);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (NC, H, NZ)", "ADC-A,L-NC-NC-H-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0F);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (C, H, NZ)", "ADC-A,L-NC-C-H-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x02);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, L (NC) (C, H, Z)", "ADC-A,L-NC-C-H-Z") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (NC, NH, NZ)", "ADC-A,L-C-NC-NH-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0D);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (C, NH, NZ)", "ADC-A,L-C-C-NH-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x1F);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (NC, H, NZ)", "ADC-A,L-C-NC-H-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x0E);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (C, H, NZ)", "ADC-A,L-C-C-H-NZ") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x01);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, L (C) (C, H, Z)", "ADC-A,L-C-C-H-Z") {  // 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0x00);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8D });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, <HL> (NC) (NC, NH, NZ)", "ADC-A,HL-NC-NC-NH-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x0E);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x0F);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (NC) (C, NH, NZ)", "ADC-A,HL-NC-C-NH-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xF0);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x20);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (NC) (NC, H, NZ)", "ADC-A,HL-NC-NC-H-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x01);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x0F);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (NC) (C, H, NZ)", "ADC-A,HL-NC-C-H-NZ") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x02);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x01);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 2);
}

TEST("ADC A, <HL> (NC) (C, H, Z)", "ADC-A,HL-NC-C-H-Z") {  // 0x8E
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xFF);
    cpu->SetHl(0x200);
    memory->SetByte(0x200, 0x01);
    LoadData(0x00, { 0x8E });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
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
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
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
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
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
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
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
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
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
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 2);
}

TEST("ADC A, A (NC) (NC, NH, NZ)", "ADC-A,A-NC-NC-NH-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x02);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x04);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, A (NC) (C, NH, NZ)", "ADC-A,A-NC-C-NH-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x90);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x20);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, A (NC) (NC, H, NZ)", "ADC-A,A-NC-NC-H-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x08);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x10);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, A (NC) (C, H, NZ)", "ADC-A,A-NC-C-H-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x99);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x32);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, A (NC) (C, NH, Z)", "ADC-A,A-NC-C-NH-Z") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x80);
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x00);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == true);
    CHECK(cycles == 1);
}

TEST("ADC A, A (C) (NC, NH, NZ)", "ADC-A,A-C-NC-NH-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x02);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x05);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, A (C) (NC, H, NZ)", "ADC-A,A-C-NC-H-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0xE);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x1D);
    CHECK(cpu->GetCarryFlag() == false);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, A (C) (C, NH, NZ)", "ADC-A,A-C-C-NH-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x81);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x03);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == false);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}

TEST("ADC A, A (C) (C, H, NZ)", "ADC-A,A-C-C-H-NZ") {  // 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0x88);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x8F });

    const auto cycles = cpu->Cycle();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0x11);
    CHECK(cpu->GetCarryFlag() == true);
    CHECK(cpu->GetHalfCarryFlag() == true);
    CHECK(cpu->GetSubtractFlag() == false);
    CHECK(cpu->GetZeroFlag() == false);
    CHECK(cycles == 1);
}
