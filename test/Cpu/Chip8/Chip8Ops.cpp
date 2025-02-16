// Chip8Ops.cpp
//
// Unit tests for the CHIP-8 CPU.

#include <PlipEmulationException.h>

#include "catch2/catch_test_macros.hpp"

#include "Cpu/Chip8/Chip8.h"
#include "Core/Chip8/Chip8Instance.h"

#include "../MockCpu.h"

using namespace Plip;

#define TEST_NAME(name) "[CHIP-8 CPU] " name
#define OP(op) "[chip8cpu][" op "]"
#define TEST(name, op) TEST_CASE_METHOD(MockCpu<Cpu::Chip8>, TEST_NAME(name), OP(op))

#define REG(reg) cpu->GetRegisters()[reg].ValueInt

constexpr uint16_t InitialPc = 0x200;

constexpr uint16_t LoadRegistersSubroutineAddress = 0xF00;
constexpr uint8_t MockV0 = 0x01;
constexpr uint8_t MockV1 = 0x11;
constexpr uint8_t MockV2 = 0x22;
constexpr uint8_t MockV3 = 0x33;
constexpr uint8_t MockV4 = 0x45;
constexpr uint8_t MockV5 = 0x55;
constexpr uint8_t MockV6 = 0x66;
constexpr uint8_t MockV7 = 0x77;
constexpr uint8_t MockV8 = 0x88;
constexpr uint8_t MockV9 = 0x99;
constexpr uint8_t MockVA = 0xAA;
constexpr uint8_t MockVB = 0xBB;
constexpr uint8_t MockVC = 0xCC;
constexpr uint8_t MockVD = 0xDD;
constexpr uint8_t MockVE = 0xEE;
constexpr uint8_t MockVF = 0xFF;

// Requires CALL/RET support to function.
void CreateLoadRegistersRoutine(const MockCpu<Cpu::Chip8>* fixture) {
    fixture->LoadData(LoadRegistersSubroutineAddress, {
        0x60, MockV0,  // V0 = 0x01
        0x61, MockV1,  // V1 = 0x11
        0x62, MockV2,  // V2 = 0x22
        0x63, MockV3,  // V3 = 0x33
        0x64, MockV4,  // V4 = 0x45
        0x65, MockV5,  // V5 = 0x55
        0x66, MockV6,  // V6 = 0x66
        0x67, MockV7,  // V7 = 0x77
        0x68, MockV8,  // V8 = 0x88
        0x69, MockV9,  // V9 = 0x99
        0x6A, MockVA,  // VA = 0xAA
        0x6B, MockVB,  // VB = 0xBB
        0x6C, MockVC,  // VC = 0xCC
        0x6D, MockVD,  // VD = 0xDD
        0x6E, MockVE,  // VE = 0xEE
        0x6F, MockVF,  // VF = 0xFF
        0x00, 0xEE      // Return
    });
}

void DoLoadAllRegisters(const MockCpu<Cpu::Chip8>* fixture) {
    CreateLoadRegistersRoutine(fixture);
    fixture->LoadData(InitialPc, { 0x2F, 00 });

    fixture->cpu->Cycle();  // Jump into subroutine.
    CHECK(fixture->cpu->GetPc() == LoadRegistersSubroutineAddress);

    while(fixture->REG("SP") == 1) {
        fixture->cpu->Cycle();
    }
    CHECK(fixture->cpu->GetPc() == InitialPc + 2);
}

void Execute(const MockCpu<Cpu::Chip8>* fixture, const std::vector<uint8_t> &data) {
    constexpr auto cycleLimit = 1000;

    fixture->LoadData(InitialPc, data);
    fixture->cpu->Reset(InitialPc);
    int cycles = 0;
    while(fixture->cpu->GetPc() != InitialPc + data.size()) {
        fixture->cpu->Cycle();
        if(++cycles >= cycleLimit) {
            throw std::runtime_error("Infinite loop detected.");
        }
    }
}

TEST("Stack underflow", "RET-underflow") {  // 00EE
    LoadData(InitialPc, { 0x00, 0xEE });
    cpu->Reset(InitialPc);
    REQUIRE_THROWS(cpu->Cycle());  // Stack underflow.
}

