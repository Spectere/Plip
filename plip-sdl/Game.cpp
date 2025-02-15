// Game.cpp
//
// The main game loop.

#include "Game.h"
#include "PlipUiEvent.h"

using PlipSdl::Game;

Game::Game(Plip::PlipInstance* plip, SdlEvent* event, SdlWindow* window, Timer* timer, Gui* gui, const int targetFps)
: m_plip(plip), m_event(event), m_window(window), m_timer(timer), m_gui(gui), m_frameTimeNs(1000000000 / targetFps) { }

bool Game::GetPaused() const {
    return m_gui->State.PauseCore;
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
                    m_gui->State.SingleStep = true;
                    break;

                case PlipUiEvent::ToggleGui:
                    m_gui->SetEnabled(!m_gui->GetEnabled());
                    break;

                case PlipUiEvent::TogglePause:
                    m_gui->State.PauseCore = !m_gui->State.PauseCore;
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
            if(m_gui->State.PerformRead) {
                auto memoryBase = m_gui->State.ReadAddress;
                const auto coreMemoryAmount = m_plip->GetCore()->GetMemoryMap()->GetLength();
                constexpr auto displayColumns = m_gui->State.MemoryDisplayColumns;
                constexpr auto displayRows = m_gui->State.MemoryDisplayRows;

                // Clamp the memory base.
                if(memoryBase >= coreMemoryAmount) {
                    memoryBase = coreMemoryAmount - 1;
                }

                // Figure out where to start reading.
                if(memoryBase < displayColumns) {
                    memoryBase = 0;
                } else if(memoryBase + displayColumns > coreMemoryAmount) {
                    memoryBase = memoryBase - (memoryBase % displayColumns) - (displayColumns * (displayRows - 1));
                } else {
                    memoryBase = memoryBase - (memoryBase % displayColumns) - displayColumns;
                }
                m_gui->State.MemoryDisplayBase = memoryBase;

                // Dump!
                for(auto i = 0; i < displayColumns * displayRows; i++) {
                    m_gui->State.MemoryContents[i] = m_plip->GetCore()->GetMemoryMap()->GetByte(memoryBase + i);
                }
            }

            m_gui->SetDebugInfo(m_plip->GetCore()->GetDebugInfo());
            m_gui->Update();

            if(m_gui->State.PerformWrite) {
                m_plip->GetCore()->GetMemoryMap()->SetByte(
                    m_gui->State.WriteAddress,
                    m_gui->State.WriteValue
                );
                m_gui->State.PerformWrite = false;
            }
        }

        if(!m_gui->State.PauseCore) {
            m_plip->Run(m_frameTimeNs);
        } else if(m_gui->State.PauseCore && m_gui->State.SingleStep) {
            m_plip->Step();
            m_gui->State.SingleStep = false;
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

void Game::SetPaused(const bool paused) const {
    m_gui->State.PauseCore = paused;
}
