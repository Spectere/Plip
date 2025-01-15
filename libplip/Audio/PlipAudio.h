/* PlipAudio.h
 *
 * Provides a toolkit-agnostic audio interface.
 */

#pragma once

#include <cstdint>
#include <vector>

namespace Plip {
    class PlipAudio {
    public:
        virtual void DequeueAll() = 0;
        virtual void Enqueue(std::vector<float> buffer) = 0;
        virtual uintmax_t GetQueueSize() = 0;
        virtual bool IsActive() final { return m_active; }

        int GetBufferLength() const { return m_bufferLength; }
        int GetSampleRate() const { return m_sampleRate; }

        static constexpr int BitRate = 32;
        static constexpr int Channels = 2;

    protected:
        PlipAudio(const int sampleRate, const int bufferLength) : m_sampleRate(sampleRate), m_bufferLength(bufferLength) {}
        ~PlipAudio() = default;

        int m_sampleRate;
        int m_bufferLength;

        bool m_active = false;
    };
}
