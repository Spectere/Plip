// RunnerCpuError.h
//
// An error result from a test.

#pragma once

#include <string>

struct RunnerCpuError {
    std::string Register;
    uint64_t Expected;
    uint64_t Actual;
};
