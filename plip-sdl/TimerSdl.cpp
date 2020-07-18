/* TimerSdl.h
 *
 * Uses SDL's delay function to maintain timing. Since SDL's only supports
 * millisecond accuracy, using the native OS timing functions is preferred
 * where possible.
 */

#include "TimerSdl.h"

#define MS_TO_NS(ms) ((ms) * 1000000)
#define NS_TO_MS(ns) ((ns) / 1000000)

namespace PlipSdl {
    void TimerSdl::Nanosleep(const long ns) {
        // SDL2's timing functions are only precise to the millisecond, so we
        // save the lost precision and apply it to a future function call.
        // This method is not suitable for repeated short delays, but is
        // generally fine for maintaining timing.
        Uint32 waitTime = NS_TO_MS(ns);
        m_sleepSkew += (float)ns / 1000000 - waitTime;

        if(m_sleepSkew >= 1) {
            Uint32 skewAdd = (long)m_sleepSkew;
            waitTime += skewAdd;
            m_sleepSkew -= skewAdd;
        }

        if(waitTime > 0)
            SDL_Delay(waitTime);
    }

    void TimerSdl::StopwatchStart() {
        m_stopwatchVal = SDL_GetTicks();
    }

    long TimerSdl::StopwatchStop() {
        Uint32 endVal = SDL_GetTicks();
        if(endVal >= m_stopwatchVal)
            return MS_TO_NS(endVal - m_stopwatchVal);

        // The timer wraps around after 49 days. Try to cope with the stopwatch
        // running for that length of time. :'(
        return MS_TO_NS((UINT32_MAX - endVal) + m_stopwatchVal);
    }
}