TEST("Call and return from subroutine", "CALL/RET") {  // 0x2nnn, 0x00EE
    LoadData(InitialPc, { 0x23, 0x00 });  // Jump to 0x300
    LoadData(0x300, { 0x00, 0xEE });            // Return from subroutine
    cpu->Reset(InitialPc);
    cpu->Cycle();
    REQUIRE(cpu->GetPc() == 0x300);  // Jump successful
    REQUIRE(REG("SP") == 1);  // Stack pointer incremented.
    cpu->Cycle();
    REQUIRE(cpu->GetPc() == InitialPc + 2);  // Return successful
    REQUIRE(REG("SP") == 0);  // Stack pointer decremented.
}

TEST("Stack overflow", "CALL-overflow") {  // 0x2nnn
    LoadData(InitialPc, { 0x22, 0x10 });
    LoadData(0x210, { 0x22, 0x20 });
    LoadData(0x220, { 0x22, 0x30 });
    LoadData(0x230, { 0x22, 0x40 });
    LoadData(0x240, { 0x22, 0x50 });
    LoadData(0x250, { 0x22, 0x60 });
    LoadData(0x260, { 0x22, 0x70 });
    LoadData(0x270, { 0x22, 0x80 });
    LoadData(0x280, { 0x22, 0x90 });
    LoadData(0x290, { 0x22, 0xA0 });
    LoadData(0x2A0, { 0x22, 0xB0 });
    LoadData(0x2B0, { 0x22, 0xB0 });
    LoadData(0x2C0, { 0x22, 0xB0 });
    LoadData(0x2D0, { 0x22, 0xB0 });
    LoadData(0x2E0, { 0x22, 0xB0 });
    LoadData(0x2F0, { 0x23, 0x00 });
    LoadData(0x300, { 0x23, 0x10 });

    cpu->Reset(InitialPc);

    for(auto i = 0; i < 16; i++) { // Fill up the stack...
        cpu->Cycle();
    }
    REQUIRE_THROWS(cpu->Cycle());  // Boom!
}

TEST("Unconditional jump", "JP") {  // 1nnn
    LoadData(InitialPc, { 0x1F, 0xFF });
    cpu->Reset(InitialPc);

    cpu->Cycle();
    REQUIRE(cpu->GetPc() == 0xFFF);
}

TEST("Skip if register equals immediate value", "SEr,b") {  // 3xnn
    Execute(this, {
        0x60, 0x01,  // LD V0, 0x01
        0x30, 0x01,  // SE V0, 0x01
        0x61, 0x02,  // LD V1, 0x02
        0x30, 0x00,  // SE V0, 0x00
        0x62, 0x03,  // LD V2, 0x03
    });

    REQUIRE(REG("V1") == 0x00);
    REQUIRE(REG("V2") == 0x03);
}

TEST("Skip if register does not equal immediate value", "SNEr,b") {  // 4xnn
    Execute(this, {
        0x60, 0x01,  // LD V0, 0x01
        0x40, 0x01,  // SNE V0, 0x01
        0x61, 0x02,  // LD V1, 0x02
        0x40, 0x00,  // SNE V0, 0x00
        0x62, 0x03,  // LD V2, 0x03
    });

    REQUIRE(REG("V1") == 0x02);
    REQUIRE(REG("V2") == 0x00);
}

TEST("Skip if register equals another register", "SNr,r") {  // 5xy0
    Execute(this, {
        0x60, 0x01,  // LD V0, 0x01
        0x6F, 0x01,  // LD VF, 0x01
        0x50, 0xF0,  // SE V0, VF
        0x61, 0x02,  // LD V1, 0x02
        0x50, 0x10,  // SE V0, V1
        0x62, 0x03,  // LD V2, 0x03
    });

    REQUIRE(REG("V1") == 0x00);
    REQUIRE(REG("V2") == 0x03);
}

