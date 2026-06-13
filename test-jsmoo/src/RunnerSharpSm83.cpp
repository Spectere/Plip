// RunnerSharpSm83.cpp
//
// A test runner for the Sharp SM83 (LR36902).

#include "RunnerSharpSm83.h"

RunnerSharpSm83::RunnerSharpSm83() : RunnerCpu(64 * 1024) /* 64 KiB */ {
    m_cpu = new MockSharpSm83(1000, memory);
}

std::vector<RunnerCpuError> RunnerSharpSm83::CompareState(RunnerTestState state) {
    std::vector<RunnerCpuError> results {};
    const auto reg = m_cpu->GetRegisterPointer();

    for(const auto & [r, expected] : state.Registers) {
        if(r == "a" && reg->A != expected)
            results.push_back({ "A", expected, reg->A });
        else if(r == "b" && reg->B != expected)
            results.push_back({ "B", expected, reg->B });
        else if(r == "c" && reg->C != expected)
            results.push_back({ "C", expected, reg->C });
        else if(r == "d" && reg->D != expected)
            results.push_back({ "D", expected, reg->D });
        else if(r == "e" && reg->E != expected)
            results.push_back({ "E", expected, reg->E });
        else if(r == "f" && reg->F != expected)
            results.push_back({ "F", expected, reg->F });
        else if(r == "h" && reg->H != expected)
            results.push_back({ "H", expected, reg->H });
        else if(r == "l" && reg->L != expected)
            results.push_back({ "L", expected, reg->L });
        else if(r == "sp" && reg->SP != expected)
            results.push_back({ "SP", expected, reg->SP });
        else if(const auto pc = m_cpu->GetPc(); r == "pc" && pc != expected)
            results.push_back({ "PC", expected, pc });
        else if(const uint64_t ime = m_cpu->GetIme() ? 1 : 0; r == "ime" && ime != expected)
            results.push_back({ "IME", expected, ime });
    }

    return results;
}

void RunnerSharpSm83::SetInitialState(const RunnerTestState state) {
    const auto reg = m_cpu->GetRegisterPointer();

    m_cpu->Reset(state.ProgramCounter);

    for(const auto & [r, val] : state.Registers) {
        if(r == "a") reg->A = val;
        else if(r == "b") reg->B = val;
        else if(r == "c") reg->C = val;
        else if(r == "d") reg->D = val;
        else if(r == "e") reg->E = val;
        else if(r == "f") reg->F = val;
        else if(r == "h") reg->H = val;
        else if(r == "l") reg->L = val;
        else if(r == "sp") reg->SP = val;
        else if(r == "ie") m_cpu->SetInterruptEnabled(val);
        else if(r == "ime") m_cpu->SetIme(val);
    }
}

void RunnerSharpSm83::Step() {
    m_cpu->Step();
}
