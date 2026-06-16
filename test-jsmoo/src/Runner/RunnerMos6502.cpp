// RunnerMos6502.cpp
//
// A test runner for the MOS 6502 (NMOS version).

#include "RunnerMos6502.h"

RunnerMos6502::RunnerMos6502() : RunnerCpu(64 * 1024) /* 64 KiB */ {
    m_cpu = new MockMos6502(1000, memory);
}

std::vector<RunnerCpuError> RunnerMos6502::CompareState(RunnerTestState state) {
    std::vector<RunnerCpuError> results {};
    const auto reg = m_cpu->GetRegisterPointer();

    for(const auto &[r, expected] : state.Registers) {
        if(r == "a" && reg->A != expected)
            results.push_back({ "A", expected, reg->A });
        if(r == "x" && reg->X != expected)
            results.push_back({ "X", expected, reg->X });
        if(r == "y" && reg->Y != expected)
            results.push_back({ "Y", expected, reg->Y });
        if(r == "p" && reg->P != expected)
            results.push_back({ "P", expected, reg->P });
        if(r == "s" && reg->S != expected)
            results.push_back({ "S", expected, reg->S });
    }

    return results;
}

void RunnerMos6502::SetInitialState(RunnerTestState state) {
    const auto reg = m_cpu->GetRegisterPointer();

    // Plip's 6502 uses the reset vector to set initial state. We need to override that for the SST suite to work.
    m_cpu->Reset(0);
    m_cpu->GetRegisterPointer()->PC = state.ProgramCounter;

    for(const auto &[r, val] : state.Registers) {
        if(r == "a") reg->A = val;
        if(r == "x") reg->X = val;
        if(r == "y") reg->Y = val;
        if(r == "p") reg->P = val;
        if(r == "s") reg->S = val;
    }
}

void RunnerMos6502::Step() {
    m_cpu->Step();
}
