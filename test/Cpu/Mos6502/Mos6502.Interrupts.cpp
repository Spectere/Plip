/* Mos6502.Interrupts.cpp
 *
 * Tests the CPU core's ability to respond to maskable and non-maskable interrupts.
 */

#include "catch2/catch_test_macros.hpp"

#include "Mos6502Common.h"

//
// IRQ
//
TEST("IRQ (Interrupts Enabled)", "IRQ-IntEnabled") {
    constexpr int expectedCycles = 7;

    LoadData(0xFFFE, { 0x20, 0x30 });
    LoadData(0x200, 0x02);  // KIL

    cpu->ClearInterruptDisable();
    cpu->SetBreakCommand();
    cpu->RaiseInterrupt();

    EXECUTE(expectedCycles);
    CHECK_FALSE(cpu->CpuHasCrashed());
    CHECK_PC(0x3020);
    CHECK_BREAK_CLEAR;
}

TEST("IRQ (Interrupts Disabled)", "IRQ-IntDisabled") {
    LoadData(0xFFFE, { 0x20, 0x30 });
    LoadData(0x200, 0x02);  // KIL

    cpu->SetInterruptDisable();
    cpu->RaiseInterrupt();

    cpu->Step();
    CHECK(cpu->CpuHasCrashed());
    CHECK_PC(0x0201);
}

//
// NMI
//
TEST("NMI (Interrupts Enabled)", "NMI-IntEnabled") {
    constexpr int expectedCycles = 7;

    LoadData(0xFFFA, { 0x20, 0x30 });
    LoadData(0x200, { 0xEA, 0x02 });  // NOP; KIL

    cpu->ClearInterruptDisable();
    cpu->SetBreakCommand();
    cpu->FlagNmi();
    cpu->Cycle();  // NOP (2 cycles)
    cpu->Cycle();  // The end of this cycle should signal for the CPU to jump to the NMI vector.

    EXECUTE(expectedCycles);
    CHECK_FALSE(cpu->CpuHasCrashed());
    CHECK_PC(0x3020);
    CHECK_BREAK_CLEAR;
}

TEST("NMI (Interrupts Disabled)", "NMI-IntDisabled") {
    constexpr int expectedCycles = 7;

    LoadData(0xFFFA, { 0x20, 0x30 });
    LoadData(0x200, { 0xEA, 0x02 });  // NOP; KIL

    cpu->SetInterruptDisable();
    cpu->SetBreakCommand();
    cpu->FlagNmi();
    cpu->Cycle();  // NOP (2 cycles)
    cpu->Cycle();  // The end of this cycle should signal for the CPU to jump to the NMI vector.

    EXECUTE(expectedCycles);
    CHECK_FALSE(cpu->CpuHasCrashed());
    CHECK_PC(0x3020);
    CHECK_BREAK_CLEAR;
}
