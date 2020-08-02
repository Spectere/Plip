/* PlipInstance.cpp
 *
 * The main class for controlling the Plip emulation suite.
 */

#include "PlipInstance.h"
#include "PlipVersion.h"

#include "Core/Chip8/Chip8Instance.h"
#include "Core/GameBoy/GameBoyInstance.h"

namespace Plip {
    PlipInstance::PlipInstance(PlipVideo *video, PlipAudio *audio) {
        m_audio = audio;
        m_video = video;
    }

    PlipAudio* PlipInstance::GetAudio() {
        return m_audio;
    }

    PlipCore* PlipInstance::GetCore() {
        return m_core;
    }

    PlipInput* PlipInstance::GetInput() {
        return m_input;
    }

    std::vector<PlipCoreDescription> PlipInstance::GetSupportedCores() {
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

    PlipVideo* PlipInstance::GetVideo() {
        return m_video;
    }

    PlipError PlipInstance::Load(PlipValidCore core, const std::string &path) {
        switch(core) {
            case PlipValidCore::Chip8:
                m_core = new Core::Chip8::Chip8Instance(m_audio, m_input, m_video);
                break;
            case PlipValidCore::GameBoy:
                m_core = new Core::GameBoy::GameBoyInstance(m_audio, m_input, m_video);
                break;
            default:
                return PlipError::InvalidCore;
        }

        return m_core->Load(path);
    }

    void PlipInstance::Run(long ns) {
        m_core->Delta(ns);
    }
}
