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
    [[nodiscard]] uint32_t GetPc() override { return m_cpu->GetPc(); }
    void SetInitialState(RunnerTestState state) override;

    void Step() override;

private:
    MockMos6502* m_cpu;
};
