// MockSharpSm83.h
//
// SM83 core with testing hooks.

#pragma once

#include "Cpu/SharpLr35902/SharpLr35902.h"

using namespace Plip;

class MockSharpSm83 final : public Cpu::SharpLr35902 {
public:
    MockSharpSm83(const long hz, PlipMemoryMap* memoryMap) : SharpLr35902(hz, memoryMap, false) { }

    [[nodiscard]] bool GetIme() const { return m_ime != Cpu::SharpLr35902ImeState::Disabled; }
    [[nodiscard]] Cpu::SharpLr35902Registers* GetRegisterPointer() { return &m_registers; }
    void SetInterruptEnabled(const bool val) { m_enableInterrupts = val; }
    void SetIme(const bool val) { m_ime = val ? Cpu::SharpLr35902ImeState::Enabled
                                              : Cpu::SharpLr35902ImeState::Disabled; }
};
