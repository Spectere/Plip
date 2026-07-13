// RunnerTestError.h
//
// Describes the mismatches that occurred within a single failed test.

#pragma once

#include <string>
#include <vector>

#include "RunnerCpuError.h"
#include "RunnerCycleError.h"
#include "RunnerMemoryError.h"

struct RunnerTestResult {
    std::string Key;
    std::string Filename;
    std::vector<RunnerCpuError> RegisterMisses;
    std::vector<RunnerMemoryError> MemoryMisses;
    std::vector<std::string> ExceptionsThrown;
    std::vector<RunnerCycleError> CycleMemoryMisses;

    bool Skipped;
    std::string SkipReason;

    [[nodiscard]] bool Success() const {
        return RegisterMisses.empty() && MemoryMisses.empty() && ExceptionsThrown.empty() && CycleMemoryMisses.empty();
    }

    friend bool operator<(const RunnerTestResult& lhs, const RunnerTestResult& rhs) {
        return (lhs.Key < rhs.Key);
    }
};
