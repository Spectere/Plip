// Gui.h
//
// Handles initializing, populating, and rendering the ImGui overlay.

#pragma once

#include <map>

#include "DebugValue.h"
#include "GuiState.h"

#include "Sdl/SdlWindow.h"

namespace PlipSdl {
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
