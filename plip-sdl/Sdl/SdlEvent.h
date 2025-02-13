/* SdlEvent.h
 *
 * An SDL2 event handler.
 */

#pragma once

#include <vector>

#include <SDL3/SDL.h>

#include "../Gui.h"
#include "Input/PlipInput.h"

namespace PlipSdl {
    enum class PlipSdlEvent {
        Quit,
        WindowResized,
        ToggleGui,
        TogglePause,
        TurboEnable,
        TurboDisable,
        Step,
    };

    class SdlEvent {
    public:
        SdlEvent(Plip::PlipInput* input, Gui* gui) : m_input(input), m_gui(gui) {}

        void AddDigitalBinding(int id, SDL_Scancode scancode);
        void AddDigitalBinding(int id, const std::string &binding);
        void SetKey(const std::string &action, SDL_Scancode scancode);
        void SetKey(const std::string &action, const std::string &binding);
        std::vector<PlipSdlEvent> ProcessEvents();

    private:
        void UpdateDigitalInput(SDL_Scancode scancode, bool value);

        std::unordered_map<SDL_Scancode, int> m_digitalBinding;
        Plip::PlipInput *m_input;

        SDL_Scancode m_pauseKey {};
        SDL_Scancode m_stepKey {};
        SDL_Scancode m_guiKey {};
        SDL_Scancode m_turboKey {};

        Gui* m_gui;
    };
}
