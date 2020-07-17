/* SdlEvent.h
 *
 * An SDL2 event handler.
 */

#pragma once

#include "PlipInput.h"

namespace PlipSdl {
    class SdlEvent : protected Plip::PlipInput {
    public:
        SdlEvent() = default;
    };
}
