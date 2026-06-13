// RunnerTestState.h
//
// Describes the overall state of the CPU at a given moment.

#pragma once

#include <map>
#include <vector>

#include "RunnerTestMemory.h"

struct RunnerTestState {
    uint32_t ProgramCounter;
    std::map<std::string, uint64_t> Registers;
    std::vector<RunnerTestMemory> Memory;
};
