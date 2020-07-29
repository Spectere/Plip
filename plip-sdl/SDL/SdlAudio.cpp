/* SdlAudio.cpp
 *
 * An SDL2 audio implementation.
 */

#include <iostream>

#include "SdlAudio.h"

namespace PlipSdl {
    SdlAudio::SdlAudio() {
        SDL_InitSubSystem(SDL_INIT_AUDIO);

        // Open audio device.
        SDL_AudioSpec want {};

        want.freq = Plip::PlipAudio::SampleRate;
        want.format = AUDIO_F32;
        want.channels = Plip::PlipAudio::Channels;
        want.samples = SampleLength;
        want.callback = nullptr;

        m_device = SDL_OpenAudioDevice(nullptr, 0, &want, &m_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if(m_device < 0) {
            std::cerr << "Unable to open audio: " << SDL_GetError() << std::endl;
            return;
        }

        // Set up conversion structure.
        SDL_BuildAudioCVT(&m_cvt,
                AUDIO_F32, 2, 48000,
                m_spec.format, m_spec.channels, m_spec.freq);

        if(m_cvt.needed > 0) {
            // Conversion required.
            m_playFunc = &SdlAudio::ConvertQueue;
        } else if(m_cvt.needed < 0) {
            // An error has occurred.
            std::cerr << "Unable to set up audio converter: " << SDL_GetError() << std::endl;
            SDL_CloseAudio();
            return;
        } else {
            // No conversion required.
            m_playFunc = &SdlAudio::DirectQueue;
        }

        m_active = true;
        SDL_PauseAudioDevice(m_device, 0);
    }

    SdlAudio::~SdlAudio() {
        if(m_active) SDL_CloseAudioDevice(m_device);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    void SdlAudio::ConvertQueue(void *data, int size) {
        m_cvt.len = size * 4;  // 32-bit float == 4 bytes
        m_cvt.buf = (Uint8*)SDL_malloc(m_cvt.len * m_cvt.len_mult);
        SDL_memcpy(m_cvt.buf, data, m_cvt.len);
        SDL_ConvertAudio(&m_cvt);

        // TODO: This currently leaves gaps in the audio if m_cvt.buf is not a
        // multiple of SampleRate. Should probably be fixed at some point. :)
        SDL_QueueAudio(m_device, m_cvt.buf, m_cvt.len_cvt);
        SDL_free(m_cvt.buf);
    }

    void SdlAudio::DequeueAll() {
        SDL_ClearQueuedAudio(m_device);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"
    void SdlAudio::DirectQueue(void *data, int size) {
        SDL_QueueAudio(m_device, data, size * 4);
    }
#pragma clang diagnostic pop

    void SdlAudio::Enqueue(std::vector<float> buffer) {
        if(!IsActive()) return;
        (this->*m_playFunc)(buffer.data(), buffer.size());
    }

    int SdlAudio::GetBufferSize() {
        return m_spec.samples;
    }

    uintmax_t SdlAudio::GetQueueSize() {
        return SDL_GetQueuedAudioSize(m_device);
    }
}
