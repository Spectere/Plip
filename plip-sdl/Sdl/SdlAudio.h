/* SdlAudio.h
 *
 * An SDL2 audio implementation.
 */

#pragma once

#include <SDL3/SDL.h>

#include "Audio/PlipAudio.h"

namespace PlipSdl {
    class SdlAudio final : public Plip::PlipAudio {
    public:
        SdlAudio(int sampleRate, int bufferLength);
        ~SdlAudio();

        void DequeueAll() override;
        void Enqueue(std::vector<float> buffer) override;
        uintmax_t GetQueueSize() override;

        static constexpr int SampleLength = 4096;

    private:
        size_t m_bufferSize = {};
        SDL_AudioStream* m_stream = nullptr;
    };
}
