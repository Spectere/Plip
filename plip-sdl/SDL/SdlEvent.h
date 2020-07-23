/* SdlEvent.h
 *
 * An SDL2 event handler.
 */

#pragma once

#include "Input/PlipInput.h"

namespace PlipSdl {
    enum class SdlUiEvent {
        None,
        Quit
    };

    class SdlEvent {
    public:
        explicit SdlEvent(Plip::PlipInput *input) : m_input(input) {};

        SdlUiEvent ProcessEvents();

    private:
        Plip::PlipInput *m_input;
    };
}
