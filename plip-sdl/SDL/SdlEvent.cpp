/* SdlEvent.cpp
 *
 * An SDL2 event handler.
 */

#include <SDL.h>

#include "SdlEvent.h"

namespace PlipSdl {
    SdlUiEvent SdlEvent::ProcessEvents() {
        SDL_Event ev;
        auto uiEvent = SdlUiEvent::None;

        while(SDL_PollEvent(&ev)) {
            switch(ev.type) {
                case SDL_QUIT:
                    uiEvent = SdlUiEvent::Quit;
                    break;
            }
        }

        return uiEvent;
    }
}
