/* TimerPosix.cpp
 */

#include "TimerPosix.h"

namespace PlipSdl {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
    void TimerPosix::Nanosleep(const long ns) {
        const struct timespec req {  // tv_nsec must be under 1 million
            .tv_sec = ns / 1000000000,
            .tv_nsec = ns % 1000000000
        };
        struct timespec rem;  // Populated by nanosleep().
        nanosleep(&req, nullptr);
    }

    void TimerPosix::StopwatchStart() {
        clock_gettime(CLOCK_MONOTONIC, &m_stopwatchVal);
    }

    long TimerPosix::StopwatchStop() {
        struct timespec end_val;

        clock_gettime(CLOCK_MONOTONIC, &end_val);
        return ((end_val.tv_sec - m_stopwatchVal.tv_sec) * 1000000)
               + (end_val.tv_nsec - m_stopwatchVal.tv_nsec);
    }
#pragma clang diagnostic pop
}
