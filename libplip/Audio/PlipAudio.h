/* PlipAudio.h
 *
 * Provides a toolkit-agnostic audio interface.
 */

#pragma once

#include <vector>

namespace Plip {
    class PlipAudio {
    public:
        virtual void DequeueAll() = 0;
        virtual void Enqueue(std::vector<float> buffer) = 0;
        virtual int GetBufferSize() = 0;
        virtual uintmax_t GetQueueSize() = 0;
        virtual bool IsActive() final { return m_active; }

        static const int BitRate = 32;
        static const int Channels = 2;
        static const int SampleRate = 48000;

    protected:
        PlipAudio() = default;

        bool m_active = false;
    };
}
