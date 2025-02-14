/* SdlEvent.cpp
 *
 * An SDL2 event handler.
 */

#include <SDL3/SDL.h>

#include "SdlEvent.h"

#include "PlipUtility.h"

namespace PlipSdl {
    void SdlEvent::AddDigitalBinding(int id, SDL_Scancode scancode) {
        m_digitalBinding.insert(std::pair(scancode, id));
    }

    void SdlEvent::AddDigitalBinding(const int id, const std::string &binding) {
        const auto scancode = SDL_GetScancodeFromName(binding.c_str());
        AddDigitalBinding(id, scancode);
    }

    std::vector<PlipUiEvent> SdlEvent::ProcessEvents() {
        SDL_Event ev;
        auto uiEvents = std::vector<PlipUiEvent>();

        while(SDL_PollEvent(&ev)) {
            switch(ev.type) {
                case SDL_EVENT_KEY_DOWN:
                    if(ev.key.scancode == m_guiKey) {
                        uiEvents.push_back(PlipUiEvent::ToggleGui);
                    } else if(ev.key.scancode == m_pauseKey) {
                        uiEvents.push_back(PlipUiEvent::TogglePause);
                    } else if(ev.key.scancode == m_stepKey) {
                        uiEvents.push_back( PlipUiEvent::Step);
                    } else if(ev.key.scancode == m_turboKey) {
                        uiEvents.push_back(PlipUiEvent::TurboEnable);
                    } else {
                        UpdateDigitalInput(ev.key.scancode, true);
                    }
                    break;

                case SDL_EVENT_KEY_UP:
                    if(ev.key.scancode == m_turboKey) {
                        uiEvents.push_back( PlipUiEvent::TurboDisable);
                    } else {
                        UpdateDigitalInput(ev.key.scancode, false);
                    }
                    break;

                case SDL_EVENT_QUIT:
                    uiEvents.push_back(PlipUiEvent::Quit);
                    break;

                case SDL_EVENT_WINDOW_RESIZED:
                    uiEvents.push_back(PlipUiEvent::WindowResized);
                    break;

                default:
                    break;
            }

            m_gui->SendEvent(ev);
        }

        return uiEvents;
    }

    void SdlEvent::SetKey(const std::string &action, SDL_Scancode scancode) {
        const auto lower = Plip::PlipUtility::StringToLower(action);

        if(lower == "gui") m_guiKey = scancode;
        if(lower == "pause") m_pauseKey = scancode;
        if(lower == "step") m_stepKey = scancode;
        if(lower == "turbo") m_turboKey = scancode;
    }

    void SdlEvent::SetKey(const std::string &action, const std::string &binding) {
        const auto scancode = SDL_GetScancodeFromName(binding.c_str());
        SetKey(action, scancode);
    }

    void SdlEvent::UpdateDigitalInput(const SDL_Scancode scancode, const bool value) {
        const auto it = m_digitalBinding.find(scancode);
        if(it == m_digitalBinding.cend()) return;
        m_input->UpdateInput(it->second, { .digital = value });
    }
}
