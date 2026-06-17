// MockSharpSm83.h
//
// SM83 core with testing hooks.

#pragma once

#include "Cpu/SharpSm83/SharpSm83.h"

using namespace Plip;

class MockSharpSm83 final : public Cpu::SharpSm83 {
public:
    MockSharpSm83(const long hz, PlipMemoryMap* memoryMap) : SharpSm83(hz, memoryMap, false) { }

    [[nodiscard]] bool GetIme() const { return m_ime != Cpu::SharpSm83ImeState::Disabled; }
    [[nodiscard]] bool GetInterruptEnabled() const { return m_enableInterrupts; }
    [[nodiscard]] Cpu::SharpSm83Registers* GetRegisterPointer() { return &m_registers; }
    void SetInterruptEnabled(const bool val) { m_enableInterrupts = val; }
    void SetIme(const bool val) { m_ime = val ? Cpu::SharpSm83ImeState::Enabled
                                              : Cpu::SharpSm83ImeState::Disabled; }
};
