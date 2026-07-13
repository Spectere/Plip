// MemoryValue.h
//
// A memory testing result.

#pragma once

#include <cstdint>

struct MemoryValue {
    uint64_t Value {};
    bool ReportOnly {};
};
