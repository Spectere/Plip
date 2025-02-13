// Gui.h
//
// Handles initializing, populating, and rendering the ImGui overlay.

#pragma once

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
        void SetEnabled(bool enable);

    private:
        bool m_enabled = false;
        SDL_Renderer* m_renderer;
    };
}
