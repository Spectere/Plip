/* GameLoop.cpp
 *
 * The main emulation loop.
 */

#include "GameLoop.h"

namespace PlipSdl {
    GameLoop::GameLoop(Plip::PlipInstance *plip, Console *console, SdlEvent *event, Timer *timer, int updateRate) {
        m_plip = plip;
        m_event = event;
        m_timer = timer;

        m_updateRate = updateRate;
        m_updateTime = 1000000000 / m_updateRate;

        m_console = console;

        m_console->RegisterCommand("dump",
                                   [this](auto &&console, auto &&args) { ConsoleDump(console, args); });
        m_console->RegisterCommand("quit",
                                   [this](auto&&, auto&&) { m_running = false; });
    }

    void GameLoop::ConsoleDump(Console *console, const std::vector<std::string> &args) {
        console->WriteLine(m_plip->GetCore()->DumpRegisters());
    }

    void GameLoop::Play() {
        auto audio = m_plip->GetAudio();
        auto core = m_plip->GetCore();

        m_running = true;
        SdlUiEvent uiEvent;
        while(m_running) {
            m_timer->StopwatchStart();

            if(m_console->GetConsoleEnabled()) {
                // Console is enabled. Don't run the core.
                uiEvent = m_console->ProcessEvents();
                m_console->Run();
            } else {
                uiEvent = m_event->ProcessEvents();

                if(core->GetPaused()) {
                    if(uiEvent == SdlUiEvent::Step) {
                        m_plip->Run(core->GetStepTime());
                    } else if(uiEvent == SdlUiEvent::FrameAdvance) {
                        m_plip->Run(m_updateTime);
                    }
                } else {
                    // As implemented, this will not be able to compensate for the host being
                    // unable to keep up with the emulation core.
                    // TODO: Fix this so that it will skip frames where appropriate.
                    m_plip->Run(m_updateTime);
                }
            }

            switch(uiEvent) {
                case SdlUiEvent::PlayPause:
                    core->SetPaused(!core->GetPaused());
                    break;

                case SdlUiEvent::ToggleConsole:
                    m_console->ToggleConsole();
                    break;

                case SdlUiEvent::Quit:
                    m_running = false;
                    break;

                default: break; // clang-tidy appeasement
            }

            auto time = m_timer->StopwatchStop();
            auto delay = m_updateTime - time;
            while(delay < 0)
                delay += m_updateTime;

            m_timer->Nanosleep(delay);
        }

        audio->DequeueAll();
    }
}
