/* TimerSdl.h
 *
 * Uses SDL's delay function to maintain timing. Since SDL's only supports
 * millisecond accuracy, using the native OS timing functions is preferred
 * where possible.
 */

#pragma once

#include <SDL.h>

#include "Timer.h"

namespace PlipSdl {
    class TimerSdl : public Timer {
    public:
        TimerSdl() = default;

        void Nanosleep(long ns) override;
        void StopwatchStart() override;
        long StopwatchStop() override;

    private:
        float m_sleepSkew = 0;
        Uint32 m_stopwatchVal = 0;
    };
}
