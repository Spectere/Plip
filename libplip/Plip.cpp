/* Plip.cpp
 *
 * The main class for controlling the Plip emulation suite.
 */

#include "Plip.h"
#include "PlipVersion.h"

#include "Core/PlipChip8.h"

namespace Plip {
    Plip::Plip(PlipVideo *video) {
        m_video = video;
    }

    PlipInput* Plip::GetInput() {
        return m_input;
    }

    std::string Plip::GetVersion() {
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

    PlipVideo* Plip::GetVideo() {
        return m_video;
    }

    PlipError Plip::Load(PlipValidCore core, const std::string &path) {
        switch(core) {
            case PlipValidCore::Chip8:
                m_core = new Core::PlipChip8(m_input);
                break;
            default:
                return PlipError::InvalidCore;
        }

        return m_core->Load(path);
    }
}
