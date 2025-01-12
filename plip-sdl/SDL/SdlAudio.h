/* SdlAudio.h
 *
 * An SDL2 audio implementation.
 */

#pragma once

#include <SDL3/SDL.h>

#include "Audio/PlipAudio.h"

namespace PlipSdl {
    class SdlAudio : public Plip::PlipAudio {
    public:
        SdlAudio();
        ~SdlAudio();

        void DequeueAll() override;
        void Enqueue(std::vector<float> buffer) override;
        uintmax_t GetQueueSize() override;

        static const int SampleLength = 4096;

    private:
        size_t m_bufferSize = {};
        SDL_AudioStream* m_device = nullptr;
    };
}
