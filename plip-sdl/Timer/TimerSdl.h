/* TimerSdl.h
 *
 * Uses SDL's delay function to maintain timing. Since SDL3 now supports
 * nanosecond accuracy, we don't need to use any OS-specific calls.
 */

#pragma once

#include <SDL3/SDL.h>

#include "Timer.h"

namespace PlipSdl {
    class TimerSdl final : public Timer {
    public:
        TimerSdl() = default;
        ~TimerSdl() = default;

        void Nanosleep(long ns) override;
        void StopwatchStart() override;
        long StopwatchStop() override;

    private:
        float m_sleepSkew = 0;
        Uint64 m_stopwatchVal = 0;
    };
}
