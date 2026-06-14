// IRunner.h
//
// Common runner interface.

#pragma once

#include <set>

#include "RunnerTestResult.h"

struct IRunner {
    virtual ~IRunner() = default;
    virtual std::set<RunnerTestResult> RunTests(const std::set<std::string>& tests) = 0;
};
