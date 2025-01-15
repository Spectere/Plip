// Game.cpp
//
// The main game loop.

#include "Game.h"

using PlipSdl::Game;

Game::Game(Plip::PlipInstance *plip, SdlEvent *event, SdlWindow *window, Timer *timer, const int targetFps)
: m_plip(plip), m_event(event), m_window(window), m_timer(timer), m_frameTimeNs(1000000000 / targetFps) { }

bool Game::GetPaused() const {
    return m_paused;
}

void Game::Run() const {
    const auto audio = m_plip->GetAudio();

    auto running = true;
    while(running) {
        m_timer->StopwatchStart();

        switch(m_event->ProcessEvents()) {
            case SdlUiEvent::Quit:
                running = false;
                break;

            case SdlUiEvent::WindowResized:
                m_window->CalculateDestinationRectangle();
                break;

            default:
                break;
        }

        if(!m_paused) {
            m_plip->Run(m_frameTimeNs);
        }

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
