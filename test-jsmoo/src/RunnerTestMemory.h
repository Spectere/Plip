// RunnerTestMemory.h
//
// Describes the state of a byte of memory in a unit test.

#pragma once

#include <cstdint>

struct RunnerTestMemory {
    uint32_t Address;
    uint8_t Value;
};
