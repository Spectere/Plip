// Game.cpp
//
// The main game loop.

#include "Game.h"
#include "PlipUiEvent.h"

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
                case PlipUiEvent::Quit:
                    running = false;
                    break;

                case PlipUiEvent::Step:
                    m_step = true;
                    break;

                case PlipUiEvent::ToggleGui:
                    m_gui->SetEnabled(!m_gui->GetEnabled());
                    break;

                case PlipUiEvent::TogglePause:
                    m_paused = !m_paused;
                    break;

                case PlipUiEvent::TurboDisable:
                    m_turbo = false;
                    break;

                case PlipUiEvent::TurboEnable:
                    m_turbo = true;
                    break;

                case PlipUiEvent::WindowResized:
                    m_window->CalculateDestinationRectangle();
                    break;

                default:
                    break;
            }
        }

        m_gui->NewFrame();
        if(m_gui->GetEnabled()) {
            m_gui->SetDebugInfo(m_plip->GetCore()->GetDebugInfo());
            switch(m_gui->Update()) {
                case PlipUiEvent::PauseDisable:
                    m_paused = false;
                    break;

                case PlipUiEvent::PauseEnable:
                    m_paused = true;
                    break;

                case PlipUiEvent::Step:
                    m_step = true;
                    break;

                default:
                    break;
            }
        }

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
