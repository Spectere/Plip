/* Plip.h
 *
 * The main class for controlling the Plip emulation suite.
 */

#pragma once

#include <string>

#include "Input/PlipInput.h"
#include "Video/PlipVideo.h"

namespace Plip {
    class Plip {
    public:
        explicit Plip(PlipVideo *video);

        static std::string GetVersion();

        PlipInput* GetInput();
        PlipVideo* GetVideo();

    private:
        PlipInput *m_input = new PlipInput();
        PlipVideo *m_video;
    };
}
