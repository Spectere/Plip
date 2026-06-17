// DebugAudioChannel.h
//
// Debug hook for an audio implementation.

#pragma once

#include <string>
#include <utility>

namespace Plip {
    struct DebugAudioChannel {
        DebugAudioChannel(std::string description, bool* value) : Description(std::move(description)), Value(value) {}

        std::string Description {};
        bool* Value {};
    };
}
