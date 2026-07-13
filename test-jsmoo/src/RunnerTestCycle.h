// RunnerTestCycle.h
//
// Describes a single cycle in a Jsmoo-style unit test.

#pragma once

#include <cstdint>

struct RunnerTestCycle {
    uint64_t Address;
    uint8_t Value;

    bool MemoryRead;
    bool MemoryWrite;
    bool MemoryRequest;
};
