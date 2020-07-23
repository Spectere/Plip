/* Timer.h
 *
 * Implements an interface for a high precision timer and stopwatch.
 */

#pragma once

namespace PlipSdl {
    class Timer {
    public:
        virtual void Nanosleep(long ns) = 0;
        virtual void StopwatchStart() = 0;
        virtual long StopwatchStop() = 0;

    protected:
        Timer() = default;
    };
}
