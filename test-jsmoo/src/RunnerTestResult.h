// RunnerTestError.h
//
// Describes the mismatches that occurred within a single failed test.

#pragma once

#include <string>
#include <vector>

#include "RunnerCpuError.h"
#include "RunnerMemoryError.h"

struct RunnerTestResult {
    std::string Key;
    std::vector<RunnerCpuError> RegisterMisses;
    std::vector<RunnerMemoryError> MemoryMisses;

    bool Skipped;
    std::string SkipReason;

    [[nodiscard]] bool Success() const {
        return RegisterMisses.empty() && MemoryMisses.empty();
    }

    friend bool operator<(const RunnerTestResult& lhs, const RunnerTestResult& rhs) {
        return (lhs.Key < rhs.Key);
    }
};
