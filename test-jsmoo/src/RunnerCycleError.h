// RunnerCycleError.h
//
// Results for cycle-accurate memory testing.

#pragma once

#include <cstdint>
#include <format>

#include "Cpu/MemoryBusState.h"

struct RunnerCycleError {
    uint64_t CycleNumber {};

    MemoryBusState MemoryBusStateExpected {};
    MemoryBusState MemoryBusStateActual {};

    uint64_t AddressBusExpected {};
    uint64_t AddressBusActual {};

    uint8_t DataBusExpected {};
    uint8_t DataBusActual {};

    static std::string BusStateString(MemoryBusState busState) {
        return std::format("{}{}{}",
            (busState == MemoryBusState::Read ? "r" : "-"),
            (busState == MemoryBusState::Write ? "w" : "-"),
            (busState != MemoryBusState::None ? "m" : "-")
        );
    }
};
