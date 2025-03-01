/* SharpLr35902.Bits.cpp
 *
 * Tests the CPU core's ability to service interrupts.
 */

#include "catch2/catch_test_macros.hpp"

#include "SharpLr35902Common.h"

TEST("Interrupt VBlank (Running, IME+)", "IntVBlank-Running,IME-E") {
    memory->SetByte(0xFFFF, 0b00011111);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x40);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011110);
    CHECK(cycles == 5);
}

TEST("Interrupt VBlank (Running, IME~)", "IntVBlank-Running,IME-P") {
    memory->SetByte(0xFFFF, 0b00011111);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt VBlank (Running, IME-)", "IntVBlank-Running,IME-D") {
    memory->SetByte(0xFFFF, 0b00011111);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt VBlank (Halt, IME+)", "IntVBlank-Halt,IME-E") {
    memory->SetByte(0xFFFF, 0b00011111);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x40);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011110);
    CHECK(cycles == 5);
}

TEST("Interrupt VBlank (Halt, IME~)", "IntVBlank-Halt,IME-P") {
    memory->SetByte(0xFFFF, 0b00011111);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt VBlank (Halt, IME-)", "IntVBlank-Halt,IME-D") {
    memory->SetByte(0xFFFF, 0b00011111);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt LCD (Running, IME+)", "IntLCD-Running,IME-E") {
    memory->SetByte(0xFFFF, 0b00011110);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x48);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011101);
    CHECK(cycles == 5);
}

TEST("Interrupt LCD (Running, IME~)", "IntLCD-Running,IME-P") {
    memory->SetByte(0xFFFF, 0b00011110);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt LCD (Running, IME-)", "IntLCD-Running,IME-D") {
    memory->SetByte(0xFFFF, 0b00011110);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt LCD (Halt, IME+)", "IntLCD-Halt,IME-E") {
    memory->SetByte(0xFFFF, 0b00011110);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x48);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011101);
    CHECK(cycles == 5);
}

TEST("Interrupt LCD (Halt, IME~)", "IntLCD-Halt,IME-P") {
    memory->SetByte(0xFFFF, 0b00011110);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt LCD (Halt, IME-)", "IntLCD-Halt,IME-D") {
    memory->SetByte(0xFFFF, 0b00011110);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Timer (Running, IME+)", "IntTimer-Running,IME-E") {
    memory->SetByte(0xFFFF, 0b00011100);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x50);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011011);
    CHECK(cycles == 5);
}

TEST("Interrupt Timer (Running, IME~)", "IntTimer-Running,IME-P") {
    memory->SetByte(0xFFFF, 0b00011100);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Timer (Running, IME-)", "IntTimer-Running,IME-D") {
    memory->SetByte(0xFFFF, 0b00011100);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Timer (Halt, IME+)", "IntTimer-Halt,IME-E") {
    memory->SetByte(0xFFFF, 0b00011100);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x50);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011011);
    CHECK(cycles == 5);
}

TEST("Interrupt Timer (Halt, IME~)", "IntTimer-Halt,IME-P") {
    memory->SetByte(0xFFFF, 0b00011100);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Timer (Halt, IME-)", "IntTimer-Halt,IME-D") {
    memory->SetByte(0xFFFF, 0b00011100);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Serial (Running, IME+)", "IntSerial-Running,IME-E") {
    memory->SetByte(0xFFFF, 0b00011000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x58);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00010111);
    CHECK(cycles == 5);
}

TEST("Interrupt Serial (Running, IME~)", "IntSerial-Running,IME-P") {
    memory->SetByte(0xFFFF, 0b00011000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Serial (Running, IME-)", "IntSerial-Running,IME-D") {
    memory->SetByte(0xFFFF, 0b00011000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Serial (Halt, IME+)", "IntSerial-Halt,IME-E") {
    memory->SetByte(0xFFFF, 0b00011000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x58);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00010111);
    CHECK(cycles == 5);
}

TEST("Interrupt Serial (Halt, IME~)", "IntSerial-Halt,IME-P") {
    memory->SetByte(0xFFFF, 0b00011000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Serial (Halt, IME-)", "IntSerial-Halt,IME-D") {
    memory->SetByte(0xFFFF, 0b00011000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Joypad (Running, IME+)", "IntJoypad-Running,IME-E") {
    memory->SetByte(0xFFFF, 0b00010000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x60);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00001111);
    CHECK(cycles == 5);
}

TEST("Interrupt Joypad (Running, IME~)", "IntJoypad-Running,IME-P") {
    memory->SetByte(0xFFFF, 0b00010000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Joypad (Running, IME-)", "IntJoypad-Running,IME-D") {
    memory->SetByte(0xFFFF, 0b00010000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Joypad (Halt, IME+)", "IntJoypad-Halt,IME-E") {
    memory->SetByte(0xFFFF, 0b00010000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Enabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x60);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00001111);
    CHECK(cycles == 5);
}

TEST("Interrupt Joypad (Halt, IME~)", "IntJoypad-Halt,IME-P") {
    memory->SetByte(0xFFFF, 0b00010000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::PendingEnable);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Enabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}

TEST("Interrupt Joypad (Halt, IME-)", "IntJoypad-Halt,IME-D") {
    memory->SetByte(0xFFFF, 0b00010000);
    memory->SetByte(0xFF0F, 0b00011111);
    cpu->SetImeState(Cpu::SharpLr35902ImeState::Disabled);
    cpu->SetHalt(true);

    const auto cycles = cpu->Step();
    CHECK(cpu->GetPc() == 0x00);
    CHECK(cpu->GetImeState() == Cpu::SharpLr35902ImeState::Disabled);
    CHECK(cpu->GetHalt() == false);
    CHECK(memory->GetByte(0xFF0F) == 0b00011111);
    CHECK(cycles == 1);
}
