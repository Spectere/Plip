// MemoryBusState.h
//
// The state of the CPU's memory bus. Useful for per-cycle debugging.

#pragma once

enum class MemoryBusState {
    None,
    Read,
    Write,
};
