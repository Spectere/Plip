/* GameBoyInstance.h
 *
 * A GameBoy core for Plip.
 */

#pragma once

#include "../PlipCore.h"
#include "../../Cpu/SharpLr35902/SharpLr35902.h"
#include "../../Memory/PlipMemoryMap.h"
#include "../../Memory/PlipMemoryRam.h"
#include "../../Memory/PlipMemoryRom.h"

namespace Plip::Core::GameBoy {
    class GameBoyInstance : public PlipCore {
    public:
        GameBoyInstance(PlipAudio *audio, PlipInput *input, PlipVideo *video);

        void Delta(long ns) override;
        PlipError Load(const std::string &path) override;

        const int ClockRate = 4194304 / 4;

    private:
        Plip::Cpu::SharpLr35902 *m_cpu;

        Plip::PlipMemoryRam *m_cartRam;
        Plip::PlipMemoryRam *m_workRam;
        Plip::PlipMemoryRom *m_rom;
    };
}
