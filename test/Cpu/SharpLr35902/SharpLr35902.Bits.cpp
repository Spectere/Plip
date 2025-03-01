/* SharpLr35902.Bits.cpp
 *
 * Tests the CPU core's ability to perform shifts, rotates, and bit manipulation.
 */

#include "catch2/catch_test_macros.hpp"

#include "SharpLr35902Common.h"

TEST("RLCA (NC) (initial NC)", "RLCA-NC-InitNC") {  // 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RLCA (NC) (initial C)", "RLCA-NC-InitC") {  // 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RLCA (C) (initial NC)", "RLCA-NC-InitNC") {  // 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RLCA (C) (initial C)", "RLCA-NC-InitC") {  // 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RRCA (NC) (initial NC)", "RRCA-NC-InitNC") {  // 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RRCA (NC) (initial C)", "RRCA-NC-InitC") {  // 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RRCA (C) (initial NC)", "RRCA-NC-InitNC") {  // 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RRCA (C) (initial C)", "RRCA-NC-InitC") {  // 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RLA (NC) (initial NC)", "RLA-NC-InitNC") {  // 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RLA (NC) (initial C)", "RLA-NC-InitC") {  // 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RLA (C) (initial NC)", "RLA-C-InitNC") {  // 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010100);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RLA (C) (initial C)", "RLA-C-InitC") {  // 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RRA (NC) (initial NC)", "RRA-NC-InitNC") {  // 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RRA (NC) (initial C)", "RRA-NC-InitC") {  // 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RRA (C) (initial NC)", "RRA-C-InitNC") {  // 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RRA (C) (initial C)", "RRA-C-InitC") {  // 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 1);
}

