/* GameBoyInstance.cpp
 *
 * A GameBoy core for Plip.
 */

#include "GameBoyInstance.h"

namespace Plip::Core::GameBoy {
    GameBoyInstance::GameBoyInstance(PlipAudio *audio, PlipInput *input, PlipVideo *video)
    : PlipCore(audio, input, video) {
        m_cpu = new Cpu::SharpLr35902(ClockRate, m_memory);
    }

    void GameBoyInstance::Delta(long ns) {}

    PlipError GameBoyInstance::Load(const std::string &path) {
        return PlipError::Success;
    }
}
