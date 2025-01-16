/* SdlEvent.h
 *
 * An SDL2 event handler.
 */

#pragma once

#include <SDL3/SDL.h>

#include "Input/PlipInput.h"

namespace PlipSdl {
    enum class SdlUiEvent {
        None,
        Quit,
        WindowResized,
    };

    class SdlEvent {
    public:
        explicit SdlEvent(Plip::PlipInput *input) : m_input(input) {}

        void AddDigitalBinding(int id, SDL_Scancode scancode);
        void AddDigitalBinding(int id, const std::string &binding);
        SdlUiEvent ProcessEvents();

    private:
        void UpdateDigitalInput(SDL_Scancode scancode, bool value);

        std::unordered_map<SDL_Scancode, int> m_digitalBinding;
        Plip::PlipInput *m_input;
    };
}
