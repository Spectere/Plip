/* TimerPosix.h
 *
 * Uses the POSIX nanosleep function to maintain timing.
 */

#pragma once

#include <ctime>

#include "Timer.h"

namespace PlipSdl {
    class TimerPosix : public Timer {
    public:
        TimerPosix() = default;

        void Nanosleep(long ns) override;
        void StopwatchStart() override;
        long StopwatchStop() override;

    private:
        struct timespec m_stopwatchVal {};
    };
}
