// MockAudio.h
//
// An audio implementation that doesn't play anything.

#pragma once

#include <cstdint>

#include "Audio/PlipAudio.h"

class MockAudio final : public Plip::PlipAudio {
public:
    MockAudio() : PlipAudio(44100, 2048) {}
    ~MockAudio() = default;

    void DequeueAll() override {}
    void Enqueue(std::vector<float> buffer) override { /* no-op */ }
    uintmax_t GetQueueSize() override { return 0; }
};
