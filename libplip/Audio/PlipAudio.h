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

        static constexpr int BitRate = 32;
        static constexpr int Channels = 2;
        static constexpr int SampleRate = 48000;
        static constexpr int BufferLength = 4096;

    protected:
        PlipAudio() = default;
        ~PlipAudio() = default;

        bool m_active = false;
    };
}
