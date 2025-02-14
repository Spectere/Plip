// Gui.h
//
// Handles initializing, populating, and rendering the ImGui overlay.

#pragma once

#include <map>

#include "PlipUiEvent.h"
#include "Cpu/PlipCpu.h"
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
        void SetDebugInfo(std::map<std::string, std::map<std::string, Plip::Cpu::RegisterValue>> debugInfo);
        void SetEnabled(bool enable);
        PlipUiEvent Update();

    private:
        bool m_enabled = false;
        SDL_Renderer* m_renderer;

        std::map<std::string, std::map<std::string, Plip::Cpu::RegisterValue>> m_debugInfo;
    };
}
