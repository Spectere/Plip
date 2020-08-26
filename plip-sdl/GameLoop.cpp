/* GameLoop.cpp
 *
 * The main emulation loop.
 */

#include <iomanip>
#include <sstream>

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
        m_console->RegisterCommand("mem",
                                   [this](auto &&console, auto &&args) { ConsoleMem(console, args); });
        m_console->RegisterCommand("quit",
                                   [this](auto&&, auto&&) { m_running = false; });
    }

    void GameLoop::ConsoleDump(Console *console, const std::vector<std::string> &args) {
        console->WriteLine(m_plip->GetCore()->DumpRegisters());
    }

    void GameLoop::ConsoleMem(Console *console, const std::vector<std::string> &args) {
        std::stringstream ss;
        auto mem = m_plip->GetCore()->GetMemoryMap();

        auto printHex = [](uintmax_t val, int precision) {
            std::stringstream fmt;
            fmt << "0x" << std::uppercase << std::setfill('0') << std::setw(precision)
                << std::hex << val;
            return fmt.str();
        };

        if(args.size() < 2) {
            console->WriteLine("usage: mem [address] ([value])");
            return;
        }

        unsigned long input;
        if(!Console::ParseULong(args[1], &input)) {
            console->WriteLine("invalid address");
            return;
        }

        auto addr = (uint32_t)input;

        uint8_t val;
        if(args.size() < 3) {
            val = mem->GetByte(addr);
        } else {
            if(!Console::ParseULong(args[2], &input)) {
                console->WriteError("invalid value");
                return;
            }

            if(input > 0xFF)
                console->WriteWarn("value will be truncated");

            val = input & 0xFF;
            mem->SetByte(addr, val);
        }

        ss << "[" << printHex(addr, 8) << "] == " << std::to_string(val) << " ("
           << printHex(val, 2) << ")";
        console->WriteLine(ss.str());
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
