/* PlipCore.cpp
 *
 * Defines an emulation core.
 */

#include "PlipCore.h"

namespace Plip {
    PlipCore::PlipCore(PlipAudio *audio, PlipInput *input, PlipVideo *video, PlipConfig *config) {
        m_audio = audio;
        m_config = config;
        m_input = input;
        m_video = video;
    }

    std::vector<PlipCoreDescription> PlipCore::GetSupportedCores() {
        std::vector<PlipCoreDescription> coreList;

        for(auto core : m_supportedCores)
            coreList.push_back(core);

        return coreList;
    }
}
