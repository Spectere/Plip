/* GameBoyInstance.cpp
 *
 * A GameBoy emulation core.
 */

#include <cstring>

#include "GameBoyInstance.h"

#include "../../PlipInitializationException.h"
#include "../../PlipIo.h"
#include "../../PlipUtility.h"

using Plip::Core::GameBoy::GameBoyInstance;

GameBoyInstance::GameBoyInstance(PlipAudio *audio, PlipInput *input, PlipVideo *video, const PlipKeyValuePairCollection &config)
: PlipCore(audio, input, video, config) {
    // Load the boot ROM.
    const std::string bootRomPath = m_config.GetValue("BootRom");

    if(bootRomPath.empty()) {
        throw PlipInitializationException("Boot ROM must be specified in the core config.");
    }
    if(!PlipIo::FileExists(bootRomPath)) {
        throw PlipInitializationException("Specified Boot ROM could not be found.");
    }

    const auto bootRomSize = PlipIo::GetSize(bootRomPath);
    const auto bootRomData = PlipIo::ReadFile(bootRomPath, bootRomSize);
    m_bootRom = new PlipMemoryRom(bootRomData.data(), bootRomSize);

    // Initialize framebuffer and video subsystem.
    m_video->ResizeOutput(ScreenWidth, ScreenHeight, 1.0, 1.0);
    m_videoFormat = PlipVideo::GetFormatInfo(video->GetFormat());
    m_videoBufferSize = m_videoFormat.pixelWidth * ScreenWidth * ScreenHeight;
    m_videoBuffer = new uint8_t[m_videoBufferSize];
    m_videoMode = VideoMode::OamSearch;

    // Paint the framebuffer white.
    memset(m_videoBuffer, 0xFF, m_videoBufferSize);

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
    delete m_videoBuffer;
}

void GameBoyInstance::BootRomFlagHandler() {
    if(m_memory->LastWrittenAddress != IoRegistersAddress + BootRomDisable) return;
    if(m_memory->LastWrittenValue == 0) return;

    // Swap the boot ROM out for the cartridge ROM, then update the register.
    m_bootRomDisableFlag = true;
    m_memory->AssignBlock(m_rom, 0x0000, 0x0000, 0x0100);
    m_ioRegisters->SetByte(BootRomDisable, 1);
}

void GameBoyInstance::Delta(long ns) {
    auto timeRemaining = ns;
    const auto cycleTime = m_cpu->GetCycleTime();

    do {
        const auto cpuTime = m_cpu->Cycle();

        if(!m_bootRomDisableFlag) {
            // See if we need to disable the boot ROM.
            BootRomFlagHandler();
        } else {
            // Keep the register set.
            m_ioRegisters->SetByte(BootRomDisable, 1);
        }

        // PPU
        VideoCycle();

        timeRemaining -= cpuTime;
    } while(cycleTime < timeRemaining);
}

std::map<std::string, std::map<std::string, Plip::DebugValue>> GameBoyInstance::GetDebugInfo() const {
    return {
        { "CPU Registers", m_cpu->GetRegisters() },
        { "CPU (Other)", m_cpu->GetDebugInfo() }
    };
}

Plip::PlipError GameBoyInstance::Load(const std::string &path) {
    if(!PlipIo::FileExists(path)) {
        return PlipError::FileNotFound;
    }

    // Load the ROM into 0x0000-0x8000.
    const auto size = PlipIo::GetSize(path);
    const auto data = PlipIo::ReadFile(path, size);
    m_rom = new PlipMemoryRom(data.data(), size);
    m_memory->AssignBlock(m_rom, BaseRomAddress, 0x0000, 0x8000);

    // Read the ROM header.
    ReadCartridgeFeatures();
    InitCartridgeRam();

    // Load the boot ROM into 0x0000-0x00FF (overlaying the cartridge ROM).
    m_memory->AssignBlock(m_bootRom, 0x0000, 0x0000, 0x0100);
    m_bootRomDisableFlag = false;

    return PlipError::Success;
}

void GameBoyInstance::InitCartridgeRam() {
    switch(const auto ramSize = m_rom->GetByte(CartRamSizeOffset)) {
        case 0x00:  // 0KB
        case 0x01:  // 2KB
        case 0x02:  // 8KB
            m_cartRamBanks = 0;
            break;

        case 0x03:  // 32KB
            m_cartRamBanks = 4;
            break;

        case 0x04:  // 128KB
            m_cartRamBanks = 16;
            break;

        case 0x05:  // 64KB
            m_cartRamBanks = 8;
            break;

        default: {
            std::stringstream ex;
            ex << "invalid cart RAM size byte: "
               << PlipUtility::FormatHex(ramSize, 2);
            throw PlipEmulationException(ex.str().c_str());
        }
    }

    if(m_cartRamBanks > 0) {
        m_cartRam = new PlipMemoryRam(8192 * m_cartRamBanks);
        m_memory->AssignBlock(m_cartRam, CartRamAddress, 0x0000, 0x2000);
    }
}

bool GameBoyInstance::IsPcAt(const uint64_t pc) const {
    return m_cpu->GetPc() == pc;
}

void GameBoyInstance::ReadCartridgeFeatures() {
    const auto cartType = m_rom->GetByte(0x0147);

    // MBC
    switch(cartType) {
        case 0x00: case 0x08: case 0x09:
            m_mbc = MemoryBankController::None;
            break;

        default: {
            std::stringstream ex;
            ex << "invalid/unsupported memory bank controller: "
               << PlipUtility::FormatHex(cartType, 2);
            throw PlipEmulationException(ex.str().c_str());
        }
    }

    // RAM
    switch(cartType) {
        case 0x02: case 0x03: case 0x08: case 0x09: case 0x0C: case 0x0D:
        case 0x0F: case 0x10: case 0x12: case 0x13: case 0x1A: case 0x1B:
        case 0x1D: case 0x1E: case 0x22: case 0xFF:
            m_hasRam = true;
            break;
        default:
            m_hasRam = false;
            break;
    }

    // Battery
    switch(cartType) {
        case 0x03: case 0x06: case 0x09: case 0x0D: case 0x0F: case 0x10:
        case 0x13: case 0x1B: case 0x1E: case 0x22: case 0xFF:
            m_hasBattery = true;
            break;
        default:
            m_hasBattery = false;
            break;
    }

    // RTC
    switch(cartType) {
        case 0x0F: case 0x10:
            m_hasRtc = true;
            break;
        default:
            m_hasRtc = false;
            break;
    }

    // Camera
    m_hasCamera = cartType == 0xFC;

    // Sensor
    m_hasSensor = cartType == 0x22;
}
