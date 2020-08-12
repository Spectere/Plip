/* SdlEvent.h
 *
 * An SDL2 event handler.
 */

#pragma once

#include <SDL.h>

#include "Input/PlipInput.h"

namespace PlipSdl {
    enum class SdlUiEvent {
        None,
        ToggleConsole,
        Quit
    };

    class SdlEvent {
    public:
        explicit SdlEvent(Plip::PlipInput *input) : m_input(input) {};

        void AddDigitalBinding(int id, SDL_Scancode scancode);
        void AddDigitalBinding(int id, const std::string &binding);
        void SetConsoleKey(SDL_Scancode scancode);
        void SetConsoleKey(const std::string &binding);
        SdlUiEvent ProcessEvents();

    private:
        void UpdateDigitalInput(SDL_Scancode scancode, bool value);

        SDL_Scancode m_consoleKey;
        std::unordered_map<SDL_Scancode, int> m_digitalBinding;
        Plip::PlipInput *m_input;
    };
}
