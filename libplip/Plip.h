/* Plip.h
 *
 * The main class for controlling the Plip emulation suite.
 */

#pragma once

#include <string>

#include "PlipInput.h"
#include "PlipVideo.h"

namespace Plip {
    class Plip {
    public:
        Plip(PlipInput *input, PlipVideo *video);

        static std::string GetVersion();

        PlipInput* GetInput();
        PlipVideo* GetVideo();

    private:
        PlipInput *m_input;
        PlipVideo *m_video;
    };
}
