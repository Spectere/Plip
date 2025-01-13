/* TimerSdl.h
 *
 * Uses SDL's delay function to maintain timing. Since SDL3 now supports
 * nanosecond accuracy, we don't need to use any OS-specific calls.
 */

#include "TimerSdl.h"

namespace PlipSdl {
    void TimerSdl::Nanosleep(const long ns) {
        SDL_DelayNS(ns);  // *So* much easier than it used to be.
    }

    void TimerSdl::StopwatchStart() {
        m_stopwatchVal = SDL_GetTicksNS();
    }

    long TimerSdl::StopwatchStop() {
        const Uint64 endVal = SDL_GetTicksNS();
        if(endVal >= m_stopwatchVal) {
            return static_cast<long>(endVal - m_stopwatchVal);
        }

        // The timer is eventually going to wrap around. If that happens, try to cope with it.
        return UINT64_MAX - m_stopwatchVal + endVal;
    }
}
