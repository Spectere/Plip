// Game.h
//
// The main game loop.

#pragma once

#include "Gui.h"
#include "PlipInstance.h"
#include "Sdl/SdlEvent.h"
#include "Sdl/SdlWindow.h"
#include "Timer/Timer.h"

namespace PlipSdl {
    class Game {
    public:
        Game(Plip::PlipInstance* plip, SdlEvent* event, SdlWindow* window, Timer* timer, Gui* gui, int targetFps);
        void Run();

        [[nodiscard]] bool GetPaused() const;
        void SetPaused(bool paused);

    private:
        Plip::PlipInstance* m_plip;
        Gui* m_gui;
        SdlEvent* m_event;
        SdlWindow* m_window;
        Timer* m_timer;

        bool m_paused = false;
        bool m_step = false;
        bool m_turbo = false;
        int m_frameTimeNs;
    };
}
