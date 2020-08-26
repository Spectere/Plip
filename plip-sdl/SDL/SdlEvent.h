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
        FrameAdvance,
        PlayPause,
        Step,
        ToggleConsole,
        Quit
    };

    class SdlEvent {
    public:
        explicit SdlEvent(Plip::PlipInput *input) : m_input(input) {};

        void AddDigitalBinding(int id, SDL_Scancode scancode);
        void AddDigitalBinding(int id, const std::string &binding);
        void SetKey(const std::string &action, SDL_Scancode scancode);
        void SetKey(const std::string &action, const std::string &binding);
        SdlUiEvent ProcessEvents();

    private:
        static constexpr unsigned int Hash(const char *str, int h = 0) {
            return !str[h] ? 5381 : (Hash(str, h + 1) * 33) ^ str[h];
        }

        void UpdateDigitalInput(SDL_Scancode scancode, bool value);

        SDL_Scancode m_consoleKey {};
        SDL_Scancode m_frameAdvanceKey {};
        SDL_Scancode m_pauseKey {};
        SDL_Scancode m_stepKey {};
        std::unordered_map<SDL_Scancode, int> m_digitalBinding;
        Plip::PlipInput *m_input;
    };
}
