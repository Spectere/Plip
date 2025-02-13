// Game.cpp
//
// The main game loop.

#include "Game.h"

using PlipSdl::Game;

Game::Game(Plip::PlipInstance* plip, SdlEvent* event, SdlWindow* window, Timer* timer, Gui* gui, const int targetFps)
: m_plip(plip), m_event(event), m_window(window), m_timer(timer), m_gui(gui), m_frameTimeNs(1000000000 / targetFps) { }

bool Game::GetPaused() const {
    return m_paused;
}

void Game::Run() {
    const auto audio = m_plip->GetAudio();

    auto running = true;
    while(running) {
        m_timer->StopwatchStart();

        auto uiEvents = m_event->ProcessEvents();
        for(const auto &event : uiEvents) {
            switch(event) {
                case PlipSdlEvent::Quit:
                    running = false;
                    break;

                case PlipSdlEvent::ToggleGui:
                    m_gui->SetEnabled(!m_gui->GetEnabled());
                    break;

                case PlipSdlEvent::TogglePause:
                    m_paused = !m_paused;
                    break;

                case PlipSdlEvent::WindowResized:
                    m_window->CalculateDestinationRectangle();
                    break;

                default:
                    break;
            }
        }

        m_gui->NewFrame();

        if(!m_paused) {
            m_plip->Run(m_frameTimeNs);
        }

        m_window->Render();
        m_gui->Render();
        m_window->Present();

        const auto elapsedTime = m_timer->StopwatchStop();
        auto delay = m_frameTimeNs - elapsedTime;
        while(delay < 0) {
            delay += m_frameTimeNs;
        }

        m_timer->Nanosleep(delay);
    }

    audio->DequeueAll();
}

void Game::SetPaused(const bool paused) {
    m_paused = paused;
}