TEST("Load registers from value", "LDr,b") {  // 6xnn
    cpu->Reset(InitialPc);

    DoLoadAllRegisters(this);

    REQUIRE(REG("V0") == MockV0);
    REQUIRE(REG("V1") == MockV1);
    REQUIRE(REG("V2") == MockV2);
    REQUIRE(REG("V3") == MockV3);
    REQUIRE(REG("V4") == MockV4);
    REQUIRE(REG("V5") == MockV5);
    REQUIRE(REG("V6") == MockV6);
    REQUIRE(REG("V7") == MockV7);
    REQUIRE(REG("V8") == MockV8);
    REQUIRE(REG("V9") == MockV9);
    REQUIRE(REG("VA") == MockVA);
    REQUIRE(REG("VB") == MockVB);
    REQUIRE(REG("VC") == MockVC);
    REQUIRE(REG("VD") == MockVD);
    REQUIRE(REG("VE") == MockVE);
    REQUIRE(REG("VF") == MockVF);
}

TEST("Add value to register", "ADD") {  // 7xnn
    Execute(this, {
        0x60, 0x01,  // Load V0 with 0x01
        0x70, 0x01,  // Add 0x01 to V0
    });

    REQUIRE(REG("V0") == 0x02);
}

TEST("Load register from register", "LDr,r") {  // 8xy0
    Execute(this, {
        0x60, 0x11,  // Load V0 normally.
        0x81, 0x00,  // Load V0 from V1, etc.
        0x82, 0x10,
        0x83, 0x20,
        0x84, 0x30,
        0x85, 0x40,
        0x86, 0x50,
        0x87, 0x60,
        0x88, 0x70,
        0x89, 0x80,
        0x8A, 0x90,
        0x8B, 0xA0,
        0x8C, 0xB0,
        0x8D, 0xC0,
        0x8E, 0xD0,
        0x8F, 0xE0,
        0x60, 0x00,  // Reset V0
        0x80, 0xF0,  // Load V0 from VF
    });

    REQUIRE(REG("V0") == 0x11);
    REQUIRE(REG("V1") == 0x11);
    REQUIRE(REG("V2") == 0x11);
    REQUIRE(REG("V3") == 0x11);
    REQUIRE(REG("V4") == 0x11);
    REQUIRE(REG("V5") == 0x11);
    REQUIRE(REG("V6") == 0x11);
    REQUIRE(REG("V7") == 0x11);
    REQUIRE(REG("V8") == 0x11);
    REQUIRE(REG("V9") == 0x11);
    REQUIRE(REG("VA") == 0x11);
    REQUIRE(REG("VB") == 0x11);
    REQUIRE(REG("VC") == 0x11);
    REQUIRE(REG("VD") == 0x11);
    REQUIRE(REG("VE") == 0x11);
    REQUIRE(REG("VF") == 0x11);
}

TEST("Bitwise OR", "OR") {  // 8xy1
    Execute(this, {
        0x60, 0b00000001,  // LD V0, 0x01
        0x61, 0b00010000,  // LD V1, 0x10
        0x80, 0x11,  // OR V0, V1
    });

    REQUIRE(REG("V0") == 0b00010001);
}

TEST("Bitwise AND", "AND") {  // 8xy2
    Execute(this, {
        0x60, 0b00110001,  // LD V0, 0x31
        0x61, 0b00010011,  // LD V1, 0x13
        0x80, 0x12,        // AND V0, V1
    });

    REQUIRE(REG("V0") == 0b00010001);
}

TEST("Bitwise XOR", "XOR") {  // 8xy3
    Execute(this, {
        0x60, 0b00110001,  // LD V0, 0x31
        0x61, 0b00010011,  // LD V1, 0x13
        0x80, 0x13,        // XOR V0, V1
    });

    REQUIRE(REG("V0") == 0b00100010);
}

TEST("Addition", "ADD-no-overflow") {  // 8xy4
    Execute(this, {
        0x60, 0x01,  // LD V0, 0x01
        0x61, 0x02,  // LD V1, 0x02
        0x80, 0x14,  // ADD V0, V1
    });

    REQUIRE(REG("V0") == 0x03);
    REQUIRE(REG("VF") == 0x00);
}

TEST("Addition with overflow", "ADD-overflow") {  // 8xy4
    Execute(this, {
        0x60, 0xFF,  // LD V0, 0xFF
        0x61, 0x02,  // LD V1, 0x02
        0x80, 0x14,  // ADD V0, V1
    });

    REQUIRE(REG("V0") == 0x01);
    REQUIRE(REG("VF") == 0x01);
}

