// TestResults.h
//
// The results of a subtest.

#pragma once

#include <set>

#include "UnitTest.h"

struct TestResult {
    std::string Key {};
    size_t SubtestId {};
    UnitTest Test {};
    double Duration {};
    uint64_t Cycles {};
    bool TimedOut {};

    std::string ComparisonError {};
    std::string InitializationError {};
    std::string RunnerError {};

    Results Actual {};
    [[nodiscard]] Results Expected() const { return GetSubtest().Expected; }

    bool Success {};
    std::set<std::string> MismatchedRegisters {};

    friend bool operator<(const TestResult& lhs, const TestResult& rhs) {
        return (lhs.Key < rhs.Key);
    }

    [[nodiscard]] bool ErrorOccurred() const {
        return !InitializationError.empty() || !ComparisonError.empty() || !RunnerError.empty();
    }

    [[nodiscard]] Subtest GetSubtest() const {
        return Test.Subtests[SubtestId];
    }
};
