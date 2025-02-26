// Game.h
//
// The main game loop.

#pragma once

#include "Gui.h"
#include "PlipInstance.h"
#include "Sdl/SdlEvent.h"
#include "Sdl/SdlWindow.h"

namespace PlipSdl {
    class Game {
    public:
        Game(Plip::PlipInstance* plip, SdlEvent* event, SdlWindow* window, Gui* gui, int targetFps);
        void Run() const;

        [[nodiscard]] bool GetPaused() const;
        void SetPaused(bool paused) const;

    private:
        static constexpr auto AverageFrameTimeSampleSize = 60;

        void AddBreakpoint(uint64_t addr);
        void RemoveBreakpoint(uint64_t addr);

        Plip::PlipInstance* m_plip;
        Gui* m_gui;
        SdlEvent* m_event;
        SdlWindow* m_window;

        double m_frameTime;
    };
}
