/* PlipInstance.h
 *
 * The main class for controlling the Plip emulation suite.
 */

#pragma once

#include <string>

#include "PlipError.h"
#include "Core/PlipCore.h"
#include "Audio/PlipAudio.h"
#include "Input/PlipInput.h"
#include "Video/PlipVideo.h"

namespace Plip {
    class PlipInstance final {
    public:
        PlipInstance(PlipVideo *video, PlipAudio *audio);

        static std::string GetVersion();
        static std::vector<PlipCoreDescription> GetSupportedCores();

        [[nodiscard]] PlipAudio* GetAudio() const;
        [[nodiscard]] PlipCore* GetCore() const;
        [[nodiscard]] PlipInput* GetInput() const;
        [[nodiscard]] PlipVideo* GetVideo() const;
        PlipError Load(PlipValidCore core, const std::string &path);
        void Run(long ns) const;

    private:
        PlipAudio *m_audio;
        PlipCore *m_core = nullptr;
        PlipInput *m_input = new PlipInput();
        PlipVideo *m_video;
    };
}
