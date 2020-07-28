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
    class Plip final {
    public:
        explicit Plip(PlipVideo *video);

        static std::string GetVersion();
        static std::vector<PlipCoreDescription> GetSupportedCores();

        PlipCore* GetCore();
        PlipInput* GetInput();
        PlipVideo* GetVideo();
        PlipError Load(PlipValidCore core, const std::string &path);
        void Run(long ns);

    private:
        PlipCore *m_core = nullptr;
        PlipInput *m_input = new PlipInput();
        PlipVideo *m_video;
    };
}
