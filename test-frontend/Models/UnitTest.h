// UnitTest.h
//
// A test to perform. This may be divided into multiple subtests.

#pragma once

#include <string>
#include <vector>

#include "StopEvent.h"
#include "Subtest.h"

struct UnitTest {
    std::string Key {};
    std::string Filename {};
    std::string Name {};
    std::string Suite {};
    std::string Core {};
    std::string Rom {};
    StopEvent Stop {};
    std::vector<Subtest> Subtests {};

    friend bool operator<(const UnitTest& lhs, const UnitTest& rhs) {
        return (lhs.Key < rhs.Key);
    }
};
