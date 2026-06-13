// RunnerTest.h
//
// A test definition, interpreted from a Jsmoo-style test.

#pragma once

#include <string>
#include <vector>

#include "RunnerTestCycle.h"
#include "RunnerTestState.h"

struct RunnerTest {
    std::string Filename;
    std::string TestName;
    std::vector<RunnerTestCycle> Cycles;
    RunnerTestState InitialState;
    RunnerTestState FinalState;
};
