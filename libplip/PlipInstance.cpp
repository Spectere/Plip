/* PlipInstance.cpp
 *
 * The main class for controlling the Plip emulation suite.
 */

#include "PlipInstance.h"
#include "PlipVersion.h"

#include "Core/Chip8/Chip8Instance.h"

using Plip::PlipInstance;

PlipInstance::PlipInstance(PlipVideo *video, PlipAudio *audio) {
    m_audio = audio;
    m_video = video;
}

Plip::PlipAudio* PlipInstance::GetAudio() const {
    return m_audio;
}

Plip::PlipCore* PlipInstance::GetCore() const {
    return m_core;
}

Plip::PlipInput* PlipInstance::GetInput() const {
    return m_input;
}

std::vector<Plip::PlipCoreDescription> PlipInstance::GetSupportedCores() {
    return PlipCore::GetSupportedCores();
}

std::string PlipInstance::GetVersion() {
    #ifndef GIT_FOUND
        return PRODUCT_NAME;
    #else
        #ifndef GIT_TAG
            return PRODUCT_NAME " (" GIT_BRANCH "@" GIT_REVISION ")";
        #else
            return PRODUCT_NAME " " GIT_TAG;
        #endif // GIT_TAG
    #endif // GIT_FOUND
}

Plip::PlipVideo* PlipInstance::GetVideo() const {
    return m_video;
}

Plip::PlipError PlipInstance::Load(const PlipValidCore core, const std::string &path, const PlipKeyValuePairCollection &config) {
    switch(core) {
        case PlipValidCore::Chip8:
            m_core = new Core::Chip8::Chip8Instance(m_audio, m_input, m_video, config);
            break;
        default:
            return PlipError::InvalidCore;
    }

    return m_core->Load(path);
}

void PlipInstance::Run(const long ns) const {
    m_core->Delta(ns);
}
