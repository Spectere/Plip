/* GameBoyInstance.cpp
 *
 * A GameBoy emulation core.
 */

#include "GameBoyInstance.h"

#include "../../PlipInitializationException.h"
#include "../../PlipIo.h"

using Plip::Core::GameBoy::GameBoyInstance;

GameBoyInstance::GameBoyInstance(PlipAudio *audio, PlipInput *input, PlipVideo *video, const PlipKeyValuePairCollection &config)
: PlipCore(audio, input, video, config) {
    // Load the boot ROM.
    const std::string bootRomPath = m_config.GetValue("bootRom");

    if(bootRomPath.empty()) {
        throw PlipInitializationException("Boot ROM must be specified in the core config.");
    }
    if(!PlipIo::FileExists(bootRomPath)) {
        throw PlipInitializationException("Specified Boot ROM could not be found.");
    }

    const auto bootRomSize = PlipIo::GetSize(bootRomPath);
    const auto bootRomData = PlipIo::ReadFile(bootRomPath, bootRomSize);
    m_bootRom = new PlipMemoryRom(bootRomData.data(), bootRomSize);

    // Initialize system memory map.
    m_memory->AssignBlock(m_videoRam, VideoRamAddress);
    m_memory->AssignBlock(m_workRam, WorkRamAddress);
    m_memory->AssignBlock(m_oam, OamAddress);
    m_memory->AssignBlock(m_unusable, UnusableAddress);
    m_memory->AssignBlock(m_ioRegisters, IoRegistersAddress);
    m_memory->AssignBlock(m_highRam, HighRamAddress);

    // Initialize CPU.
    m_cpu = new Cpu::SharpLr35902(BaseClockRate / 4, m_memory);
}

GameBoyInstance::~GameBoyInstance() {
    delete m_cpu;
}

void GameBoyInstance::Delta(long ns) {
    throw PlipInitializationException("GameBoyInstance::Delta not implemented.");
}

std::map<std::string, std::map<std::string, Plip::DebugValue>> GameBoyInstance::GetDebugInfo() const {
    return {
        { "CPU", m_cpu->GetRegisters() }
    };
}

Plip::PlipError GameBoyInstance::Load(const std::string &path) {
    throw PlipInitializationException("GameBoyInstance::Load not implemented.");
}

bool GameBoyInstance::IsPcAt(const uint64_t pc) const {
    return m_cpu->GetPc() == pc;
}
