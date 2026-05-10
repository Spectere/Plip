// Game.cpp
//
// The main game loop.

#include <chrono>
#include <thread>

#include "Game.h"
#include "PlipUiEvent.h"

using PlipSdl::Game;

Game::Game(Plip::PlipInstance* plip, SdlEvent* event, SdlWindow* window, Gui* gui, const int targetFps)
: m_plip(plip), m_gui(gui), m_event(event), m_window(window), m_frameTime(1.0 / targetFps) { }

bool Game::GetPaused() const {
    return m_gui->State.PauseCore;
}

void Game::Run() const {
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::nanoseconds;
    using std::chrono::steady_clock;

    const auto frameTimeNs = m_frameTime * 1000000000;
    const auto frameDuration = duration_cast<steady_clock::duration>(duration<double>(m_frameTime));
    const auto audio = m_plip->GetAudio();
    auto averageTimeCount = 0;
    double averageFrameTime = 0;
    double averageWorkTime = 0;
    auto lastFrameTime = steady_clock::now();
    auto nextFrameTarget = steady_clock::now() + frameDuration;

    double sleepLatencySec = 0;
    auto sleepLatency = duration_cast<steady_clock::duration>(duration<double>(0.0));

    auto running = true;
    while(running) {
        const auto frameStartTime = steady_clock::now();

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
                    m_gui->State.BreakpointHit = UINT64_MAX;
                    m_gui->State.PcAddresses.clear();
                    break;

                case PlipUiEvent::TurboDisable:
                    m_gui->State.TurboEnabled = false;
                    break;

                case PlipUiEvent::TurboEnable:
                    m_gui->State.TurboEnabled = true;
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
                constexpr auto displayColumns = GuiState::MemoryDisplayColumns;
                constexpr auto displayRows = GuiState::MemoryDisplayRows;

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

            if(m_gui->State.BreakpointsUpdated) {
                m_gui->State.BreakpointsUpdated = false;
                m_plip->GetCore()->SetBreakpoints(m_gui->State.Breakpoints);
            }
        }

        if(!m_gui->State.PauseCore) {
            m_plip->Run(frameTimeNs);

            if(m_plip->GetCore()->GetActiveBreakpoint() != UINT64_MAX) {
                m_gui->State.BreakpointHit = m_plip->GetCore()->GetActiveBreakpoint();
                m_gui->State.PauseCore = true;
            }
        } else if(m_gui->State.PauseCore && m_gui->State.SingleStep) {
            m_plip->Step();
            m_gui->State.SingleStep = false;
            m_gui->State.PcAddresses = m_plip->GetCore()->GetPcs();
        } else {
            m_gui->State.PcAddresses = m_plip->GetCore()->GetPcs();
        }

        m_window->Render();
        m_gui->Render();
        m_window->Present();

        const auto elapsedTime = duration<double>(
            steady_clock::now() - frameStartTime
        ).count();

        if(!m_gui->State.TurboEnabled) {
            std::this_thread::sleep_until(nextFrameTarget);
            nextFrameTarget += frameDuration - sleepLatency;

            if(nextFrameTarget < steady_clock::now() - frameDuration) {
                // Pause, or long stall. Reset the frame target.
                nextFrameTarget = steady_clock::now() + frameDuration - sleepLatency;
            }
        }

        const auto currentFrameTime = steady_clock::now();
        const auto currentFrameDuration = duration<double>(
            currentFrameTime - lastFrameTime
        ).count();

        averageWorkTime += elapsedTime;
        averageFrameTime += currentFrameDuration;
        if(++averageTimeCount > AverageTimeSampleSize) {
            const auto averageFrameTimeSec = averageFrameTime / AverageTimeSampleSize;

            // Use the average frame time to calculate the approximate sleep latency for this system.
            sleepLatencySec = averageFrameTimeSec - m_frameTime + sleepLatencySec;
            if(sleepLatencySec < -SleepLatencyErrorMargin || sleepLatencySec > SleepLatencyErrorMargin) {
                // The time is way off. Reset.
                sleepLatencySec = 0.0;
            }

            sleepLatency = duration_cast<nanoseconds>(duration<double>(sleepLatencySec));

            // Update GUI and reset work values.
            m_gui->State.AverageFrameTime = averageFrameTimeSec * 1000;
            m_gui->State.AverageWorkTime = averageWorkTime / AverageTimeSampleSize * 1000;
            averageTimeCount = 0;
            averageFrameTime = 0;
            averageWorkTime = 0;
        }

        lastFrameTime = currentFrameTime;
    }

    m_plip->GetCore()->Shutdown();
    audio->DequeueAll();
}

void Game::SetPaused(const bool paused) const {
    m_gui->State.PauseCore = paused;
}
