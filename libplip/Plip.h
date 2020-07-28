/* Plip.h
 *
 * The main class for controlling the Plip emulation suite.
 */

#pragma once

#include <string>

#include "PlipCore.h"
#include "PlipError.h"
#include "Input/PlipInput.h"
#include "Video/PlipVideo.h"

namespace Plip {
    class Plip {
    public:
        explicit Plip(PlipVideo *video);

        static std::string GetVersion();

        PlipInput* GetInput();
        PlipVideo* GetVideo();
        PlipError Load(PlipValidCore core, const std::string &path);

    private:
        PlipCore *m_core = nullptr;
        PlipInput *m_input = new PlipInput();
        PlipVideo *m_video;
    };
}
