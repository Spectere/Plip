/* SdlEvent.cpp
 *
 * An SDL2 event handler.
 */

#include <SDL3/SDL.h>

#include "SdlEvent.h"

namespace PlipSdl {
    void SdlEvent::AddDigitalBinding(int id, SDL_Scancode scancode) {
        m_digitalBinding.insert(std::pair<SDL_Scancode, int>(scancode, id));
    }

    void SdlEvent::AddDigitalBinding(int id, const std::string &binding) {
        auto scancode = SDL_GetScancodeFromName(binding.c_str());
        AddDigitalBinding(id, scancode);
    }

    SdlUiEvent SdlEvent::ProcessEvents() {
        SDL_Event ev;
        auto uiEvent = SdlUiEvent::None;

        while(SDL_PollEvent(&ev)) {
            switch(ev.type) {
                case SDL_EVENT_KEY_DOWN:
                    UpdateDigitalInput(ev.key.scancode, true);
                    break;

                case SDL_EVENT_KEY_UP:
                    UpdateDigitalInput(ev.key.scancode, false);
                    break;

                case SDL_EVENT_QUIT:
                    uiEvent = SdlUiEvent::Quit;
                    break;
            }
        }

        return uiEvent;
    }

    void SdlEvent::UpdateDigitalInput(SDL_Scancode scancode, bool value) {
        auto it = m_digitalBinding.find(scancode);
        if(it == m_digitalBinding.cend()) return;
        m_input->UpdateInput(it->second, { .digital = value });
    }
}
