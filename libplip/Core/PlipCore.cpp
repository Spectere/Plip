/* PlipCore.cpp
 *
 * Defines an emulation core.
 */

#include "PlipCore.h"

#include <utility>

#include "../PlipKeyValuePairCollection.h"

namespace Plip {
    PlipCore::PlipCore(PlipAudio *audio, PlipInput *input, PlipVideo *video, PlipKeyValuePairCollection config)
        : m_audio(audio), m_input(input), m_video(video), m_config(std::move(config)) {}

    std::vector<PlipCoreDescription> PlipCore::GetSupportedCores() {
        std::vector<PlipCoreDescription> coreList;

        for(auto core : m_supportedCores)
            coreList.push_back(core);

        return coreList;
    }
}
