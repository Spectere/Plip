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
        void Run() const;

        [[nodiscard]] bool GetPaused() const;
        void SetPaused(bool paused) const;

    private:
        Plip::PlipInstance* m_plip;
        Gui* m_gui;
        SdlEvent* m_event;
        SdlWindow* m_window;
        Timer* m_timer;

        int m_frameTimeNs;
    };
}
