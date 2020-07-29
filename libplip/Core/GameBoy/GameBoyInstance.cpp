/* GameBoyInstance.cpp
 *
 * A GameBoy core for Plip.
 */

#include "GameBoyInstance.h"

namespace Plip::Core::GameBoy {
    GameBoyInstance::GameBoyInstance(PlipAudio *audio, PlipInput *input, PlipVideo *video)
    : PlipCore(audio, input, video) {}

    void GameBoyInstance::Delta(long ns) {}

    PlipError GameBoyInstance::Load(const std::string &path) {
        return PlipError::Success;
    }
}