TEST("RLC B (NC, NZ) (initial NC)", "RLC-B-NC,NZ-InitNC") {  // 0xCB, 0x00
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    LoadData(0x00, { 0xCB, 0x00 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC B (NC, NZ) (initial C)", "RLC-B-NC,NZ-InitC") {  // 0xCB, 0x00
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x00 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC B (C, NZ) (initial NC)", "RLC-B-C,NZ-InitNC") {  // 0xCB, 0x00
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    LoadData(0x00, { 0xCB, 0x00 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC B (C, NZ) (initial C)", "RLC-B-C,NZ-InitC") {  // 0xCB, 0x00
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x00 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC B (NC, Z) (initial NC)", "RLC-B-NC,Z-InitNC") {  // 0xCB, 0x00
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0x00 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC B (NC, Z) (initial C)", "RLC-B-NC,Z-InitC") {  // 0xCB, 0x00
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x00 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC C (NC, NZ) (initial NC)", "RLC-C-NC,NZ-InitNC") {  // 0xCB, 0x01
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    LoadData(0x00, { 0xCB, 0x01 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC C (NC, NZ) (initial C)", "RLC-C-NC,NZ-InitC") {  // 0xCB, 0x01
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x01 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC C (C, NZ) (initial NC)", "RLC-C-C,NZ-InitNC") {  // 0xCB, 0x01
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    LoadData(0x00, { 0xCB, 0x01 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC C (C, NZ) (initial C)", "RLC-C-C,NZ-InitC") {  // 0xCB, 0x01
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x01 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC C (NC, Z) (initial NC)", "RLC-C-NC,Z-InitNC") {  // 0xCB, 0x01
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0x01 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC C (NC, Z) (initial C)", "RLC-C-NC,Z-InitC") {  // 0xCB, 0x01
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x01 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC D (NC, NZ) (initial NC)", "RLC-D-NC,NZ-InitNC") {  // 0xCB, 0x02
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    LoadData(0x00, { 0xCB, 0x02 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC D (NC, NZ) (initial C)", "RLC-D-NC,NZ-InitC") {  // 0xCB, 0x02
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x02 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC D (C, NZ) (initial NC)", "RLC-D-C,NZ-InitNC") {  // 0xCB, 0x02
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    LoadData(0x00, { 0xCB, 0x02 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC D (C, NZ) (initial C)", "RLC-D-C,NZ-InitC") {  // 0xCB, 0x02
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x02 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC D (NC, Z) (initial NC)", "RLC-D-NC,Z-InitNC") {  // 0xCB, 0x02
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0x02 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC D (NC, Z) (initial C)", "RLC-D-NC,Z-InitC") {  // 0xCB, 0x02
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x02 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC E (NC, NZ) (initial NC)", "RLC-E-NC,NZ-InitNC") {  // 0xCB, 0x03
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    LoadData(0x00, { 0xCB, 0x03 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC E (NC, NZ) (initial C)", "RLC-E-NC,NZ-InitC") {  // 0xCB, 0x03
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x03 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC E (C, NZ) (initial NC)", "RLC-E-C,NZ-InitNC") {  // 0xCB, 0x03
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    LoadData(0x00, { 0xCB, 0x03 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC E (C, NZ) (initial C)", "RLC-E-C,NZ-InitC") {  // 0xCB, 0x03
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x03 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC E (NC, Z) (initial NC)", "RLC-E-NC,Z-InitNC") {  // 0xCB, 0x03
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0x03 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC E (NC, Z) (initial C)", "RLC-E-NC,Z-InitC") {  // 0xCB, 0x03
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x03 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC H (NC, NZ) (initial NC)", "RLC-H-NC,NZ-InitNC") {  // 0xCB, 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    LoadData(0x00, { 0xCB, 0x04 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC H (NC, NZ) (initial C)", "RLC-H-NC,NZ-InitC") {  // 0xCB, 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x04 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC H (C, NZ) (initial NC)", "RLC-H-C,NZ-InitNC") {  // 0xCB, 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    LoadData(0x00, { 0xCB, 0x04 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC H (C, NZ) (initial C)", "RLC-H-C,NZ-InitC") {  // 0xCB, 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x04 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC H (NC, Z) (initial NC)", "RLC-H-NC,Z-InitNC") {  // 0xCB, 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0x04 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC H (NC, Z) (initial C)", "RLC-H-NC,Z-InitC") {  // 0xCB, 0x04
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x04 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC L (NC, NZ) (initial NC)", "RLC-L-NC,NZ-InitNC") {  // 0xCB, 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    LoadData(0x00, { 0xCB, 0x05 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC L (NC, NZ) (initial C)", "RLC-L-NC,NZ-InitC") {  // 0xCB, 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x05 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC L (C, NZ) (initial NC)", "RLC-L-C,NZ-InitNC") {  // 0xCB, 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    LoadData(0x00, { 0xCB, 0x05 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC L (C, NZ) (initial C)", "RLC-L-C,NZ-InitC") {  // 0xCB, 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x05 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC L (NC, Z) (initial NC)", "RLC-L-NC,Z-InitNC") {  // 0xCB, 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0x05 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC L (NC, Z) (initial C)", "RLC-L-NC,Z-InitC") {  // 0xCB, 0x05
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x05 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC <HL> (NC, NZ) (initial NC)", "RLC-HL-NC,NZ-InitNC") {  // 0xCB, 0x06
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    LoadData(0x00, { 0xCB, 0x06 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RLC <HL> (NC, NZ) (initial C)", "RLC-HL-NC,NZ-InitC") {  // 0xCB, 0x06
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x06 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RLC <HL> (C, NZ) (initial NC)", "RLC-HL-C,NZ-InitNC") {  // 0xCB, 0x06
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    LoadData(0x00, { 0xCB, 0x06 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RLC <HL> (C, NZ) (initial C)", "RLC-HL-C,NZ-InitC") {  // 0xCB, 0x06
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x06 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RLC <HL> (NC, Z) (initial NC)", "RLC-HL-NC,Z-InitNC") {  // 0xCB, 0x06
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0x06 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RLC <HL> (NC, Z) (initial C)", "RLC-HL-NC,Z-InitC") {  // 0xCB, 0x06
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x06 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RLC A (NC, NZ) (initial NC)", "RLC-A-NC,NZ-InitNC") {  // 0xCB, 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0xCB, 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC A (NC, NZ) (initial C)", "RLC-A-NC,NZ-InitC") {  // 0xCB, 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC A (C, NZ) (initial NC)", "RLC-A-C,NZ-InitNC") {  // 0xCB, 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0xCB, 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC A (C, NZ) (initial C)", "RLC-A-C,NZ-InitC") {  // 0xCB, 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC A (NC, Z) (initial NC)", "RLC-A-NC,Z-InitNC") {  // 0xCB, 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RLC A (NC, Z) (initial C)", "RLC-A-NC,Z-InitC") {  // 0xCB, 0x07
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x07 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC B (NC, NZ) (initial NC)", "RRC-B-NC,NZ-InitNC") {  // 0xCB, 0x08
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    LoadData(0x00, { 0xCB, 0x08 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC B (NC, NZ) (initial C)", "RRC-B-NC,NZ-InitC") {  // 0xCB, 0x08
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x08 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC B (C, NZ) (initial NC)", "RRC-B-C,NZ-InitNC") {  // 0xCB, 0x08
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    LoadData(0x00, { 0xCB, 0x08 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC B (C, NZ) (initial C)", "RRC-B-C,NZ-InitC") {  // 0xCB, 0x08
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x08 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC B (NC, Z) (initial NC)", "RRC-B-NC,Z-InitNC") {  // 0xCB, 0x08
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0x08 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC B (NC, Z) (initial C)", "RRC-B-NC,Z-InitC") {  // 0xCB, 0x08
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x08 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC C (NC, NZ) (initial NC)", "RRC-C-NC,NZ-InitNC") {  // 0xCB, 0x09
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    LoadData(0x00, { 0xCB, 0x09 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC C (NC, NZ) (initial C)", "RRC-C-NC,NZ-InitC") {  // 0xCB, 0x09
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x09 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC C (C, NZ) (initial NC)", "RRC-C-C,NZ-InitNC") {  // 0xCB, 0x09
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    LoadData(0x00, { 0xCB, 0x09 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC C (C, NZ) (initial C)", "RRC-C-C,NZ-InitC") {  // 0xCB, 0x09
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x09 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC C (NC, Z) (initial NC)", "RRC-C-NC,Z-InitNC") {  // 0xCB, 0x09
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0x09 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC C (NC, Z) (initial C)", "RRC-C-NC,Z-InitC") {  // 0xCB, 0x09
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x09 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC D (NC, NZ) (initial NC)", "RRC-D-NC,NZ-InitNC") {  // 0xCB, 0x0A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    LoadData(0x00, { 0xCB, 0x0A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC D (NC, NZ) (initial C)", "RRC-D-NC,NZ-InitC") {  // 0xCB, 0x0A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC D (C, NZ) (initial NC)", "RRC-D-C,NZ-InitNC") {  // 0xCB, 0x0A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    LoadData(0x00, { 0xCB, 0x0A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC D (C, NZ) (initial C)", "RRC-D-C,NZ-InitC") {  // 0xCB, 0x0A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC D (NC, Z) (initial NC)", "RRC-D-NC,Z-InitNC") {  // 0xCB, 0x0A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0x0A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC D (NC, Z) (initial C)", "RRC-D-NC,Z-InitC") {  // 0xCB, 0x0A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC E (NC, NZ) (initial NC)", "RRC-E-NC,NZ-InitNC") {  // 0xCB, 0x0B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    LoadData(0x00, { 0xCB, 0x0B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC E (NC, NZ) (initial C)", "RRC-E-NC,NZ-InitC") {  // 0xCB, 0x0B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC E (C, NZ) (initial NC)", "RRC-E-C,NZ-InitNC") {  // 0xCB, 0x0B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    LoadData(0x00, { 0xCB, 0x0B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC E (C, NZ) (initial C)", "RRC-E-C,NZ-InitC") {  // 0xCB, 0x0B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC E (NC, Z) (initial NC)", "RRC-E-NC,Z-InitNC") {  // 0xCB, 0x0B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0x0B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC E (NC, Z) (initial C)", "RRC-E-NC,Z-InitC") {  // 0xCB, 0x0B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC H (NC, NZ) (initial NC)", "RRC-H-NC,NZ-InitNC") {  // 0xCB, 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    LoadData(0x00, { 0xCB, 0x0C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC H (NC, NZ) (initial C)", "RRC-H-NC,NZ-InitC") {  // 0xCB, 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC H (C, NZ) (initial NC)", "RRC-H-C,NZ-InitNC") {  // 0xCB, 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    LoadData(0x00, { 0xCB, 0x0C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC H (C, NZ) (initial C)", "RRC-H-C,NZ-InitC") {  // 0xCB, 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC H (NC, Z) (initial NC)", "RRC-H-NC,Z-InitNC") {  // 0xCB, 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0x0C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC H (NC, Z) (initial C)", "RRC-H-NC,Z-InitC") {  // 0xCB, 0x0C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC L (NC, NZ) (initial NC)", "RRC-L-NC,NZ-InitNC") {  // 0xCB, 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    LoadData(0x00, { 0xCB, 0x0D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC L (NC, NZ) (initial C)", "RRC-L-NC,NZ-InitC") {  // 0xCB, 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC L (C, NZ) (initial NC)", "RRC-L-C,NZ-InitNC") {  // 0xCB, 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    LoadData(0x00, { 0xCB, 0x0D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC L (C, NZ) (initial C)", "RRC-L-C,NZ-InitC") {  // 0xCB, 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC L (NC, Z) (initial NC)", "RRC-L-NC,Z-InitNC") {  // 0xCB, 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0x0D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC L (NC, Z) (initial C)", "RRC-L-NC,Z-InitC") {  // 0xCB, 0x0D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC <HL> (NC, NZ) (initial NC)", "RRC-HL-NC,NZ-InitNC") {  // 0xCB, 0x0E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    LoadData(0x00, { 0xCB, 0x0E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RRC <HL> (NC, NZ) (initial C)", "RRC-HL-NC,NZ-InitC") {  // 0xCB, 0x0E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RRC <HL> (C, NZ) (initial NC)", "RRC-HL-C,NZ-InitNC") {  // 0xCB, 0x0E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    LoadData(0x00, { 0xCB, 0x0E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RRC <HL> (C, NZ) (initial C)", "RRC-HL-C,NZ-InitC") {  // 0xCB, 0x0E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RRC <HL> (NC, Z) (initial NC)", "RRC-HL-NC,Z-InitNC") {  // 0xCB, 0x0E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0x0E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RRC <HL> (NC, Z) (initial C)", "RRC-HL-NC,Z-InitC") {  // 0xCB, 0x0E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RRC A (NC, NZ) (initial NC)", "RRC-A-NC,NZ-InitNC") {  // 0xCB, 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0xCB, 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC A (NC, NZ) (initial C)", "RRC-A-NC,NZ-InitC") {  // 0xCB, 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC A (C, NZ) (initial NC)", "RRC-A-C,NZ-InitNC") {  // 0xCB, 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0xCB, 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC A (C, NZ) (initial C)", "RRC-A-C,NZ-InitC") {  // 0xCB, 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC A (NC, Z) (initial NC)", "RRC-A-NC,Z-InitNC") {  // 0xCB, 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RRC A (NC, Z) (initial C)", "RRC-A-NC,Z-InitC") {  // 0xCB, 0x0F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x0F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL B (NC, NZ) (initial NC)", "RL-B-NC,NZ-InitNC") {  // 0xCB, 0x10
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    LoadData(0x00, { 0xCB, 0x10 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL B (NC, NZ) (initial C)", "RL-B-NC,NZ-InitC") {  // 0xCB, 0x10
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x10 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10101011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL B (C, NZ) (initial NC)", "RL-B-NC,NZ-InitNC") {  // 0xCB, 0x10
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    LoadData(0x00, { 0xCB, 0x10 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01010100);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL B (C, NZ) (initial C)", "RL-B-NC,NZ-InitC") {  // 0xCB, 0x10
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x10 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL B (NC, Z) (initial NC)", "RL-B-NC,Z-InitNC") {  // 0xCB, 0x10
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0x10 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL B (C, Z) (initial NC)", "RL-B-NC,Z-InitNC") {  // 0xCB, 0x10
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10000000);
    LoadData(0x00, { 0xCB, 0x10 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL C (NC, NZ) (initial NC)", "RL-C-NC,NZ-InitNC") {  // 0xCB, 0x11
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    LoadData(0x00, { 0xCB, 0x11 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL C (NC, NZ) (initial C)", "RL-C-NC,NZ-InitC") {  // 0xCB, 0x11
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x11 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10101011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL C (C, NZ) (initial NC)", "RL-C-NC,NZ-InitNC") {  // 0xCB, 0x11
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    LoadData(0x00, { 0xCB, 0x11 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01010100);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL C (C, NZ) (initial C)", "RL-C-NC,NZ-InitC") {  // 0xCB, 0x11
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x11 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL C (NC, Z) (initial NC)", "RL-C-NC,Z-InitNC") {  // 0xCB, 0x11
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0x11 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL C (C, Z) (initial NC)", "RL-C-NC,Z-InitNC") {  // 0xCB, 0x11
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10000000);
    LoadData(0x00, { 0xCB, 0x11 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL D (NC, NZ) (initial NC)", "RL-D-NC,NZ-InitNC") {  // 0xCB, 0x12
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    LoadData(0x00, { 0xCB, 0x12 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL D (NC, NZ) (initial C)", "RL-D-NC,NZ-InitC") {  // 0xCB, 0x12
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x12 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10101011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL D (C, NZ) (initial NC)", "RL-D-NC,NZ-InitNC") {  // 0xCB, 0x12
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    LoadData(0x00, { 0xCB, 0x12 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01010100);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL D (C, NZ) (initial C)", "RL-D-NC,NZ-InitC") {  // 0xCB, 0x12
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x12 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL D (NC, Z) (initial NC)", "RL-D-NC,Z-InitNC") {  // 0xCB, 0x12
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0x12 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL D (C, Z) (initial NC)", "RL-D-NC,Z-InitNC") {  // 0xCB, 0x12
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10000000);
    LoadData(0x00, { 0xCB, 0x12 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL E (NC, NZ) (initial NC)", "RL-E-NC,NZ-InitNC") {  // 0xCB, 0x13
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    LoadData(0x00, { 0xCB, 0x13 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL E (NC, NZ) (initial C)", "RL-E-NC,NZ-InitC") {  // 0xCB, 0x13
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x13 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10101011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL E (C, NZ) (initial NC)", "RL-E-NC,NZ-InitNC") {  // 0xCB, 0x13
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    LoadData(0x00, { 0xCB, 0x13 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01010100);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL E (C, NZ) (initial C)", "RL-E-NC,NZ-InitC") {  // 0xCB, 0x13
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x13 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL E (NC, Z) (initial NC)", "RL-E-NC,Z-InitNC") {  // 0xCB, 0x13
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0x13 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL E (C, Z) (initial NC)", "RL-E-NC,Z-InitNC") {  // 0xCB, 0x13
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10000000);
    LoadData(0x00, { 0xCB, 0x13 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL H (NC, NZ) (initial NC)", "RL-H-NC,NZ-InitNC") {  // 0xCB, 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    LoadData(0x00, { 0xCB, 0x14 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL H (NC, NZ) (initial C)", "RL-H-NC,NZ-InitC") {  // 0xCB, 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x14 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10101011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL H (C, NZ) (initial NC)", "RL-H-NC,NZ-InitNC") {  // 0xCB, 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    LoadData(0x00, { 0xCB, 0x14 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01010100);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL H (C, NZ) (initial C)", "RL-H-NC,NZ-InitC") {  // 0xCB, 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x14 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL H (NC, Z) (initial NC)", "RL-H-NC,Z-InitNC") {  // 0xCB, 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0x14 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL H (C, Z) (initial NC)", "RL-H-NC,Z-InitNC") {  // 0xCB, 0x14
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10000000);
    LoadData(0x00, { 0xCB, 0x14 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL L (NC, NZ) (initial NC)", "RL-L-NC,NZ-InitNC") {  // 0xCB, 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    LoadData(0x00, { 0xCB, 0x15 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL L (NC, NZ) (initial C)", "RL-L-NC,NZ-InitC") {  // 0xCB, 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x15 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10101011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL L (C, NZ) (initial NC)", "RL-L-NC,NZ-InitNC") {  // 0xCB, 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    LoadData(0x00, { 0xCB, 0x15 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01010100);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL L (C, NZ) (initial C)", "RL-L-NC,NZ-InitC") {  // 0xCB, 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x15 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL L (NC, Z) (initial NC)", "RL-L-NC,Z-InitNC") {  // 0xCB, 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0x15 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL L (C, Z) (initial NC)", "RL-L-NC,Z-InitNC") {  // 0xCB, 0x15
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10000000);
    LoadData(0x00, { 0xCB, 0x15 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL <HL> (NC, NZ) (initial NC)", "RL-HL-NC,NZ-InitNC") {  // 0xCB, 0x16
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    LoadData(0x00, { 0xCB, 0x16 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RL <HL> (NC, NZ) (initial C)", "RL-HL-NC,NZ-InitC") {  // 0xCB, 0x16
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x16 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10101011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RL <HL> (C, NZ) (initial NC)", "RL-HL-NC,NZ-InitNC") {  // 0xCB, 0x16
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    LoadData(0x00, { 0xCB, 0x16 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01010100);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RL <HL> (C, NZ) (initial C)", "RL-HL-NC,NZ-InitC") {  // 0xCB, 0x16
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x16 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RL <HL> (NC, Z) (initial NC)", "RL-HL-NC,Z-InitNC") {  // 0xCB, 0x16
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0x16 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RL <HL> (C, Z) (initial NC)", "RL-HL-NC,Z-InitNC") {  // 0xCB, 0x16
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10000000);
    LoadData(0x00, { 0xCB, 0x16 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RL A (NC, NZ) (initial NC)", "RL-A-NC,NZ-InitNC") {  // 0xCB, 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0xCB, 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL A (NC, NZ) (initial C)", "RL-A-NC,NZ-InitC") {  // 0xCB, 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101011);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL A (C, NZ) (initial NC)", "RL-A-NC,NZ-InitNC") {  // 0xCB, 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0xCB, 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010100);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL A (C, NZ) (initial C)", "RL-A-NC,NZ-InitC") {  // 0xCB, 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL A (NC, Z) (initial NC)", "RL-A-NC,Z-InitNC") {  // 0xCB, 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RL A (C, Z) (initial NC)", "RL-A-NC,Z-InitNC") {  // 0xCB, 0x17
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10000000);
    LoadData(0x00, { 0xCB, 0x17 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR B (NC, NZ) (initial NC)", "RR-B-NC,NZ-InitNC") {  // 0xCB, 0x18
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    LoadData(0x00, { 0xCB, 0x18 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR B (NC, NZ) (initial C)", "RR-B-NC,NZ-InitC") {  // 0xCB, 0x18
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x18 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b11010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR B (C, NZ) (initial NC)", "RR-B-C,NZ-InitNC") {  // 0xCB, 0x18
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    LoadData(0x00, { 0xCB, 0x18 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR B (C, NZ) (initial C)", "RR-B-C,NZ-InitC") {  // 0xCB, 0x18
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x18 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR B (NC, Z) (initial NC)", "RR-B-NC,Z-InitNC") {  // 0xCB, 0x18
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0x18 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR B (C, Z) (initial NC)", "RR-B-C,Z-InitNC") {  // 0xCB, 0x18
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000001);
    LoadData(0x00, { 0xCB, 0x18 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR C (NC, NZ) (initial NC)", "RR-C-NC,NZ-InitNC") {  // 0xCB, 0x19
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    LoadData(0x00, { 0xCB, 0x19 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR C (NC, NZ) (initial C)", "RR-C-NC,NZ-InitC") {  // 0xCB, 0x19
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x19 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b11010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR C (C, NZ) (initial NC)", "RR-C-C,NZ-InitNC") {  // 0xCB, 0x19
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    LoadData(0x00, { 0xCB, 0x19 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR C (C, NZ) (initial C)", "RR-C-C,NZ-InitC") {  // 0xCB, 0x19
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x19 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR C (NC, Z) (initial NC)", "RR-C-NC,Z-InitNC") {  // 0xCB, 0x19
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0x19 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR C (C, Z) (initial NC)", "RR-C-C,Z-InitNC") {  // 0xCB, 0x19
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000001);
    LoadData(0x00, { 0xCB, 0x19 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR D (NC, NZ) (initial NC)", "RR-D-NC,NZ-InitNC") {  // 0xCB, 0x1A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    LoadData(0x00, { 0xCB, 0x1A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR D (NC, NZ) (initial C)", "RR-D-NC,NZ-InitC") {  // 0xCB, 0x1A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b11010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR D (C, NZ) (initial NC)", "RR-D-C,NZ-InitNC") {  // 0xCB, 0x1A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    LoadData(0x00, { 0xCB, 0x1A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR D (C, NZ) (initial C)", "RR-D-C,NZ-InitC") {  // 0xCB, 0x1A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR D (NC, Z) (initial NC)", "RR-D-NC,Z-InitNC") {  // 0xCB, 0x1A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0x1A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR D (C, Z) (initial NC)", "RR-D-C,Z-InitNC") {  // 0xCB, 0x1A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000001);
    LoadData(0x00, { 0xCB, 0x1A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR E (NC, NZ) (initial NC)", "RR-E-NC,NZ-InitNC") {  // 0xCB, 0x1B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    LoadData(0x00, { 0xCB, 0x1B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR E (NC, NZ) (initial C)", "RR-E-NC,NZ-InitC") {  // 0xCB, 0x1B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b11010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR E (C, NZ) (initial NC)", "RR-E-C,NZ-InitNC") {  // 0xCB, 0x1B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    LoadData(0x00, { 0xCB, 0x1B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR E (C, NZ) (initial C)", "RR-E-C,NZ-InitC") {  // 0xCB, 0x1B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR E (NC, Z) (initial NC)", "RR-E-NC,Z-InitNC") {  // 0xCB, 0x1B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0x1B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR E (C, Z) (initial NC)", "RR-E-C,Z-InitNC") {  // 0xCB, 0x1B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000001);
    LoadData(0x00, { 0xCB, 0x1B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR H (NC, NZ) (initial NC)", "RR-H-NC,NZ-InitNC") {  // 0xCB, 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    LoadData(0x00, { 0xCB, 0x1C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR H (NC, NZ) (initial C)", "RR-H-NC,NZ-InitC") {  // 0xCB, 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b11010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR H (C, NZ) (initial NC)", "RR-H-C,NZ-InitNC") {  // 0xCB, 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    LoadData(0x00, { 0xCB, 0x1C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR H (C, NZ) (initial C)", "RR-H-C,NZ-InitC") {  // 0xCB, 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR H (NC, Z) (initial NC)", "RR-H-NC,Z-InitNC") {  // 0xCB, 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0x1C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR H (C, Z) (initial NC)", "RR-H-C,Z-InitNC") {  // 0xCB, 0x1C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000001);
    LoadData(0x00, { 0xCB, 0x1C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR L (NC, NZ) (initial NC)", "RR-L-NC,NZ-InitNC") {  // 0xCB, 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    LoadData(0x00, { 0xCB, 0x1D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR L (NC, NZ) (initial C)", "RR-L-NC,NZ-InitC") {  // 0xCB, 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b11010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR L (C, NZ) (initial NC)", "RR-L-C,NZ-InitNC") {  // 0xCB, 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    LoadData(0x00, { 0xCB, 0x1D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR L (C, NZ) (initial C)", "RR-L-C,NZ-InitC") {  // 0xCB, 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR L (NC, Z) (initial NC)", "RR-L-NC,Z-InitNC") {  // 0xCB, 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0x1D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR L (C, Z) (initial NC)", "RR-L-C,Z-InitNC") {  // 0xCB, 0x1D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000001);
    LoadData(0x00, { 0xCB, 0x1D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR <HL> (NC, NZ) (initial NC)", "RR-HL-NC,NZ-InitNC") {  // 0xCB, 0x1E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    LoadData(0x00, { 0xCB, 0x1E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RR <HL> (NC, NZ) (initial C)", "RR-HL-NC,NZ-InitC") {  // 0xCB, 0x1E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b11010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RR <HL> (C, NZ) (initial NC)", "RR-HL-C,NZ-InitNC") {  // 0xCB, 0x1E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    LoadData(0x00, { 0xCB, 0x1E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RR <HL> (C, NZ) (initial C)", "RR-HL-C,NZ-InitC") {  // 0xCB, 0x1E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RR <HL> (NC, Z) (initial NC)", "RR-HL-NC,Z-InitNC") {  // 0xCB, 0x1E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0x1E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RR <HL> (C, Z) (initial NC)", "RR-HL-C,Z-InitNC") {
    // 0xCB, 0x1E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000001);
    LoadData(0x00, { 0xCB, 0x1E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 4);
}

TEST("RR A (NC, NZ) (initial NC)", "RR-A-NC,NZ-InitNC") {  // 0xCB, 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0xCB, 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR A (NC, NZ) (initial C)", "RR-A-NC,NZ-InitC") {  // 0xCB, 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11010101);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR A (C, NZ) (initial NC)", "RR-A-C,NZ-InitNC") {  // 0xCB, 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0xCB, 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR A (C, NZ) (initial C)", "RR-A-C,NZ-InitC") {  // 0xCB, 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    cpu->SetCarryFlag();
    LoadData(0x00, { 0xCB, 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR A (NC, Z) (initial NC)", "RR-A-NC,Z-InitNC") {  // 0xCB, 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_CARRY_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("RR A (C, Z) (initial NC)", "RR-A-C,Z-InitNC") {  // 0xCB, 0x1F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000001);
    LoadData(0x00, { 0xCB, 0x1F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_CARRY_SET;
    CHECK_HALF_CLEAR;
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA B (NC, NZ)", "SLA-B-NC,NZ") {  // 0xCB, 0x20
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    LoadData(0x00, { 0xCB, 0x20 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA B (C, NZ)", "SLA-B-C,NZ") {  // 0xCB, 0x20
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    LoadData(0x00, { 0xCB, 0x20 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01010100);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA B (NC, Z)", "SLA-B-NC,Z") {  // 0xCB, 0x20
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0x20 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA B (C, Z)", "SLA-B-C,Z") {  // 0xCB, 0x20
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10000000);
    LoadData(0x00, { 0xCB, 0x20 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA C (NC, NZ)", "SLA-C-NC,NZ") {  // 0xCB, 0x21
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    LoadData(0x00, { 0xCB, 0x21 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA C (C, NZ)", "SLA-C-C,NZ") {  // 0xCB, 0x21
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    LoadData(0x00, { 0xCB, 0x21 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01010100);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA C (NC, Z)", "SLA-C-NC,Z") {  // 0xCB, 0x21
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0x21 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA C (C, Z)", "SLA-C-C,Z") {  // 0xCB, 0x21
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10000000);
    LoadData(0x00, { 0xCB, 0x21 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA D (NC, NZ)", "SLA-D-NC,NZ") {  // 0xCB, 0x22
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    LoadData(0x00, { 0xCB, 0x22 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA D (C, NZ)", "SLA-D-C,NZ") {  // 0xCB, 0x22
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    LoadData(0x00, { 0xCB, 0x22 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01010100);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA D (NC, Z)", "SLA-D-NC,Z") {  // 0xCB, 0x22
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0x22 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA D (C, Z)", "SLA-D-C,Z") {  // 0xCB, 0x22
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10000000);
    LoadData(0x00, { 0xCB, 0x22 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA E (NC, NZ)", "SLA-E-NC,NZ") {  // 0xCB, 0x23
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    LoadData(0x00, { 0xCB, 0x23 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA E (C, NZ)", "SLA-E-C,NZ") {  // 0xCB, 0x23
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    LoadData(0x00, { 0xCB, 0x23 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01010100);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA E (NC, Z)", "SLA-E-NC,Z") {  // 0xCB, 0x23
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0x23 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA E (C, Z)", "SLA-E-C,Z") {  // 0xCB, 0x23
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10000000);
    LoadData(0x00, { 0xCB, 0x23 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA H (NC, NZ)", "SLA-H-NC,NZ") {  // 0xCB, 0x24
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    LoadData(0x00, { 0xCB, 0x24 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA H (C, NZ)", "SLA-H-C,NZ") {  // 0xCB, 0x24
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    LoadData(0x00, { 0xCB, 0x24 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01010100);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA H (NC, Z)", "SLA-H-NC,Z") {  // 0xCB, 0x24
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0x24 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA H (C, Z)", "SLA-H-C,Z") {  // 0xCB, 0x24
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10000000);
    LoadData(0x00, { 0xCB, 0x24 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA L (NC, NZ)", "SLA-L-NC,NZ") {  // 0xCB, 0x25
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    LoadData(0x00, { 0xCB, 0x25 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA L (C, NZ)", "SLA-L-C,NZ") {  // 0xCB, 0x25
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    LoadData(0x00, { 0xCB, 0x25 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01010100);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA L (NC, Z)", "SLA-L-NC,Z") {  // 0xCB, 0x25
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0x25 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA L (C, Z)", "SLA-L-C,Z") {  // 0xCB, 0x25
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10000000);
    LoadData(0x00, { 0xCB, 0x25 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA <HL> (NC, NZ)", "SLA-HL-NC,NZ") {  // 0xCB, 0x26
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    LoadData(0x00, { 0xCB, 0x26 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("SLA <HL> (C, NZ)", "SLA-HL-C,NZ") {  // 0xCB, 0x26
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    LoadData(0x00, { 0xCB, 0x26 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01010100);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("SLA <HL> (NC, Z)", "SLA-HL-NC,Z") {  // 0xCB, 0x26
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0x26 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("SLA <HL> (C, Z)", "SLA-HL-C,Z") {  // 0xCB, 0x26
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10000000);
    LoadData(0x00, { 0xCB, 0x26 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("SLA A (NC, NZ)", "SLA-A-NC,NZ") {  // 0xCB, 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0xCB, 0x27 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA A (C, NZ)", "SLA-A-C,NZ") {  // 0xCB, 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0xCB, 0x27 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010100);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SLA A (NC, Z)", "SLA-A-NC,Z") {  // 0xCB, 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0x27 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SLA A (C, Z)", "SLA-A-C,Z") {  // 0xCB, 0x27
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10000000);
    LoadData(0x00, { 0xCB, 0x27 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA B (NC, NZ)", "SRA-B-NC,NZ") {  // 0xCB, 0x28
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    LoadData(0x00, { 0xCB, 0x28 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b11010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA B (C, NZ)", "SRA-B-C,NZ") {  // 0xCB, 0x28
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    LoadData(0x00, { 0xCB, 0x28 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA B (NC, Z)", "SRA-B-NC,Z") {  // 0xCB, 0x28
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0x28 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA B (C, Z)", "SRA-B-C,Z") {  // 0xCB, 0x28
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000001);
    LoadData(0x00, { 0xCB, 0x28 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA C (NC, NZ)", "SRA-C-NC,NZ") {  // 0xCB, 0x29
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    LoadData(0x00, { 0xCB, 0x29 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b11010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA C (C, NZ)", "SRA-C-C,NZ") {  // 0xCB, 0x29
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    LoadData(0x00, { 0xCB, 0x29 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA C (NC, Z)", "SRA-C-NC,Z") {  // 0xCB, 0x29
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0x29 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA C (C, Z)", "SRA-C-C,Z") {  // 0xCB, 0x29
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000001);
    LoadData(0x00, { 0xCB, 0x29 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA D (NC, NZ)", "SRA-D-NC,NZ") {  // 0xCB, 0x2A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    LoadData(0x00, { 0xCB, 0x2A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b11010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA D (C, NZ)", "SRA-D-C,NZ") {  // 0xCB, 0x2A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    LoadData(0x00, { 0xCB, 0x2A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA D (NC, Z)", "SRA-D-NC,Z") {  // 0xCB, 0x2A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0x2A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA D (C, Z)", "SRA-D-C,Z") {  // 0xCB, 0x2A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000001);
    LoadData(0x00, { 0xCB, 0x2A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA E (NC, NZ)", "SRA-E-NC,NZ") {  // 0xCB, 0x2B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    LoadData(0x00, { 0xCB, 0x2B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b11010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA E (C, NZ)", "SRA-E-C,NZ") {  // 0xCB, 0x2B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    LoadData(0x00, { 0xCB, 0x2B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA E (NC, Z)", "SRA-E-NC,Z") {  // 0xCB, 0x2B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0x2B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA E (C, Z)", "SRA-E-C,Z") {  // 0xCB, 0x2B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000001);
    LoadData(0x00, { 0xCB, 0x2B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA H (NC, NZ)", "SRA-H-NC,NZ") {  // 0xCB, 0x2C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    LoadData(0x00, { 0xCB, 0x2C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b11010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA H (C, NZ)", "SRA-H-C,NZ") {  // 0xCB, 0x2C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    LoadData(0x00, { 0xCB, 0x2C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA H (NC, Z)", "SRA-H-NC,Z") {  // 0xCB, 0x2C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0x2C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA H (C, Z)", "SRA-H-C,Z") {  // 0xCB, 0x2C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000001);
    LoadData(0x00, { 0xCB, 0x2C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA L (NC, NZ)", "SRA-L-NC,NZ") {  // 0xCB, 0x2D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    LoadData(0x00, { 0xCB, 0x2D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b11010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA L (C, NZ)", "SRA-L-C,NZ") {  // 0xCB, 0x2D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    LoadData(0x00, { 0xCB, 0x2D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA L (NC, Z)", "SRA-L-NC,Z") {  // 0xCB, 0x2D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0x2D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA L (C, Z)", "SRA-L-C,Z") {  // 0xCB, 0x2D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000001);
    LoadData(0x00, { 0xCB, 0x2D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA <HL> (NC, NZ)", "SRA-HL-NC,NZ") {  // 0xCB, 0x2E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    LoadData(0x00, { 0xCB, 0x2E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b11010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("SRA <HL> (C, NZ)", "SRA-HL-C,NZ") {  // 0xCB, 0x2E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    LoadData(0x00, { 0xCB, 0x2E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("SRA <HL> (NC, Z)", "SRA-HL-NC,Z") {  // 0xCB, 0x2E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0x2E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("SRA <HL> (C, Z)", "SRA-HL-C,Z") {  // 0xCB, 0x2E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000001);
    LoadData(0x00, { 0xCB, 0x2E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("SRA A (NC, NZ)", "SRA-A-NC,NZ") {  // 0xCB, 0x2F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0xCB, 0x2F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA A (C, NZ)", "SRA-A-C,NZ") {  // 0xCB, 0x2F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0xCB, 0x2F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRA A (NC, Z)", "SRA-A-NC,Z") {  // 0xCB, 0x2F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0x2F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRA A (C, Z)", "SRA-A-C,Z") {  // 0xCB, 0x2F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000001);
    LoadData(0x00, { 0xCB, 0x2F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SWAP B (NZ)", "SWAP-B-NZ") {  // 0xCB, 0x30
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101111);
    LoadData(0x00, { 0xCB, 0x30 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b11111010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP B (Z)", "SWAP-B-Z") {  // 0xCB, 0x30
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0x30 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP C (NZ)", "SWAP-C-NZ") {  // 0xCB, 0x31
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101111);
    LoadData(0x00, { 0xCB, 0x31 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b11111010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP C (Z)", "SWAP-C-Z") {  // 0xCB, 0x31
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0x31 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP D (NZ)", "SWAP-D-NZ") {  // 0xCB, 0x32
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101111);
    LoadData(0x00, { 0xCB, 0x32 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b11111010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP D (Z)", "SWAP-D-Z") {  // 0xCB, 0x32
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0x32 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP E (NZ)", "SWAP-E-NZ") {  // 0xCB, 0x33
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101111);
    LoadData(0x00, { 0xCB, 0x33 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b11111010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP E (Z)", "SWAP-E-Z") {  // 0xCB, 0x33
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0x33 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP H (NZ)", "SWAP-H-NZ") {  // 0xCB, 0x34
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101111);
    LoadData(0x00, { 0xCB, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b11111010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP H (Z)", "SWAP-H-Z") {  // 0xCB, 0x34
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0x34 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP L (NZ)", "SWAP-L-NZ") {  // 0xCB, 0x35
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101111);
    LoadData(0x00, { 0xCB, 0x35 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b11111010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP L (Z)", "SWAP-L-Z") {  // 0xCB, 0x35
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0x35 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP <HL> (NZ)", "SWAP-HL-NZ") {  // 0xCB, 0x36
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101111);
    LoadData(0x00, { 0xCB, 0x36 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b11111010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("SWAP <HL> (Z)", "SWAP-HL-Z") {  // 0xCB, 0x36
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0x36 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("SWAP A (NZ)", "SWAP-A-NZ") {  // 0xCB, 0x37
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101111);
    LoadData(0x00, { 0xCB, 0x37 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11111010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SWAP A (Z)", "SWAP-A-Z") {  // 0xCB, 0x37
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0x37 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL B (NC, NZ)", "SRL-B-NC,NZ") {  // 0xCB, 0x38
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10101010);
    LoadData(0x00, { 0xCB, 0x38 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL B (C, NZ)", "SRL-B-C,NZ") {  // 0xCB, 0x38
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01010101);
    LoadData(0x00, { 0xCB, 0x38 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL B (NC, Z)", "SRL-B-NC,Z") {  // 0xCB, 0x38
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0x38 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL B (C, Z)", "SRL-B-C,Z") {  // 0xCB, 0x38
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000001);
    LoadData(0x00, { 0xCB, 0x38 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL C (NC, NZ)", "SRL-C-NC,NZ") {  // 0xCB, 0x39
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10101010);
    LoadData(0x00, { 0xCB, 0x39 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL C (C, NZ)", "SRL-C-C,NZ") {  // 0xCB, 0x39
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01010101);
    LoadData(0x00, { 0xCB, 0x39 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL C (NC, Z)", "SRL-C-NC,Z") {  // 0xCB, 0x39
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0x39 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL C (C, Z)", "SRL-C-C,Z") {  // 0xCB, 0x39
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000001);
    LoadData(0x00, { 0xCB, 0x39 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL D (NC, NZ)", "SRL-D-NC,NZ") {  // 0xCB, 0x3A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10101010);
    LoadData(0x00, { 0xCB, 0x3A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL D (C, NZ)", "SRL-D-C,NZ") {  // 0xCB, 0x3A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01010101);
    LoadData(0x00, { 0xCB, 0x3A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL D (NC, Z)", "SRL-D-NC,Z") {  // 0xCB, 0x3A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0x3A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL D (C, Z)", "SRL-D-C,Z") {  // 0xCB, 0x3A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000001);
    LoadData(0x00, { 0xCB, 0x3A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL E (NC, NZ)", "SRL-E-NC,NZ") {  // 0xCB, 0x3B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10101010);
    LoadData(0x00, { 0xCB, 0x3B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL E (C, NZ)", "SRL-E-C,NZ") {  // 0xCB, 0x3B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01010101);
    LoadData(0x00, { 0xCB, 0x3B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL E (NC, Z)", "SRL-E-NC,Z") {  // 0xCB, 0x3B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0x3B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL E (C, Z)", "SRL-E-C,Z") {  // 0xCB, 0x3B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000001);
    LoadData(0x00, { 0xCB, 0x3B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL H (NC, NZ)", "SRL-H-NC,NZ") {  // 0xCB, 0x3C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10101010);
    LoadData(0x00, { 0xCB, 0x3C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL H (C, NZ)", "SRL-H-C,NZ") {  // 0xCB, 0x3C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01010101);
    LoadData(0x00, { 0xCB, 0x3C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL H (NC, Z)", "SRL-H-NC,Z") {  // 0xCB, 0x3C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0x3C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL H (C, Z)", "SRL-H-C,Z") {  // 0xCB, 0x3C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000001);
    LoadData(0x00, { 0xCB, 0x3C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL L (NC, NZ)", "SRL-L-NC,NZ") {  // 0xCB, 0x3D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10101010);
    LoadData(0x00, { 0xCB, 0x3D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL L (C, NZ)", "SRL-L-C,NZ") {  // 0xCB, 0x3D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01010101);
    LoadData(0x00, { 0xCB, 0x3D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL L (NC, Z)", "SRL-L-NC,Z") {  // 0xCB, 0x3D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0x3D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL L (C, Z)", "SRL-L-C,Z") {  // 0xCB, 0x3D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000001);
    LoadData(0x00, { 0xCB, 0x3D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL <HL> (NC, NZ)", "SRL-HL-NC,NZ") {  // 0xCB, 0x3E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10101010);
    LoadData(0x00, { 0xCB, 0x3E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("SRL <HL> (C, NZ)", "SRL-HL-C,NZ") {  // 0xCB, 0x3E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01010101);
    LoadData(0x00, { 0xCB, 0x3E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("SRL <HL> (NC, Z)", "SRL-HL-NC,Z") {  // 0xCB, 0x3E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0x3E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 4);
}

TEST("SRL <HL> (C, Z)", "SRL-HL-C,Z") {  // 0xCB, 0x3E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000001);
    LoadData(0x00, { 0xCB, 0x3E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 4);
}

TEST("SRL A (NC, NZ)", "SRL-A-NC,NZ") {  // 0xCB, 0x3F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10101010);
    LoadData(0x00, { 0xCB, 0x3F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01010101);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL A (C, NZ)", "SRL-A-C,NZ") {  // 0xCB, 0x3F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01010101);
    LoadData(0x00, { 0xCB, 0x3F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00101010);
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("SRL A (NC, Z)", "SRL-A-NC,Z") {  // 0xCB, 0x3F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0x3F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_CLEAR;
    CHECK(cycles == 2);
}

TEST("SRL A (C, Z)", "SRL-A-C,Z") {  // 0xCB, 0x3F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000001);
    LoadData(0x00, { 0xCB, 0x3F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000000);
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_CLEAR;
    CHECK_CARRY_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, B (NZ)", "BIT-0,B-NZ") {  // 0xCB, 0x40
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000001);
    LoadData(0x00, { 0xCB, 0x40 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, B (Z)", "BIT-0,B-Z") {  // 0xCB, 0x40
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111110);
    LoadData(0x00, { 0xCB, 0x40 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, B (NZ)", "BIT-1,B-NZ") {  // 0xCB, 0x48
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000010);
    LoadData(0x00, { 0xCB, 0x48 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, B (Z)", "BIT-1,B-Z") {  // 0xCB, 0x48
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111101);
    LoadData(0x00, { 0xCB, 0x48 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, B (NZ)", "BIT-2,B-NZ") {  // 0xCB, 0x50
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000100);
    LoadData(0x00, { 0xCB, 0x50 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, B (Z)", "BIT-2,B-Z") {  // 0xCB, 0x50
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111011);
    LoadData(0x00, { 0xCB, 0x50 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, B (NZ)", "BIT-3,B-NZ") {  // 0xCB, 0x58
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00001000);
    LoadData(0x00, { 0xCB, 0x58 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, B (Z)", "BIT-3,B-Z") {  // 0xCB, 0x58
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11110111);
    LoadData(0x00, { 0xCB, 0x58 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, B (NZ)", "BIT-4,B-NZ") {  // 0xCB, 0x60
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00010000);
    LoadData(0x00, { 0xCB, 0x60 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, B (Z)", "BIT-4,B-Z") {  // 0xCB, 0x60
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11101111);
    LoadData(0x00, { 0xCB, 0x60 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, B (NZ)", "BIT-5,B-NZ") {  // 0xCB, 0x68
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00100000);
    LoadData(0x00, { 0xCB, 0x68 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, B (Z)", "BIT-5,B-Z") {  // 0xCB, 0x68
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11011111);
    LoadData(0x00, { 0xCB, 0x68 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, B (NZ)", "BIT-6,B-NZ") {  // 0xCB, 0x70
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01000000);
    LoadData(0x00, { 0xCB, 0x70 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, B (Z)", "BIT-6,B-Z") {  // 0xCB, 0x70
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10111111);
    LoadData(0x00, { 0xCB, 0x70 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, B (NZ)", "BIT-7,B-NZ") {  // 0xCB, 0x78
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b10000000);
    LoadData(0x00, { 0xCB, 0x78 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, B (Z)", "BIT-7,B-Z") {  // 0xCB, 0x78
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b01111111);
    LoadData(0x00, { 0xCB, 0x78 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, C (NZ)", "BIT-0,C-NZ") {  // 0xCB, 0x41
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000001);
    LoadData(0x00, { 0xCB, 0x41 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, C (Z)", "BIT-0,C-Z") {  // 0xCB, 0x41
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111110);
    LoadData(0x00, { 0xCB, 0x41 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, C (NZ)", "BIT-1,C-NZ") {  // 0xCB, 0x49
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000010);
    LoadData(0x00, { 0xCB, 0x49 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, C (Z)", "BIT-1,C-Z") {  // 0xCB, 0x49
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111101);
    LoadData(0x00, { 0xCB, 0x49 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, C (NZ)", "BIT-2,C-NZ") {  // 0xCB, 0x51
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000100);
    LoadData(0x00, { 0xCB, 0x51 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, C (Z)", "BIT-2,C-Z") {  // 0xCB, 0x51
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111011);
    LoadData(0x00, { 0xCB, 0x51 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, C (NZ)", "BIT-3,C-NZ") {  // 0xCB, 0x59
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00001000);
    LoadData(0x00, { 0xCB, 0x59 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, C (Z)", "BIT-3,C-Z") {  // 0xCB, 0x59
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11110111);
    LoadData(0x00, { 0xCB, 0x59 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, C (NZ)", "BIT-4,C-NZ") {  // 0xCB, 0x61
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00010000);
    LoadData(0x00, { 0xCB, 0x61 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, C (Z)", "BIT-4,C-Z") {  // 0xCB, 0x61
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11101111);
    LoadData(0x00, { 0xCB, 0x61 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, C (NZ)", "BIT-5,C-NZ") {  // 0xCB, 0x69
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00100000);
    LoadData(0x00, { 0xCB, 0x69 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, C (Z)", "BIT-5,C-Z") {  // 0xCB, 0x69
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11011111);
    LoadData(0x00, { 0xCB, 0x69 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, C (NZ)", "BIT-6,C-NZ") {  // 0xCB, 0x71
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01000000);
    LoadData(0x00, { 0xCB, 0x71 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, C (Z)", "BIT-6,C-Z") {  // 0xCB, 0x71
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10111111);
    LoadData(0x00, { 0xCB, 0x71 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, C (NZ)", "BIT-7,C-NZ") {  // 0xCB, 0x79
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b10000000);
    LoadData(0x00, { 0xCB, 0x79 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, C (Z)", "BIT-7,C-Z") {  // 0xCB, 0x79
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b01111111);
    LoadData(0x00, { 0xCB, 0x79 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, D (NZ)", "BIT-0,D-NZ") {  // 0xCB, 0x42
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000001);
    LoadData(0x00, { 0xCB, 0x42 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, D (Z)", "BIT-0,D-Z") {  // 0xCB, 0x42
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111110);
    LoadData(0x00, { 0xCB, 0x42 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, D (NZ)", "BIT-1,D-NZ") {  // 0xCB, 0x4A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000010);
    LoadData(0x00, { 0xCB, 0x4A });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, D (Z)", "BIT-1,D-Z") {  // 0xCB, 0x4A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111101);
    LoadData(0x00, { 0xCB, 0x4A });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, D (NZ)", "BIT-2,D-NZ") {  // 0xCB, 0x52
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000100);
    LoadData(0x00, { 0xCB, 0x52 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, D (Z)", "BIT-2,D-Z") {  // 0xCB, 0x52
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111011);
    LoadData(0x00, { 0xCB, 0x52 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, D (NZ)", "BIT-3,D-NZ") {  // 0xCB, 0x5A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00001000);
    LoadData(0x00, { 0xCB, 0x5A });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, D (Z)", "BIT-3,D-Z") {  // 0xCB, 0x5A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11110111);
    LoadData(0x00, { 0xCB, 0x5A });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, D (NZ)", "BIT-4,D-NZ") {  // 0xCB, 0x62
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00010000);
    LoadData(0x00, { 0xCB, 0x62 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, D (Z)", "BIT-4,D-Z") {  // 0xCB, 0x62
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11101111);
    LoadData(0x00, { 0xCB, 0x62 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, D (NZ)", "BIT-5,D-NZ") {  // 0xCB, 0x6A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00100000);
    LoadData(0x00, { 0xCB, 0x6A });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, D (Z)", "BIT-5,D-Z") {  // 0xCB, 0x6A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11011111);
    LoadData(0x00, { 0xCB, 0x6A });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, D (NZ)", "BIT-6,D-NZ") {  // 0xCB, 0x72
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01000000);
    LoadData(0x00, { 0xCB, 0x72 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, D (Z)", "BIT-6,D-Z") {  // 0xCB, 0x72
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10111111);
    LoadData(0x00, { 0xCB, 0x72 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, D (NZ)", "BIT-7,D-NZ") {  // 0xCB, 0x7A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b10000000);
    LoadData(0x00, { 0xCB, 0x7A });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, D (Z)", "BIT-7,D-Z") {  // 0xCB, 0x7A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b01111111);
    LoadData(0x00, { 0xCB, 0x7A });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, E (NZ)", "BIT-0,E-NZ") {  // 0xCB, 0x43
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000001);
    LoadData(0x00, { 0xCB, 0x43 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, E (Z)", "BIT-0,E-Z") {  // 0xCB, 0x43
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111110);
    LoadData(0x00, { 0xCB, 0x43 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, E (NZ)", "BIT-1,E-NZ") {  // 0xCB, 0x4B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000010);
    LoadData(0x00, { 0xCB, 0x4B });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, E (Z)", "BIT-1,E-Z") {  // 0xCB, 0x4B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111101);
    LoadData(0x00, { 0xCB, 0x4B });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, E (NZ)", "BIT-2,E-NZ") {  // 0xCB, 0x53
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000100);
    LoadData(0x00, { 0xCB, 0x53 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, E (Z)", "BIT-2,E-Z") {  // 0xCB, 0x53
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111011);
    LoadData(0x00, { 0xCB, 0x53 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, E (NZ)", "BIT-3,E-NZ") {  // 0xCB, 0x5B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00001000);
    LoadData(0x00, { 0xCB, 0x5B });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, E (Z)", "BIT-3,E-Z") {  // 0xCB, 0x5B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11110111);
    LoadData(0x00, { 0xCB, 0x5B });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, E (NZ)", "BIT-4,E-NZ") {  // 0xCB, 0x63
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00010000);
    LoadData(0x00, { 0xCB, 0x63 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, E (Z)", "BIT-4,E-Z") {  // 0xCB, 0x63
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11101111);
    LoadData(0x00, { 0xCB, 0x63 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, E (NZ)", "BIT-5,E-NZ") {  // 0xCB, 0x6B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00100000);
    LoadData(0x00, { 0xCB, 0x6B });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, E (Z)", "BIT-5,E-Z") {  // 0xCB, 0x6B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11011111);
    LoadData(0x00, { 0xCB, 0x6B });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, E (NZ)", "BIT-6,E-NZ") {  // 0xCB, 0x73
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01000000);
    LoadData(0x00, { 0xCB, 0x73 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, E (Z)", "BIT-6,E-Z") {  // 0xCB, 0x73
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10111111);
    LoadData(0x00, { 0xCB, 0x73 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, E (NZ)", "BIT-7,E-NZ") {  // 0xCB, 0x7B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b10000000);
    LoadData(0x00, { 0xCB, 0x7B });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, E (Z)", "BIT-7,E-Z") {  // 0xCB, 0x7B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b01111111);
    LoadData(0x00, { 0xCB, 0x7B });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, H (NZ)", "BIT-0,H-NZ") {  // 0xCB, 0x44
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000001);
    LoadData(0x00, { 0xCB, 0x44 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, H (Z)", "BIT-0,H-Z") {  // 0xCB, 0x44
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111110);
    LoadData(0x00, { 0xCB, 0x44 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, H (NZ)", "BIT-1,H-NZ") {  // 0xCB, 0x4C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000010);
    LoadData(0x00, { 0xCB, 0x4C });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, H (Z)", "BIT-1,H-Z") {  // 0xCB, 0x4C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111101);
    LoadData(0x00, { 0xCB, 0x4C });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, H (NZ)", "BIT-2,H-NZ") {  // 0xCB, 0x54
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000100);
    LoadData(0x00, { 0xCB, 0x54 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, H (Z)", "BIT-2,H-Z") {  // 0xCB, 0x54
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111011);
    LoadData(0x00, { 0xCB, 0x54 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, H (NZ)", "BIT-3,H-NZ") {  // 0xCB, 0x5C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00001000);
    LoadData(0x00, { 0xCB, 0x5C });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, H (Z)", "BIT-3,H-Z") {  // 0xCB, 0x5C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11110111);
    LoadData(0x00, { 0xCB, 0x5C });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, H (NZ)", "BIT-4,H-NZ") {  // 0xCB, 0x64
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00010000);
    LoadData(0x00, { 0xCB, 0x64 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, H (Z)", "BIT-4,H-Z") {  // 0xCB, 0x64
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11101111);
    LoadData(0x00, { 0xCB, 0x64 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, H (NZ)", "BIT-5,H-NZ") {  // 0xCB, 0x6C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00100000);
    LoadData(0x00, { 0xCB, 0x6C });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, H (Z)", "BIT-5,H-Z") {  // 0xCB, 0x6C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11011111);
    LoadData(0x00, { 0xCB, 0x6C });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, H (NZ)", "BIT-6,H-NZ") {  // 0xCB, 0x74
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01000000);
    LoadData(0x00, { 0xCB, 0x74 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, H (Z)", "BIT-6,H-Z") {  // 0xCB, 0x74
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10111111);
    LoadData(0x00, { 0xCB, 0x74 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, H (NZ)", "BIT-7,H-NZ") {  // 0xCB, 0x7C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b10000000);
    LoadData(0x00, { 0xCB, 0x7C });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, H (Z)", "BIT-7,H-Z") {  // 0xCB, 0x7C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b01111111);
    LoadData(0x00, { 0xCB, 0x7C });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, L (NZ)", "BIT-0,L-NZ") {  // 0xCB, 0x45
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000001);
    LoadData(0x00, { 0xCB, 0x45 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, L (Z)", "BIT-0,L-Z") {  // 0xCB, 0x45
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111110);
    LoadData(0x00, { 0xCB, 0x45 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, L (NZ)", "BIT-1,L-NZ") {  // 0xCB, 0x4D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000010);
    LoadData(0x00, { 0xCB, 0x4D });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, L (Z)", "BIT-1,L-Z") {  // 0xCB, 0x4D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111101);
    LoadData(0x00, { 0xCB, 0x4D });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, L (NZ)", "BIT-2,L-NZ") {  // 0xCB, 0x55
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000100);
    LoadData(0x00, { 0xCB, 0x55 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, L (Z)", "BIT-2,L-Z") {  // 0xCB, 0x55
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111011);
    LoadData(0x00, { 0xCB, 0x55 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, L (NZ)", "BIT-3,L-NZ") {  // 0xCB, 0x5D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00001000);
    LoadData(0x00, { 0xCB, 0x5D });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, L (Z)", "BIT-3,L-Z") {  // 0xCB, 0x5D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11110111);
    LoadData(0x00, { 0xCB, 0x5D });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, L (NZ)", "BIT-4,L-NZ") {  // 0xCB, 0x65
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00010000);
    LoadData(0x00, { 0xCB, 0x65 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, L (Z)", "BIT-4,L-Z") {  // 0xCB, 0x65
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11101111);
    LoadData(0x00, { 0xCB, 0x65 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, L (NZ)", "BIT-5,L-NZ") {  // 0xCB, 0x6D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00100000);
    LoadData(0x00, { 0xCB, 0x6D });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, L (Z)", "BIT-5,L-Z") {  // 0xCB, 0x6D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11011111);
    LoadData(0x00, { 0xCB, 0x6D });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, L (NZ)", "BIT-6,L-NZ") {  // 0xCB, 0x75
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01000000);
    LoadData(0x00, { 0xCB, 0x75 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, L (Z)", "BIT-6,L-Z") {  // 0xCB, 0x75
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10111111);
    LoadData(0x00, { 0xCB, 0x75 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, L (NZ)", "BIT-7,L-NZ") {  // 0xCB, 0x7D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b10000000);
    LoadData(0x00, { 0xCB, 0x7D });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, L (Z)", "BIT-7,L-Z") {  // 0xCB, 0x7D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b01111111);
    LoadData(0x00, { 0xCB, 0x7D });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, <HL> (NZ)", "BIT-0,HL-NZ") {  // 0xCB, 0x46
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000001);
    LoadData(0x00, { 0xCB, 0x46 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 0, <HL> (Z)", "BIT-0,HL-Z") {  // 0xCB, 0x46
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111110);
    LoadData(0x00, { 0xCB, 0x46 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 1, <HL> (NZ)", "BIT-1,HL-NZ") {  // 0xCB, 0x4E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000010);
    LoadData(0x00, { 0xCB, 0x4E });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 1, <HL> (Z)", "BIT-1,HL-Z") {  // 0xCB, 0x4E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111101);
    LoadData(0x00, { 0xCB, 0x4E });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 2, <HL> (NZ)", "BIT-2,HL-NZ") {  // 0xCB, 0x56
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000100);
    LoadData(0x00, { 0xCB, 0x56 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 2, <HL> (Z)", "BIT-2,HL-Z") {  // 0xCB, 0x56
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111011);
    LoadData(0x00, { 0xCB, 0x56 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 3, <HL> (NZ)", "BIT-3,HL-NZ") {  // 0xCB, 0x5E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00001000);
    LoadData(0x00, { 0xCB, 0x5E });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 3, <HL> (Z)", "BIT-3,HL-Z") {  // 0xCB, 0x5E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11110111);
    LoadData(0x00, { 0xCB, 0x5E });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 4, <HL> (NZ)", "BIT-4,HL-NZ") {  // 0xCB, 0x66
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00010000);
    LoadData(0x00, { 0xCB, 0x66 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 4, <HL> (Z)", "BIT-4,HL-Z") {  // 0xCB, 0x66
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11101111);
    LoadData(0x00, { 0xCB, 0x66 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 5, <HL> (NZ)", "BIT-5,HL-NZ") {  // 0xCB, 0x6E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00100000);
    LoadData(0x00, { 0xCB, 0x6E });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 5, <HL> (Z)", "BIT-5,HL-Z") {  // 0xCB, 0x6E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11011111);
    LoadData(0x00, { 0xCB, 0x6E });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 6, <HL> (NZ)", "BIT-6,HL-NZ") {  // 0xCB, 0x76
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01000000);
    LoadData(0x00, { 0xCB, 0x76 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 6, <HL> (Z)", "BIT-6,HL-Z") {  // 0xCB, 0x76
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10111111);
    LoadData(0x00, { 0xCB, 0x76 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 7, <HL> (NZ)", "BIT-7,HL-NZ") {  // 0xCB, 0x7E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b10000000);
    LoadData(0x00, { 0xCB, 0x7E });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 7, <HL> (Z)", "BIT-7,HL-Z") {  // 0xCB, 0x7E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b01111111);
    LoadData(0x00, { 0xCB, 0x7E });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 3);
}

TEST("BIT 0, A (NZ)", "BIT-0,A-NZ") {  // 0xCB, 0x47
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000001);
    LoadData(0x00, { 0xCB, 0x47 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 0, A (Z)", "BIT-0,A-Z") {  // 0xCB, 0x47
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111110);
    LoadData(0x00, { 0xCB, 0x47 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, A (NZ)", "BIT-1,A-NZ") {  // 0xCB, 0x4F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000010);
    LoadData(0x00, { 0xCB, 0x4F });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 1, A (Z)", "BIT-1,A-Z") {  // 0xCB, 0x4F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111101);
    LoadData(0x00, { 0xCB, 0x4F });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, A (NZ)", "BIT-2,A-NZ") {  // 0xCB, 0x57
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000100);
    LoadData(0x00, { 0xCB, 0x57 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 2, A (Z)", "BIT-2,A-Z") {  // 0xCB, 0x57
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111011);
    LoadData(0x00, { 0xCB, 0x57 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, A (NZ)", "BIT-3,A-NZ") {  // 0xCB, 0x5F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00001000);
    LoadData(0x00, { 0xCB, 0x5F });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 3, A (Z)", "BIT-3,A-Z") {  // 0xCB, 0x5F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11110111);
    LoadData(0x00, { 0xCB, 0x5F });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, A (NZ)", "BIT-4,A-NZ") {  // 0xCB, 0x67
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00010000);
    LoadData(0x00, { 0xCB, 0x67 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 4, A (Z)", "BIT-4,A-Z") {  // 0xCB, 0x67
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11101111);
    LoadData(0x00, { 0xCB, 0x67 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, A (NZ)", "BIT-5,A-NZ") {  // 0xCB, 0x6F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00100000);
    LoadData(0x00, { 0xCB, 0x6F });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 5, A (Z)", "BIT-5,A-Z") {  // 0xCB, 0x6F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11011111);
    LoadData(0x00, { 0xCB, 0x6F });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, A (NZ)", "BIT-6,A-NZ") {  // 0xCB, 0x77
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01000000);
    LoadData(0x00, { 0xCB, 0x77 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 6, A (Z)", "BIT-6,A-Z") {  // 0xCB, 0x77
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10111111);
    LoadData(0x00, { 0xCB, 0x77 });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, A (NZ)", "BIT-7,A-NZ") {  // 0xCB, 0x7F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b10000000);
    LoadData(0x00, { 0xCB, 0x7F });

    const auto cycles = cpu->Step();
    CHECK_ZERO_CLEAR;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("BIT 7, A (Z)", "BIT-7,A-Z") {  // 0xCB, 0x7F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b01111111);
    LoadData(0x00, { 0xCB, 0x7F });

    const auto cycles = cpu->Step();
    CHECK_ZERO_SET;
    CHECK_SUBTRACT_CLEAR;
    CHECK_HALF_SET;
    CHECK(cycles == 2);
}

TEST("RES 0, B", "RES-0,B") {  // 0xCB, 0x80
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111111);
    LoadData(0x00, { 0xCB, 0x80 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b11111110);
    CHECK(cycles == 2);
}

TEST("RES 1, B", "RES-1,B") {  // 0xCB, 0x88
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111111);
    LoadData(0x00, { 0xCB, 0x88 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b11111101);
    CHECK(cycles == 2);
}

TEST("RES 2, B", "RES-2,B") {  // 0xCB, 0x90
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111111);
    LoadData(0x00, { 0xCB, 0x90 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b11111011);
    CHECK(cycles == 2);
}

TEST("RES 3, B", "RES-3,B") {  // 0xCB, 0x98
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111111);
    LoadData(0x00, { 0xCB, 0x98 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b11110111);
    CHECK(cycles == 2);
}

TEST("RES 4, B", "RES-4,B") {  // 0xCB, 0xA0
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA0 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b11101111);
    CHECK(cycles == 2);
}

TEST("RES 5, B", "RES-5,B") {  // 0xCB, 0xA8
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA8 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b11011111);
    CHECK(cycles == 2);
}

TEST("RES 6, B", "RES-6,B") {  // 0xCB, 0xB0
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB0 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10111111);
    CHECK(cycles == 2);
}

TEST("RES 7, B", "RES-7,B") {  // 0xCB, 0xB8
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB8 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01111111);
    CHECK(cycles == 2);
}

TEST("RES 0, C", "RES-0,C") {  // 0xCB, 0x81
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111111);
    LoadData(0x00, { 0xCB, 0x81 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b11111110);
    CHECK(cycles == 2);
}

TEST("RES 1, C", "RES-1,C") {  // 0xCB, 0x89
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111111);
    LoadData(0x00, { 0xCB, 0x89 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b11111101);
    CHECK(cycles == 2);
}

TEST("RES 2, C", "RES-2,C") {  // 0xCB, 0x91
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111111);
    LoadData(0x00, { 0xCB, 0x91 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b11111011);
    CHECK(cycles == 2);
}

TEST("RES 3, C", "RES-3,C") {  // 0xCB, 0x99
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111111);
    LoadData(0x00, { 0xCB, 0x99 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b11110111);
    CHECK(cycles == 2);
}

TEST("RES 4, C", "RES-4,C") {  // 0xCB, 0xA1
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA1 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b11101111);
    CHECK(cycles == 2);
}

TEST("RES 5, C", "RES-5,C") {  // 0xCB, 0xA9
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA9 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b11011111);
    CHECK(cycles == 2);
}

TEST("RES 6, C", "RES-6,C") {  // 0xCB, 0xB1
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB1 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10111111);
    CHECK(cycles == 2);
}

TEST("RES 7, C", "RES-7,C") {  // 0xCB, 0xB9
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB9 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01111111);
    CHECK(cycles == 2);
}

TEST("RES 0, D", "RES-0,D") {  // 0xCB, 0x82
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111111);
    LoadData(0x00, { 0xCB, 0x82 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b11111110);
    CHECK(cycles == 2);
}

TEST("RES 1, D", "RES-1,D") {  // 0xCB, 0x8A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111111);
    LoadData(0x00, { 0xCB, 0x8A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b11111101);
    CHECK(cycles == 2);
}

TEST("RES 2, D", "RES-2,D") {  // 0xCB, 0x92
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111111);
    LoadData(0x00, { 0xCB, 0x92 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b11111011);
    CHECK(cycles == 2);
}

TEST("RES 3, D", "RES-3,D") {  // 0xCB, 0x9A
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111111);
    LoadData(0x00, { 0xCB, 0x9A });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b11110111);
    CHECK(cycles == 2);
}

TEST("RES 4, D", "RES-4,D") {  // 0xCB, 0xA2
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA2 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b11101111);
    CHECK(cycles == 2);
}

TEST("RES 5, D", "RES-5,D") {  // 0xCB, 0xAA
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111111);
    LoadData(0x00, { 0xCB, 0xAA });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b11011111);
    CHECK(cycles == 2);
}

TEST("RES 6, D", "RES-6,D") {  // 0xCB, 0xB2
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB2 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10111111);
    CHECK(cycles == 2);
}

TEST("RES 7, D", "RES-7,D") {  // 0xCB, 0xBA
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b11111111);
    LoadData(0x00, { 0xCB, 0xBA });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01111111);
    CHECK(cycles == 2);
}

TEST("RES 0, E", "RES-0,E") {  // 0xCB, 0x83
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111111);
    LoadData(0x00, { 0xCB, 0x83 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b11111110);
    CHECK(cycles == 2);
}

TEST("RES 1, E", "RES-1,E") {  // 0xCB, 0x8B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111111);
    LoadData(0x00, { 0xCB, 0x8B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b11111101);
    CHECK(cycles == 2);
}

TEST("RES 2, E", "RES-2,E") {  // 0xCB, 0x93
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111111);
    LoadData(0x00, { 0xCB, 0x93 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b11111011);
    CHECK(cycles == 2);
}

TEST("RES 3, E", "RES-3,E") {  // 0xCB, 0x9B
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111111);
    LoadData(0x00, { 0xCB, 0x9B });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b11110111);
    CHECK(cycles == 2);
}

TEST("RES 4, E", "RES-4,E") {  // 0xCB, 0xA3
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA3 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b11101111);
    CHECK(cycles == 2);
}

TEST("RES 5, E", "RES-5,E") {  // 0xCB, 0xAB
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111111);
    LoadData(0x00, { 0xCB, 0xAB });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b11011111);
    CHECK(cycles == 2);
}

TEST("RES 6, E", "RES-6,E") {  // 0xCB, 0xB3
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB3 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10111111);
    CHECK(cycles == 2);
}

TEST("RES 7, E", "RES-7,E") {  // 0xCB, 0xBB
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b11111111);
    LoadData(0x00, { 0xCB, 0xBB });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01111111);
    CHECK(cycles == 2);
}

TEST("RES 0, H", "RES-0,H") {  // 0xCB, 0x84
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111111);
    LoadData(0x00, { 0xCB, 0x84 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b11111110);
    CHECK(cycles == 2);
}

TEST("RES 1, H", "RES-1,H") {  // 0xCB, 0x8C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111111);
    LoadData(0x00, { 0xCB, 0x8C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b11111101);
    CHECK(cycles == 2);
}

TEST("RES 2, H", "RES-2,H") {  // 0xCB, 0x94
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111111);
    LoadData(0x00, { 0xCB, 0x94 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b11111011);
    CHECK(cycles == 2);
}

TEST("RES 3, H", "RES-3,H") {  // 0xCB, 0x9C
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111111);
    LoadData(0x00, { 0xCB, 0x9C });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b11110111);
    CHECK(cycles == 2);
}

TEST("RES 4, H", "RES-4,H") {  // 0xCB, 0xA4
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA4 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b11101111);
    CHECK(cycles == 2);
}

TEST("RES 5, H", "RES-5,H") {  // 0xCB, 0xAC
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111111);
    LoadData(0x00, { 0xCB, 0xAC });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b11011111);
    CHECK(cycles == 2);
}

TEST("RES 6, H", "RES-6,H") {  // 0xCB, 0xB4
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB4 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10111111);
    CHECK(cycles == 2);
}

TEST("RES 7, H", "RES-7,H") {  // 0xCB, 0xBC
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b11111111);
    LoadData(0x00, { 0xCB, 0xBC });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01111111);
    CHECK(cycles == 2);
}

TEST("RES 0, L", "RES-0,L") {  // 0xCB, 0x85
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111111);
    LoadData(0x00, { 0xCB, 0x85 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b11111110);
    CHECK(cycles == 2);
}

TEST("RES 1, L", "RES-1,L") {  // 0xCB, 0x8D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111111);
    LoadData(0x00, { 0xCB, 0x8D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b11111101);
    CHECK(cycles == 2);
}

TEST("RES 2, L", "RES-2,L") {  // 0xCB, 0x95
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111111);
    LoadData(0x00, { 0xCB, 0x95 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b11111011);
    CHECK(cycles == 2);
}

TEST("RES 3, L", "RES-3,L") {  // 0xCB, 0x9D
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111111);
    LoadData(0x00, { 0xCB, 0x9D });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b11110111);
    CHECK(cycles == 2);
}

TEST("RES 4, L", "RES-4,L") {  // 0xCB, 0xA5
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA5 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b11101111);
    CHECK(cycles == 2);
}

TEST("RES 5, L", "RES-5,L") {  // 0xCB, 0xAD
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111111);
    LoadData(0x00, { 0xCB, 0xAD });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b11011111);
    CHECK(cycles == 2);
}

TEST("RES 6, L", "RES-6,L") {  // 0xCB, 0xB5
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB5 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10111111);
    CHECK(cycles == 2);
}

TEST("RES 7, L", "RES-7,L") {  // 0xCB, 0xBD
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b11111111);
    LoadData(0x00, { 0xCB, 0xBD });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01111111);
    CHECK(cycles == 2);
}

TEST("RES <HL>, 0", "RES-HL,0") {  // 0xCB, 0x86
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111111);
    LoadData(0x00, { 0xCB, 0x86 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b11111110);
    CHECK(cycles == 4);
}

TEST("RES <HL>, 1", "RES-HL,1") {  // 0xCB, 0x8E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111111);
    LoadData(0x00, { 0xCB, 0x8E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b11111101);
    CHECK(cycles == 4);
}

TEST("RES <HL>, 2", "RES-HL,2") {  // 0xCB, 0x96
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111111);
    LoadData(0x00, { 0xCB, 0x96 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b11111011);
    CHECK(cycles == 4);
}

TEST("RES <HL>, 3", "RES-HL,3") {  // 0xCB, 0x9E
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111111);
    LoadData(0x00, { 0xCB, 0x9E });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b11110111);
    CHECK(cycles == 4);
}

TEST("RES <HL>, 4", "RES-HL,4") {  // 0xCB, 0xA6
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA6 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b11101111);
    CHECK(cycles == 4);
}

TEST("RES <HL>, 5", "RES-HL,5") {  // 0xCB, 0xAE
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111111);
    LoadData(0x00, { 0xCB, 0xAE });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b11011111);
    CHECK(cycles == 4);
}

TEST("RES <HL>, 6", "RES-HL,6") {  // 0xCB, 0xB6
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB6 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10111111);
    CHECK(cycles == 4);
}

TEST("RES <HL>, 7", "RES-HL,7") {  // 0xCB, 0xBE
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b11111111);
    LoadData(0x00, { 0xCB, 0xBE });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01111111);
    CHECK(cycles == 4);
}

TEST("RES 0, A", "RES-0,A") {  // 0xCB, 0x87
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    LoadData(0x00, { 0xCB, 0x87 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11111110);
    CHECK(cycles == 2);
}

TEST("RES 1, A", "RES-1,A") {  // 0xCB, 0x8F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    LoadData(0x00, { 0xCB, 0x8F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11111101);
    CHECK(cycles == 2);
}

TEST("RES 2, A", "RES-2,A") {  // 0xCB, 0x97
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    LoadData(0x00, { 0xCB, 0x97 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11111011);
    CHECK(cycles == 2);
}

TEST("RES 3, A", "RES-3,A") {  // 0xCB, 0x9F
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    LoadData(0x00, { 0xCB, 0x9F });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11110111);
    CHECK(cycles == 2);
}

TEST("RES 4, A", "RES-4,A") {  // 0xCB, 0xA7
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    LoadData(0x00, { 0xCB, 0xA7 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11101111);
    CHECK(cycles == 2);
}

TEST("RES 5, A", "RES-5,A") {  // 0xCB, 0xAF
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    LoadData(0x00, { 0xCB, 0xAF });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b11011111);
    CHECK(cycles == 2);
}

TEST("RES 6, A", "RES-6,A") {  // 0xCB, 0xB7
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    LoadData(0x00, { 0xCB, 0xB7 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10111111);
    CHECK(cycles == 2);
}

TEST("RES 7, A", "RES-7,A") {  // 0xCB, 0xBF
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b11111111);
    LoadData(0x00, { 0xCB, 0xBF });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01111111);
    CHECK(cycles == 2);
}

TEST("SET 0, B", "SET-0,B") {  // 0xCB, 0xC0
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC0 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000001);
    CHECK(cycles == 2);
}

TEST("SET 1, B", "SET-1,B") {  // 0xCB, 0xC8
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC8 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000010);
    CHECK(cycles == 2);
}

TEST("SET 2, B", "SET-2,B") {  // 0xCB, 0xD0
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD0 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00000100);
    CHECK(cycles == 2);
}

TEST("SET 3, B", "SET-3,B") {  // 0xCB, 0xD8
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD8 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00001000);
    CHECK(cycles == 2);
}

TEST("SET 4, B", "SET-4,B") {  // 0xCB, 0xE0
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE0 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00010000);
    CHECK(cycles == 2);
}

TEST("SET 5, B", "SET-5,B") {  // 0xCB, 0xE8
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE8 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b00100000);
    CHECK(cycles == 2);
}

TEST("SET 6, B", "SET-6,B") {  // 0xCB, 0xF0
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF0 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b01000000);
    CHECK(cycles == 2);
}

TEST("SET 7, B", "SET-7,B") {  // 0xCB, 0xF8
    cpu->SetRegister8ByIndex(cpu->RegIndexB, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF8 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexB) == 0b10000000);
    CHECK(cycles == 2);
}

TEST("SET 0, C", "SET-0,C") {  // 0xCB, 0xC1
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC1 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000001);
    CHECK(cycles == 2);
}

TEST("SET 1, C", "SET-1,C") {  // 0xCB, 0xC9
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC9 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000010);
    CHECK(cycles == 2);
}

TEST("SET 2, C", "SET-2,C") {  // 0xCB, 0xD1
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD1 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00000100);
    CHECK(cycles == 2);
}

TEST("SET 3, C", "SET-3,C") {  // 0xCB, 0xD9
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD9 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00001000);
    CHECK(cycles == 2);
}

TEST("SET 4, C", "SET-4,C") {  // 0xCB, 0xE1
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE1 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00010000);
    CHECK(cycles == 2);
}

TEST("SET 5, C", "SET-5,C") {  // 0xCB, 0xE9
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE9 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b00100000);
    CHECK(cycles == 2);
}

TEST("SET 6, C", "SET-6,C") {  // 0xCB, 0xF1
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF1 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b01000000);
    CHECK(cycles == 2);
}

TEST("SET 7, C", "SET-7,C") {  // 0xCB, 0xF9
    cpu->SetRegister8ByIndex(cpu->RegIndexC, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF9 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexC) == 0b10000000);
    CHECK(cycles == 2);
}

TEST("SET 0, D", "SET-0,D") {  // 0xCB, 0xC2
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC2 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000001);
    CHECK(cycles == 2);
}

TEST("SET 1, D", "SET-1,D") {  // 0xCB, 0xCA
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0xCA });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000010);
    CHECK(cycles == 2);
}

TEST("SET 2, D", "SET-2,D") {  // 0xCB, 0xD2
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD2 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00000100);
    CHECK(cycles == 2);
}

TEST("SET 3, D", "SET-3,D") {  // 0xCB, 0xDA
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0xDA });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00001000);
    CHECK(cycles == 2);
}

TEST("SET 4, D", "SET-4,D") {  // 0xCB, 0xE2
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE2 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00010000);
    CHECK(cycles == 2);
}

TEST("SET 5, D", "SET-5,D") {  // 0xCB, 0xEA
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0xEA });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b00100000);
    CHECK(cycles == 2);
}

TEST("SET 6, D", "SET-6,D") {  // 0xCB, 0xF2
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF2 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b01000000);
    CHECK(cycles == 2);
}

TEST("SET 7, D", "SET-7,D") {  // 0xCB, 0xFA
    cpu->SetRegister8ByIndex(cpu->RegIndexD, 0b00000000);
    LoadData(0x00, { 0xCB, 0xFA });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexD) == 0b10000000);
    CHECK(cycles == 2);
}

TEST("SET 0, E", "SET-0,E") {  // 0xCB, 0xC3
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC3 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000001);
    CHECK(cycles == 2);
}

TEST("SET 1, E", "SET-1,E") {  // 0xCB, 0xCB
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0xCB });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000010);
    CHECK(cycles == 2);
}

TEST("SET 2, E", "SET-2,E") {  // 0xCB, 0xD3
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD3 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00000100);
    CHECK(cycles == 2);
}

TEST("SET 3, E", "SET-3,E") {  // 0xCB, 0xDB
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0xDB });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00001000);
    CHECK(cycles == 2);
}

TEST("SET 4, E", "SET-4,E") {  // 0xCB, 0xE3
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE3 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00010000);
    CHECK(cycles == 2);
}

TEST("SET 5, E", "SET-5,E") {  // 0xCB, 0xEB
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0xEB });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b00100000);
    CHECK(cycles == 2);
}

TEST("SET 6, E", "SET-6,E") {  // 0xCB, 0xF3
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF3 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b01000000);
    CHECK(cycles == 2);
}

TEST("SET 7, E", "SET-7,E") {  // 0xCB, 0xFB
    cpu->SetRegister8ByIndex(cpu->RegIndexE, 0b00000000);
    LoadData(0x00, { 0xCB, 0xFB });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexE) == 0b10000000);
    CHECK(cycles == 2);
}

TEST("SET 0, H", "SET-0,H") {  // 0xCB, 0xC4
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC4 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000001);
    CHECK(cycles == 2);
}

TEST("SET 1, H", "SET-1,H") {  // 0xCB, 0xCC
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0xCC });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000010);
    CHECK(cycles == 2);
}

TEST("SET 2, H", "SET-2,H") {  // 0xCB, 0xD4
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD4 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00000100);
    CHECK(cycles == 2);
}

TEST("SET 3, H", "SET-3,H") {  // 0xCB, 0xDC
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0xDC });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00001000);
    CHECK(cycles == 2);
}

TEST("SET 4, H", "SET-4,H") {  // 0xCB, 0xE4
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE4 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00010000);
    CHECK(cycles == 2);
}

TEST("SET 5, H", "SET-5,H") {  // 0xCB, 0xEC
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0xEC });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b00100000);
    CHECK(cycles == 2);
}

TEST("SET 6, H", "SET-6,H") {  // 0xCB, 0xF4
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF4 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b01000000);
    CHECK(cycles == 2);
}

TEST("SET 7, H", "SET-7,H") {  // 0xCB, 0xFC
    cpu->SetRegister8ByIndex(cpu->RegIndexH, 0b00000000);
    LoadData(0x00, { 0xCB, 0xFC });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexH) == 0b10000000);
    CHECK(cycles == 2);
}

TEST("SET 0, L", "SET-0,L") {  // 0xCB, 0xC5
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC5 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000001);
    CHECK(cycles == 2);
}

TEST("SET 1, L", "SET-1,L") {  // 0xCB, 0xCD
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0xCD });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000010);
    CHECK(cycles == 2);
}

TEST("SET 2, L", "SET-2,L") {  // 0xCB, 0xD5
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD5 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00000100);
    CHECK(cycles == 2);
}

TEST("SET 3, L", "SET-3,L") {  // 0xCB, 0xDD
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0xDD });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00001000);
    CHECK(cycles == 2);
}

TEST("SET 4, L", "SET-4,L") {  // 0xCB, 0xE5
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE5 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00010000);
    CHECK(cycles == 2);
}

TEST("SET 5, L", "SET-5,L") {  // 0xCB, 0xED
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0xED });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b00100000);
    CHECK(cycles == 2);
}

TEST("SET 6, L", "SET-6,L") {  // 0xCB, 0xF5
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF5 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b01000000);
    CHECK(cycles == 2);
}

TEST("SET 7, L", "SET-7,L") {  // 0xCB, 0xFD
    cpu->SetRegister8ByIndex(cpu->RegIndexL, 0b00000000);
    LoadData(0x00, { 0xCB, 0xFD });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexL) == 0b10000000);
    CHECK(cycles == 2);
}

TEST("SET <HL>, 0", "SET-HL,0") {  // 0xCB, 0xC6
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC6 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000001);
    CHECK(cycles == 4);
}

TEST("SET <HL>, 1", "SET-HL,1") {  // 0xCB, 0xCE
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0xCE });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000010);
    CHECK(cycles == 4);
}

TEST("SET <HL>, 2", "SET-HL,2") {  // 0xCB, 0xD6
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD6 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00000100);
    CHECK(cycles == 4);
}

TEST("SET <HL>, 3", "SET-HL,3") {  // 0xCB, 0xDE
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0xDE });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00001000);
    CHECK(cycles == 4);
}

TEST("SET <HL>, 4", "SET-HL,4") {  // 0xCB, 0xE6
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE6 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00010000);
    CHECK(cycles == 4);
}

TEST("SET <HL>, 5", "SET-HL,5") {  // 0xCB, 0xEE
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0xEE });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b00100000);
    CHECK(cycles == 4);
}

TEST("SET <HL>, 6", "SET-HL,6") {  // 0xCB, 0xF6
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF6 });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b01000000);
    CHECK(cycles == 4);
}

TEST("SET <HL>, 7", "SET-HL,7") {  // 0xCB, 0xFE
    cpu->SetHl(0x100);
    memory->SetByte(0x100, 0b00000000);
    LoadData(0x00, { 0xCB, 0xFE });

    const auto cycles = cpu->Step();
    CHECK(memory->GetByte(0x100) == 0b10000000);
    CHECK(cycles == 4);
}

TEST("SET 0, A", "SET-0,A") {  // 0xCB, 0xC7
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0xC7 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000001);
    CHECK(cycles == 2);
}

TEST("SET 1, A", "SET-1,A") {  // 0xCB, 0xCF
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0xCF });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000010);
    CHECK(cycles == 2);
}

TEST("SET 2, A", "SET-2,A") {  // 0xCB, 0xD7
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0xD7 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00000100);
    CHECK(cycles == 2);
}

TEST("SET 3, A", "SET-3,A") {  // 0xCB, 0xDF
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0xDF });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00001000);
    CHECK(cycles == 2);
}

TEST("SET 4, A", "SET-4,A") {  // 0xCB, 0xE7
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0xE7 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00010000);
    CHECK(cycles == 2);
}

TEST("SET 5, A", "SET-5,A") {  // 0xCB, 0xEF
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0xEF });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b00100000);
    CHECK(cycles == 2);
}

TEST("SET 6, A", "SET-6,A") {  // 0xCB, 0xF7
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0xF7 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b01000000);
    CHECK(cycles == 2);
}

TEST("SET 7, A", "SET-7,A") {  // 0xCB, 0xFF
    cpu->SetRegister8ByIndex(cpu->RegIndexA, 0b00000000);
    LoadData(0x00, { 0xCB, 0xFF });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetRegister8ByIndex(cpu->RegIndexA) == 0b10000000);
    CHECK(cycles == 2);
}
