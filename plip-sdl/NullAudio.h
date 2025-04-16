/* NullAudio.h
 *
 * An audio implementation that doesn't play anything.
 */

#pragma once

#include "Audio/PlipAudio.h"

namespace PlipSdl {
    class NullAudio final : public Plip::PlipAudio {
    public:
        NullAudio() : PlipAudio(44100, 2048) {}
        ~NullAudio() {}
        
        void DequeueAll() override {}
        void Enqueue(std::vector<float> buffer) override {}
        uintmax_t GetQueueSize() override { return 0; }
    };
}
