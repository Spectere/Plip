// SharpSm83State.h
//
// The state the processor is currently in.

#pragma once

namespace Plip::Cpu {
    enum class SharpSm83State {
        Decode,
        Execute,
        Interrupt,
    };
}
