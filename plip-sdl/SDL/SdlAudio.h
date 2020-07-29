/* SdlAudio.h
 *
 * An SDL2 audio implementation.
 */

#pragma once

#include <SDL.h>

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
        void ConvertQueue(void *data, int size);
        void DirectQueue(void *data, int size);

        SDL_AudioCVT m_cvt {};
        SDL_AudioDeviceID m_device = 0;
        SDL_AudioSpec m_spec {};
        void (SdlAudio::*m_playFunc)(void*, int);
    };
}
