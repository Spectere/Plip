/* GameLoop.h
 *
 * The main emulation loop.
 */

#pragma once

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

    private:
        void ConsoleDump(Console *console, const std::vector<std::string> &args);
        void ConsoleMem(Console *console, const std::vector<std::string> &args);

        Plip::PlipInstance *m_plip;
        Console *m_console;
        SdlEvent *m_event;
        Timer *m_timer;

        int m_updateRate;
        int m_updateTime;

        bool m_running {};
    };
}
