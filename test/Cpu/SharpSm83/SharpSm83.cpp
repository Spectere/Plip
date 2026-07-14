/* SharpSm83.Bits.cpp
 *
 * Tests the CPU core's support for miscellaneous/control instructions.
 */

#include "catch2/catch_test_macros.hpp"

#include "PlipEmulationException.h"

#include "SharpSm83Common.h"

TEST("STOP (CGB)", "STOP-CGB") {
    // On CGB, STOP is used to change the CPU speed. We can't test the speed change routine here,
    // but we can ensure that the call does *not* throw.
    cpu->SetGbcMode(true);

    LoadData(0x00, 0x10);
    REQUIRE_NOTHROW(cpu->Cycle());
}

TEST("STOP (DMG)", "STOP-DMG") {
    // On DMG, STOP halts the CPU. The CPU core throws an exception as execution cannot continue.
    LoadData(0x00, 0x10);
    REQUIRE_THROWS_AS(cpu->Cycle(), PlipEmulationException);
}

TEST("HALT (IME+)", "HALT-IME-E") {
    LoadData(0x00, 0x76);

    EXECUTE(1);
    CHECK(cpu->GetHalt() == true);

    // If we run additional cycles, the PC should remain at 0x01.
    cpu->Cycle();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
    cpu->Cycle();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
}

TEST("HALT (IME~)", "HALT-IME-P") {
    cpu->SetImePending(true);
    LoadData(0x00, 0x76);

    EXECUTE(1);
    CHECK(cpu->GetHalt() == true);

    // If we run additional cycles, the PC should remain at 0x01.
    cpu->Cycle();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
    cpu->Cycle();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
}

TEST("HALT (IME-)", "HALT-IME-D") {
    cpu->SetImeEnabled(false);
    LoadData(0x00, 0x76);

    EXECUTE(1);
    CHECK(cpu->GetHalt() == true);

    // If we run additional cycles, the PC should remain at 0x01.
    cpu->Cycle();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
    cpu->Cycle();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == true);
}

TEST("HALT Bug (IME-)", "HALT-Bug-IME-D") {
    cpu->SetImeEnabled(false);
    memory->SetByte(0xFF0F, 0b00011100);
    memory->SetByte(0xFFFF, 0b00000111);
    LoadData(0x00, 0x76);

    EXECUTE(1);

    // The PC should remain at 0x01 after the next cycle, then increment to
    // 0x02 after an additional cycle.
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == false);
    cpu->Cycle();
    CHECK(cpu->GetPc() == 0x01);
    CHECK(cpu->GetHalt() == false);
    cpu->Cycle();
    CHECK(cpu->GetPc() == 0x02);
    CHECK(cpu->GetHalt() == false);
}

TEST("HALT Bug (IME~)", "HALT-Bug-IME-P") {
    cpu->SetImePending(true);
    cpu->SetSp(0x200);
    memory->SetByte(0xFF0F, 0b00000001);
    memory->SetByte(0xFFFF, 0b00000001);
    LoadData(0x00, 0x76);

    EXECUTE(5);
    CHECK(cpu->GetImeEnabled() == false);
    CHECK(cpu->GetPc() == 0x40);
    CHECK(memory->GetByte(0x1FE) == 0x00);
    CHECK(memory->GetByte(0x1FF) == 0x00);
}

TEST("HALT Bug (RST)", "HALT-Bug-RST") {
    cpu->SetImeEnabled(false);
    cpu->SetSp(0x200);
    memory->SetByte(0xFF0F, 0b00000001);
    memory->SetByte(0xFFFF, 0b00000001);
    LoadData(0x00, { 0x76, 0xD7 });

    EXECUTE(1);
    CHECK(cpu->GetPc() == 0x01);   // PC is on RST $10
    cpu->Cycle();                  // This should execute RST $10.

    // The address of RST $10 should be on the stack, and PC should be 0x10.
    CHECK(memory->GetByte(0x1FF) == 0x00);
    CHECK(memory->GetByte(0x1FE) == 0x01);
    CHECK(cpu->GetPc() == 0x10);
}

TEST("DI", "DI") {
    cpu->SetImeEnabled(true);
    LoadData(0x00, 0xF3);

    EXECUTE(1);
    CHECK(cpu->GetImeEnabled() == false);
}

TEST("EI", "EI") {
    LoadData(0x00, 0xFB);

    // Check for pending enable...
    EXECUTE(1);
    CHECK(cpu->GetImePending() == true);

    // Interrupts should be fully enabled after a NOP.
    cpu->Cycle();
    cpu->Cycle();
    CHECK(cpu->GetImeEnabled() == true);
}

TEST("EI (Multiple)", "EI-multiple") {
    LoadData(0x00, { 0xFB, 0xFB, 0xFB });

    // Check for pending enable...
    EXECUTE(1);
    CHECK(cpu->GetImePending() == true);

    // Repeatedly check for enabled.
    cpu->Cycle();
    cpu->Cycle();
    CHECK(cpu->GetImeEnabled() == true);
    cpu->Cycle();
    CHECK(cpu->GetImeEnabled() == true);
}

TEST("RET", "RET") {  // 0xC9
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, 0xC9);
    cpu->SetSp(0x1FE);

    EXECUTE(4);
    CHECK(cpu->GetPc() == 0x100);
}

TEST("RETI", "RETI") {  // 0xD9
    LoadData(0x1FE, { 0x00, 0x01 });
    LoadData(0x00, 0xD9);
    cpu->SetSp(0x1FE);
    cpu->SetImeEnabled(false);

    EXECUTE(4);
    CHECK(cpu->GetPc() == 0x100);
    CHECK(cpu->GetImeEnabled() == true);
}
