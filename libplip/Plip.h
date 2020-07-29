/* Plip.h
 *
 * The main class for controlling the Plip emulation suite.
 */

#pragma once

#include <string>

#include "PlipCore.h"
#include "PlipError.h"
#include "Audio/PlipAudio.h"
#include "Input/PlipInput.h"
#include "Video/PlipVideo.h"

namespace Plip {
    class Plip final {
    public:
        explicit Plip(PlipVideo *video, PlipAudio *audio);

        static std::string GetVersion();
        static std::vector<PlipCoreDescription> GetSupportedCores();

        PlipAudio* GetAudio();
        PlipCore* GetCore();
        PlipInput* GetInput();
        PlipVideo* GetVideo();
        PlipError Load(PlipValidCore core, const std::string &path);
        void Run(long ns);

    private:
        PlipAudio *m_audio;
        PlipCore *m_core = nullptr;
        PlipInput *m_input = new PlipInput();
        PlipVideo *m_video;
    };
}
