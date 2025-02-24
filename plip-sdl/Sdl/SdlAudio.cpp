/* SdlAudio.cpp
 *
 * An SDL2 audio implementation.
 */

#include <iostream>

#include "SdlAudio.h"

using PlipSdl::SdlAudio;

SdlAudio::SdlAudio(const int sampleRate, const int bufferLength) : PlipAudio(sampleRate, bufferLength) {
    SDL_InitSubSystem(SDL_INIT_AUDIO);

    // Open audio device.
    const SDL_AudioSpec want { SDL_AUDIO_F32, Channels, m_sampleRate };

    m_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want, nullptr, nullptr);
    if(m_stream == nullptr) {
        std::cerr << "Unable to open audio: " << SDL_GetError() << std::endl;
        return;
    }

    if(SDL_ResumeAudioStreamDevice(m_stream)) {
        m_active = true;
    } else {
        std::cerr << "Error starting audio stream: " << SDL_GetError() << std::endl;
    }
}

SdlAudio::~SdlAudio() {
    if(m_active) {
        SDL_DestroyAudioStream(m_stream);
        m_active = false;
    }

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void SdlAudio::DequeueAll() {
    SDL_ClearAudioStream(m_stream);
}

void SdlAudio::Enqueue(const std::vector<float> buffer) {
    if(!m_active) return;
    SDL_PutAudioStreamData(m_stream, buffer.data(), static_cast<int>(buffer.size()));
}

uintmax_t SdlAudio::GetQueueSize() {
    return SDL_GetAudioStreamAvailable(m_stream);
}
