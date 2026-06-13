// RunnerSharpSm83.h
//
// A test runner for the Sharp SM83 (LR36902).

#pragma once

#include "Cpu/MockSharpSm83.h"
#include "RunnerCpu.h"

class RunnerSharpSm83 : public RunnerCpu {
public:
    RunnerSharpSm83();
    virtual ~RunnerSharpSm83() { delete m_cpu; }

    std::vector<RunnerCpuError> CompareState(RunnerTestState state) override;
    void SetInitialState(RunnerTestState state) override;

    void Step() override;

private:
    MockSharpSm83* m_cpu;
};