TEST("Subtraction", "SUB-no-borrow") {  // 8xy5
    Execute(this, {
        0x60, 0x02,  // LD V0, 0x02
        0x61, 0x01,  // LD V1, 0x01
        0x80, 0x15,  // SUB V0, V1
    });

    REQUIRE(REG("V0") == 0x01);
    REQUIRE(REG("VF") == 0x01);
}

TEST("Subtraction with borrow", "SUB-borrow") {  // 8xy5
    Execute(this, {
        0x60, 0x01,  // LD V0, 0x02
        0x61, 0x02,  // LD V1, 0x01
        0x80, 0x15,  // SUB V0, V1
    });

    REQUIRE(REG("V0") == 0xFF);
    REQUIRE(REG("VF") == 0x00);
}

TEST("Shift right", "SHR") {  // 8xy6
    Execute(this, {
        0x60, 0b00000010,  // LD V0, 0x02
        0x80, 0x06,        // SHR V0
    });
    REQUIRE(REG("V0") == 0x01);
    REQUIRE(REG("VF") == 0x00);

    Execute(this, {
        0x60, 0b00000011,  // LD V0, 0x03
        0x80, 0x06,        // SHR V0
    });
    REQUIRE(REG("V0") == 0x01);
    REQUIRE(REG("VF") == 0x01);
}

TEST("Subtraction (reversed operands)", "SUBN-no-borrow") {  // 8xy7
    Execute(this, {
        0x60, 0x01,  // LD V0, 0x01
        0x61, 0x02,  // LD V1, 0x02
        0x80, 0x17,  // SUBN V0, V1
    });

    REQUIRE(REG("V0") == 0x01);
    REQUIRE(REG("VF") == 0x01);
}

TEST("Subtraction with borrow (reversed operands)", "SUBN-borrow") {  // 8xy7
    Execute(this, {
        0x60, 0x02,  // LD V0, 0x02
        0x61, 0x01,  // LD V1, 0x01
        0x80, 0x17,  // SUBN V0, V1
    });

    REQUIRE(REG("V0") == 0xFF);
    REQUIRE(REG("VF") == 0x00);
}

TEST("Shift left", "SHL") {  // 8xyE
    Execute(this, {
        0x60, 0b01000000,  // LD V0, 0x40
        0x80, 0x0E,        // SHL V0
    });
    REQUIRE(REG("V0") == 0x80);
    REQUIRE(REG("VF") == 0x00);

    Execute(this, {
        0x60, 0b11000000,  // LD V0, 0xC0
        0x80, 0x0E,        // SHL V0
    });
    REQUIRE(REG("V0") == 0x80);
    REQUIRE(REG("VF") == 0x01);
}

TEST("Load I from value", "LDI") {  // Annn
    Execute(this, {
        0xA1, 0x23,  // LD I, 0x123
    });

    REQUIRE(REG("I") == 0x123);
}

TEST("Jump to V0 + value", "JPV0,a") {  // Bnnn
    LoadData(InitialPc, {
        0x60, 0x01,  // LD V0, 0x01
        0xB2, 0x23,  // JP V0, 0x123
    });
    cpu->Reset(InitialPc);

    cpu->Cycle();
    cpu->Cycle();
    REQUIRE(cpu->GetPc() == 0x224);
}

TEST("Delay timer", "LDr,n") {  // Fx07/Fx15
    LoadData(InitialPc, {
        0x60, 0x02,  // LD V0, 0x02
        0xF0, 0x15,  // LD DT, V0
        0xF1, 0x07,  // LD V1, DT
    });
    cpu->Reset(InitialPc);

    cpu->Cycle();
    cpu->Cycle();
    REQUIRE(REG("Delay") == 0x02);

    cpu->DelayTimer();
    cpu->Cycle();
    REQUIRE(REG("Delay") == 0x01);
    REQUIRE(REG("V1") == 0x01);

    cpu->DelayTimer();
    cpu->Cycle();
    REQUIRE(REG("Delay") == 0x00);
    REQUIRE(REG("V1") == 0x01);

    cpu->DelayTimer();
    cpu->Cycle();
    REQUIRE(REG("Delay") == 0x00);
    REQUIRE(REG("V1") == 0x01);
}

