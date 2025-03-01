/* SharpLr35902.Bits.cpp
 *
 * Tests the CPU core's support for miscellaneous/control instructions.
 */

#include "catch2/catch_test_macros.hpp"

#include "SharpLr35902Common.h"

TEST("NOP", "NOP") {
    // Brace yourself...
    const auto cycles = cpu->Step();
    CHECK(cycles == 1);
}

TEST("HALT (IME+)", "HALT-IME-E") {
    LoadData(0x00, { 0x76 });

    const auto cycles = cpu->Step();
    CHECK(cycles == 1);
    CHECK(cpu->GetHalt() == true);

    // If we run additional cycles, the PC should remain at 0x01.
    cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
    cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
}

TEST("HALT (IME~)", "HALT-IME-P") {
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);
    LoadData(0x00, { 0x76 });

    const auto cycles = cpu->Step();
    CHECK(cycles == 1);
    CHECK(cpu->GetHalt() == true);

    // If we run additional cycles, the PC should remain at 0x01.
    cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
    cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
}

TEST("HALT (IME-)", "HALT-IME-D") {
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);
    LoadData(0x00, { 0x76 });

    const auto cycles = cpu->Step();
    CHECK(cycles == 1);
    CHECK(cpu->GetHalt() == true);

    // If we run additional cycles, the PC should remain at 0x01.
    cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
    cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
}

TEST("HALT Bug (IME-)", "HALT-Bug-IME-D") {
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);
    memory->SetByte(0xFF0F, 0b00011100);
    memory->SetByte(0xFFFF, 0b00000111);
    LoadData(0x00, { 0x76 });

    const auto cycles = cpu->Step();
    CHECK(cycles == 1);

    // The PC should remain at 0x01 after the next cycle, then increment to
    // 0x02 after an additional cycle.
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == false);
    cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == false);
    cpu->Step();
    CHECK(cpu->GetPc() == 0x02);
    CHECK(cpu->GetHalt() == false);
}

TEST("HALT Bug (IME~)", "HALT-Bug-IME-P") {
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);
    cpu->SetSp(0x200);
    memory->SetByte(0xFF0F, 0b00000001);
    memory->SetByte(0xFFFF, 0b00000001);
    LoadData(0x00, { 0x76 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetPc() == 0x40);
    CHECK(memory->GetByte(0x1FE) == 0x00);
    CHECK(memory->GetByte(0x1FF) == 0x00);
    CHECK(cycles == 5);
}

TEST("HALT Bug (RST)", "HALT-Bug-RST") {
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);
    cpu->SetSp(0x200);
    memory->SetByte(0xFF0F, 0b00000001);
    memory->SetByte(0xFFFF, 0b00000001);
    LoadData(0x00, { 0x76, 0xD7 });

    const auto cycles = cpu->Step();
    CHECK(cycles == 1);
    CHECK(cpu->GetPc() == 0x01);   // PC is on RST $10
    cpu->Step();                  // This should execute RST $10.

    // The address of RST $10 should be on the stack, and PC should be 0x10.
    CHECK(memory->GetByte(0x1FF) == 0x00);
    CHECK(memory->GetByte(0x1FE) == 0x01);
    CHECK(cpu->GetPc() == 0x10);
}

TEST("DI", "DI") {
    LoadData(0x00, { 0xF3 });

    const auto cycles = cpu->Step();
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cycles == 1);
}

TEST("EI", "EI") {
    LoadData(0x00, { 0xFB });

    // Check for pending enable...
    const auto cycles = cpu->Step();
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::PendingEnable);
    CHECK(cycles == 1);

    // Interrupts should be fully enabled after a NOP.
    cpu->Step();
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
}

TEST("EI (Multiple)", "EI-multiple") {
    LoadData(0x00, { 0xFB, 0xFB, 0xFB });

    // Check for pending enable...
    const auto cycles = cpu->Step();
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::PendingEnable);
    CHECK(cycles == 1);

    // Repeatedly check for enabled.
    cpu->Step();
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    cpu->Step();
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
}

