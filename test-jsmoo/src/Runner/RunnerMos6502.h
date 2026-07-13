// RunnerMos6502.h
//
// TODO: Add description here.

#pragma once

#include "../Cpu/MockMos6502.h"
#include "../RunnerCpu.h"

class RunnerMos6502 : public RunnerCpu {
public:
    RunnerMos6502();
    virtual ~RunnerMos6502() { delete m_cpu; }

    std::vector<RunnerCpuError> CompareState(RunnerTestState state) override;
    bool CycleAccurateMemoryTest() override { return false; }
    [[nodiscard]] uint32_t GetPc() override { return m_cpu->GetPc(); }
    void SetInitialState(RunnerTestState state) override;

    [[nodiscard]] MemoryBusState GetMemoryBusState() const override { return m_cpu->GetMemoryBusState(); }
    [[nodiscard]] uint64_t GetAddressBus() const override { return m_cpu->GetAddressBus(); }
    [[nodiscard]] uint8_t GetDataBus() const override { return m_cpu->GetDataBus(); }

    void Step() override;

private:
    MockMos6502* m_cpu;
};