TEST("Sound timer", "LDST,r") {  // Fx18
    LoadData(InitialPc, {
        0x60, 0x02,  // LD V0, 0x02
        0xF0, 0x18,  // LD ST, V0
    });
    cpu->Reset(InitialPc);

    cpu->Cycle();
    cpu->Cycle();
    REQUIRE(REG("Audio") == 0x02);

    cpu->DelayTimer();
    REQUIRE(REG("Audio") == 0x01);

    cpu->DelayTimer();
    REQUIRE(REG("Audio") == 0x00);

    cpu->DelayTimer();
    REQUIRE(REG("Audio") == 0x00);
}

TEST("Add register to I", "LDI,r") {  // Fx1E
    Execute(this, {
        0x60, 0x01,  // LD V0, 0x01
        0xA1, 0x23,  // LD I, 0x123
        0xF0, 0x1E,  // ADD I, V0
    });

    REQUIRE(REG("I") == 0x124);
}

TEST("Binary coded decimal", "LDB,r") {  // Fx33
    Execute(this, {
        0x60,  123,  // LD V0, 123
        0xA4, 0x00,  // LD I, 0x400
        0xF0, 0x33,  // LD B, V0
    });

    REQUIRE(memory->GetByte(0x400) == 1);
    REQUIRE(memory->GetByte(0x401) == 2);
    REQUIRE(memory->GetByte(0x402) == 3);
}

TEST("Store registers in memory", "LD(I),r") {  // Fx55
    CreateLoadRegistersRoutine(this);
    Execute(this, {
        0x2F, 0x00,  // CALL 0xF00
        0xA4, 0x00,  // LD I, 0x400
        0xF8, 0x55,  // LD [I], V8
    });

    auto index = 0x400;
    REQUIRE(memory->GetByte(index++) == MockV0);
    REQUIRE(memory->GetByte(index++) == MockV1);
    REQUIRE(memory->GetByte(index++) == MockV2);
    REQUIRE(memory->GetByte(index++) == MockV3);
    REQUIRE(memory->GetByte(index++) == MockV4);
    REQUIRE(memory->GetByte(index++) == MockV5);
    REQUIRE(memory->GetByte(index++) == MockV6);
    REQUIRE(memory->GetByte(index++) == MockV7);
    REQUIRE(memory->GetByte(index++) == MockV8);
    REQUIRE(memory->GetByte(index++) == 0x00);
    REQUIRE(memory->GetByte(index++) == 0x00);
    REQUIRE(memory->GetByte(index++) == 0x00);
    REQUIRE(memory->GetByte(index++) == 0x00);
    REQUIRE(memory->GetByte(index++) == 0x00);
    REQUIRE(memory->GetByte(index++) == 0x00);
    REQUIRE(memory->GetByte(index++) == 0x00);
}

TEST("Populate registers from memory values", "LDr,I") {  // Fx65
    LoadData(0xF00, {
        MockV0, MockV1, MockV2, MockV3, MockV4, MockV5, MockV6, MockV7,
        MockV8, MockV9, MockVA, MockVB, MockVC, MockVD, MockVE, MockVF
    });

    Execute(this, {
        0xAF, 0x00,  // LD I, 0xF00
        0xF8, 0x65,  // LD V8, [I]
    });

    REQUIRE(REG("V0") == MockV0);
    REQUIRE(REG("V1") == MockV1);
    REQUIRE(REG("V2") == MockV2);
    REQUIRE(REG("V3") == MockV3);
    REQUIRE(REG("V4") == MockV4);
    REQUIRE(REG("V5") == MockV5);
    REQUIRE(REG("V6") == MockV6);
    REQUIRE(REG("V7") == MockV7);
    REQUIRE(REG("V8") == MockV8);
    REQUIRE(REG("V9") == 0x00);
    REQUIRE(REG("VA") == 0x00);
    REQUIRE(REG("VB") == 0x00);
    REQUIRE(REG("VC") == 0x00);
    REQUIRE(REG("VD") == 0x00);
    REQUIRE(REG("VE") == 0x00);
    REQUIRE(REG("VF") == 0x00);
}
