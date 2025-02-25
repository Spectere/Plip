// Gui.h
//
// Handles initializing, populating, and rendering the ImGui overlay.

#pragma once

#include <map>
#include <set>

#include "DebugValue.h"

#include "Sdl/SdlWindow.h"

namespace PlipSdl {
    struct GuiState {
        bool GuiShown = false;

        // Emulator state.
        bool PauseCore = false;
        bool SingleStep = false;
        bool TurboEnabled = false;
        long AverageFrameTime = 0;

        // Memory display.
        bool PerformRead = false;

        constexpr static int MemoryDisplayColumns = 16;
        constexpr static int MemoryDisplayRows = 3;
        uint32_t ReadAddress {};
        uint32_t MemoryDisplayBase {};
        uint8_t MemoryContents[MemoryDisplayColumns * MemoryDisplayRows] {};

        // Memory manipulation.
        bool PerformWrite = false;

        uint32_t WriteAddress {};
        uint8_t WriteValue {};

        // Breakpoints.
        std::set<uint32_t> Breakpoints {};
        bool BreakpointsActive = false;
        uint32_t BreakpointHit = UINT32_MAX;
    };

    class Gui {
    public:
        explicit Gui(const SdlWindow* sdlWindow);
        ~Gui();

        [[nodiscard]] bool GetEnabled() const;
        void NewFrame() const;
        void Render() const;
        void SendEvent(const SDL_Event &event) const;
        void SetDebugInfo(std::map<std::string, std::map<std::string, Plip::DebugValue>> debugInfo);
        void SetEnabled(bool enable);
        void Update();

        GuiState State = {};

    private:
        void DrawBreakpointControls();
        void DrawCoreDebugInfo();
        void DrawEmulatorControls();
        void DrawMemoryTools();

        SDL_Renderer* m_renderer;

        std::map<std::string, std::map<std::string, Plip::DebugValue>> m_debugInfo;
    };
}
