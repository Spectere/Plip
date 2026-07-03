// Subtest.h
//
// A subtest to run within a unit test. Subtests usually represent different system configurations.

#pragma once

#include <string>
#include <unordered_map>

#include "Results.h"

struct Subtest {
    std::string Name {};
    std::unordered_map<std::string, std::string> Config {};
    Results Expected {};
};
