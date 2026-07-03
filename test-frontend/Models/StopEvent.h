// StopEvent.h
//
// The event that should cause the test to stop. The runner will automatically
// abort the test if it takes too long to run.

#pragma once

#include "ValueType.h"

enum class StopType {
    None,
    Cycles,
    Opcode,
};

struct StopEvent {
    StopType Type = StopType::None;
    ValueType Value {};
};
