// Game.h
//
// The main game loop.

#pragma once

#include "PlipInstance.h"
#include "SDL/SdlEvent.h"
#include "SDL/SdlWindow.h"
#include "Timer/Timer.h"

namespace PlipSdl {
    class Game {
    public:
        Game(Plip::PlipInstance *plip, SdlEvent *event, SdlWindow *window, Timer *timer, int targetFps);
        void Run() const;

        [[nodiscard]] bool GetPaused() const;
        void SetPaused(bool paused);

    private:
        Plip::PlipInstance *m_plip;
        SdlEvent *m_event;
        SdlWindow *m_window;
        Timer *m_timer;

        bool m_paused = false;
        int m_frameTimeNs;
    };
}
