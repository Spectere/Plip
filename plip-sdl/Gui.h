// Gui.h
//
// Handles initializing, populating, and rendering the ImGui overlay.

#pragma once

#include <map>

#include "DebugValue.h"

#include "PlipUiEvent.h"
#include "Sdl/SdlWindow.h"

namespace PlipSdl {
    struct GuiState {
        bool GuiShown = false;

        // Emulator state.
        bool PauseCore = false;
        bool SingleStep = false;

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
        void DrawCoreDebugInfo();
        void DrawEmulatorControls();
        void DrawMemoryTools();

        SDL_Renderer* m_renderer;

        std::map<std::string, std::map<std::string, Plip::DebugValue>> m_debugInfo;
    };
}
