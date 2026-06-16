// MockMos6502.h
//
// MOS 6502 core with testing hooks.

#pragma once

#include "Cpu/Mos6502/Mos6502.h"

using namespace Plip;

class MockMos6502 final : public Cpu::Mos6502 {
public:
    MockMos6502(const long hz, PlipMemoryMap* memoryMap) : Mos6502(hz, memoryMap, Cpu::Mos6502Version::Mos6502) { }

    [[nodiscard]] Cpu::Mos6502Registers* GetRegisterPointer() { return &m_registers; }
};
