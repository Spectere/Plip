// RunnerMemoryError.h
//
// An error result from a test.

#pragma once

#include <cstdint>

struct RunnerMemoryError {
    uint32_t Address;
    uint8_t Expected;
    uint8_t Actual;
};
