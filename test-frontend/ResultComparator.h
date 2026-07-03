// ResultComparator.h
//
// Compares the expected results vs. the actual results.

#pragma once

#include "Models/TestResults.h"

class ResultComparator {
public:
    static void CompareResults(const FrontendConfig& config, TestResult& results);

private:
    static bool CompareImages(TestResult& results, const std::string& expected, const std::string& actual);
    static bool CompareRegisters(TestResult& results);
};
