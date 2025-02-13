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

                case PlipSdlEvent::Step:
                    m_step = true;
                    break;

                case PlipSdlEvent::ToggleGui:
                    m_gui->SetEnabled(!m_gui->GetEnabled());
                    break;

                case PlipSdlEvent::TogglePause:
                    m_paused = !m_paused;
                    break;

                case PlipSdlEvent::TurboDisable:
                    m_turbo = false;
                    break;

                case PlipSdlEvent::TurboEnable:
                    m_turbo = true;
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
        } else if(m_paused && m_step) {
            m_plip->Step();
            m_step = false;
        }

        m_window->Render();
        m_gui->Render();
        m_window->Present();

        const auto elapsedTime = m_timer->StopwatchStop();
        auto delay = m_frameTimeNs - elapsedTime;

        if(!m_turbo) {
            while(delay < 0) {
                delay += m_frameTimeNs;
            }

            m_timer->Nanosleep(delay);
        }
    }

    audio->DequeueAll();
}

void Game::SetPaused(const bool paused) {
    m_paused = paused;
}
