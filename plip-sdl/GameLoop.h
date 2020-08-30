/* GameLoop.h
 *
 * The main emulation loop.
 */

#pragma once

#include <iomanip>
#include <sstream>

#include "PlipInstance.h"

#include "Console.h"
#include "SDL/SdlEvent.h"
#include "SDL/SdlWindow.h"
#include "Timer/Timer.h"

namespace PlipSdl {
    class GameLoop {
    public:
        GameLoop(Plip::PlipInstance *plip, Console *console, SdlEvent *event, Timer *timer, int updateRate);

        void Play();
        void SetStep(bool value);

    private:
        static inline std::string PrintHex(uintmax_t val, int precision) {
            std::stringstream fmt;
            fmt << "0x" << std::uppercase << std::setfill('0') << std::setw(precision)
                << std::hex << val;
            return fmt.str();
        };

        void ConsoleDump(Console *console, const std::vector<std::string> &args);
        void ConsoleMem(Console *console, const std::vector<std::string> &args);
        void ConsoleSetBreak(Console *console, const std::vector<std::string> &args);

        Plip::PlipInstance *m_plip;
        Console *m_console;
        SdlEvent *m_event;
        Timer *m_timer;
        SdlWindow *m_wnd;

        int m_updateRate;
        int m_updateTime;

        bool m_running {};
    };
}
