// RunnerSharpSm83.h
//
// A test runner for the Sharp SM83 (LR36902).

#pragma once

#include "../Cpu/MockSharpSm83.h"
#include "../RunnerCpu.h"

class RunnerSharpSm83 : public RunnerCpu {
public:
    RunnerSharpSm83();
    virtual ~RunnerSharpSm83() { delete m_cpu; }

    std::vector<RunnerCpuError> CompareState(RunnerTestState state) override;
    bool CycleAccurateMemoryTest() override { return false; }
    [[nodiscard]] uint32_t GetPc() override { return m_cpu->GetPc(); }
    void SetInitialState(RunnerTestState state) override;

    [[nodiscard]] MemoryBusState GetMemoryBusState() const override { return m_cpu->GetMemoryBusState(); }
    [[nodiscard]] uint64_t GetAddressBus() const override { return m_cpu->GetAddressBus(); }
    [[nodiscard]] uint8_t GetDataBus() const override { return m_cpu->GetDataBus(); }

    void Step() override;

private:
    MockSharpSm83* m_cpu;
};
