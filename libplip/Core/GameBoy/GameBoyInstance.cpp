/* GameBoyInstance.cpp
 *
 * A GameBoy emulation core.
 */

#include "GameBoyInstance.h"

#include "../../PlipInitializationException.h"
#include "../../PlipIo.h"
#include "../../PlipUtility.h"

using Plip::Core::GameBoy::GameBoyInstance;

#define READ_INPUT(idx) { \
    if(m_input->GetInput(idx).digital) \
        m_keypad |= 1 << idx; \
}

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
    m_bootRom = new PlipMemoryRom(bootRomData.data(), bootRomSize, 0xFF);

    // Initialize framebuffer and video subsystem.
    m_video->ResizeOutput(ScreenWidth, ScreenHeight, 1.0, 1.0);
    m_videoFormat = PlipVideo::GetFormatInfo(video->GetFormat());
    m_videoBufferSize = m_videoFormat.pixelWidth * ScreenWidth * ScreenHeight;
    m_videoBuffer = new uint8_t[m_videoBufferSize];

    // Initialize input.
    RegisterInput();

    // Invalid and unreadable memory values are generally pulled high.
    m_memory->SetInvalidByte(0xFF);

    // Create CPU.
    m_cpu = new Cpu::SharpLr35902(BaseClockRate / 4, m_memory);
}

GameBoyInstance::~GameBoyInstance() {
    delete m_cpu;
    delete m_videoBuffer;
}

void GameBoyInstance::BootRomFlagHandler() {
    if(m_memory->LastWrittenAddress != IoRegistersAddress + IOReg_BootRomDisable) return;
    if(m_memory->LastWrittenValue == 0) return;

    // Swap the boot ROM out for the cartridge ROM, then update the register.
    m_bootRomDisableFlag = true;
    m_memory->AssignBlock(m_rom, 0x0000, 0x0000, 0x0100);
    m_ioRegisters->SetByte(IOReg_BootRomDisable, 1);
}

void GameBoyInstance::Delta(const long ns) {
    auto timeRemaining = ns;
    const auto cycleTime = m_cpu->GetCycleTime();

    ReadJoypad();
    ClearActiveBreakpoint();

    do {
        // Run CPU for one cycle.
        m_cpu->Cycle();

        // Handle ROM disable flag.
        if(!m_bootRomDisableFlag) {
            // See if we need to disable the boot ROM.
            BootRomFlagHandler();
        } else {
            // Keep the register set.
            m_ioRegisters->SetByte(IOReg_BootRomDisable, 1);
        }

        // Timer
        Timer_Cycle();

        // Input
        InputRegisterHandler();

        // MBC
        MBC_Cycle();

        // PPU
        PPU_Cycle();
        
        // Hold the I/O registers at expected values.
        UndefinedRegisters();

        // Breakpoints
        if(!m_breakpoints.empty()) {
            auto bp = std::find(m_breakpoints.begin(), m_breakpoints.end(), m_cpu->GetPc());
            if(bp != m_breakpoints.end()) {
                SetActiveBreakpoint(*bp);
                break;
            }
        }

        timeRemaining -= cycleTime;
    } while(cycleTime < timeRemaining);
}

std::map<std::string, std::map<std::string, Plip::DebugValue>> GameBoyInstance::GetDebugInfo() const {
    return {
        { "CPU Registers", m_cpu->GetRegisters() },
        { "CPU (Other)", m_cpu->GetDebugInfo() },
        { "MBC", MBC_GetDebugInfo() },
        { "PPU", PPU_GetDebugInfo() },
        { "Timer", Timer_GetDebugInfo() },
        { "System", {
            { "Keypad", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_keypad)) },
            { "Boot ROM Enabled", DebugValue(!m_bootRomDisableFlag) },
        }}
    };
}

std::vector<uint64_t> GameBoyInstance::GetPcs() const {
    return { m_cpu->GetPc() };
}

Plip::PlipError GameBoyInstance::Load(const std::string &path) {
    if(!PlipIo::FileExists(path)) {
        return PlipError::FileNotFound;
    }

    // Load the ROM.
    const auto size = PlipIo::GetSize(path);
    const auto data = PlipIo::ReadFile(path, size);
    m_rom = new PlipMemoryRom(data.data(), size, 0xFF);

    // Read the ROM header.
    ReadCartridgeFeatures();
    InitCartridgeRam();

    // Update titlebar.
    m_video->SetTitle("GameBoy: " + PlipIo::GetFilename(path));

    // Reset system.
    Reset();

    return PlipError::Success;
}

void GameBoyInstance::InitCartridgeRam() {
    if(!m_hasCartRam) return;

    switch(const auto ramSize = m_rom->GetByte(CartRamSizeOffset)) {
        case 0x00:  // 0KB
        case 0x01:  // 2KB
        case 0x02:  // 8KB
            m_cartRamBanks = 1;
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

    m_cartRam = new PlipMemoryRam(8192 * m_cartRamBanks, 0xFF);
    m_memory->AssignBlock(m_cartRam, CartRamAddress, 0x0000, 0x2000);
}

void GameBoyInstance::InputRegisterHandler() const {
    auto inputRegister = m_ioRegisters->GetByte(IOReg_JoypadInput);

    inputRegister |= 0b11001111;
    if(!BIT_TEST(inputRegister, 5)) {
        // Read button keys.
        inputRegister ^= m_keypad & 0b1111;
    } else if(!BIT_TEST(inputRegister, 4)) {
        // Read d-pad.
        inputRegister ^= m_keypad >> 4;
    }
    m_ioRegisters->SetByte(IOReg_JoypadInput, inputRegister);
}

void GameBoyInstance::ReadCartridgeFeatures() {
    const auto cartType = m_rom->GetByte(0x0147);

    // MBC
    switch(cartType) {
        case 0x00: case 0x08: case 0x09:
            m_mbc = MBC_Type::None;
            break;

        case 0x01: case 0x02: case 0x03:
            m_mbc = MBC_Type::Mbc1;
            break;

        case 0x05: case 0x06:
            m_mbc = MBC_Type::Mbc2;
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
            m_hasCartRam = true;
            break;
        default:
            m_hasCartRam = false;
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

void GameBoyInstance::RaiseInterrupt(const Cpu::SharpLr35902Interrupt interrupt) const {
    const auto interruptFlag = m_ioRegisters->GetByte(IOReg_InterruptFlag);
    m_ioRegisters->SetByte(IOReg_InterruptFlag, interruptFlag | static_cast<int>(interrupt));
}

void GameBoyInstance::ReadJoypad() {
    m_keypad = 0;
    READ_INPUT(InputRight);
    READ_INPUT(InputLeft);
    READ_INPUT(InputUp);
    READ_INPUT(InputDown);
    READ_INPUT(InputA);
    READ_INPUT(InputB);
    READ_INPUT(InputSelect);
    READ_INPUT(InputStart);
}

void GameBoyInstance::RegisterInput() const {
    m_input->AddInput(InputA, PlipInputDefinition(PlipInputType::Digital, "A"), { .digital = false });
    m_input->AddInput(InputB, PlipInputDefinition(PlipInputType::Digital, "B"), { .digital = false });
    m_input->AddInput(InputSelect, PlipInputDefinition(PlipInputType::Digital, "Select"), { .digital = false });
    m_input->AddInput(InputStart, PlipInputDefinition(PlipInputType::Digital, "Start"), { .digital = false });
    m_input->AddInput(InputRight, PlipInputDefinition(PlipInputType::Digital, "Right"), { .digital = false });
    m_input->AddInput(InputLeft, PlipInputDefinition(PlipInputType::Digital, "Left"), { .digital = false });
    m_input->AddInput(InputUp, PlipInputDefinition(PlipInputType::Digital, "Up"), { .digital = false });
    m_input->AddInput(InputDown, PlipInputDefinition(PlipInputType::Digital, "Down"), { .digital = false });
}

void GameBoyInstance::RegisterWriteServiced() const {
    // Clear the last written values to ensure that the request isn't immediately
    // serviced again.
    m_memory->LastWrittenAddress = 0xFFFF;
    m_memory->LastWrittenValue = 0;
}

void GameBoyInstance::Reset() {
    // Clear RAM and I/O registers.
    for(auto i = 0; i < m_workRam->GetLength(); i++)
        m_workRam->SetByte(i, 0);

    for(auto i = 0; i < m_videoRam->GetLength(); i++)
        m_videoRam->SetByte(i, 0);

    for(auto i = 0; i < m_oam->GetLength(); i++)
        m_oam->SetByte(i, 0);

    for(auto i = 0; i < m_ioRegisters->GetLength(); i++)
        m_ioRegisters->SetByte(i, 0);

    for(auto i = 0; i < m_highRam->GetLength(); i++)
        m_highRam->SetByte(i, 0);

    if(m_cartRamBanks > 0)
        for(auto i = 0; i < m_cartRam->GetLength(); i++)
            m_cartRam->SetByte(i, 0);

    // Initialize system memory map.
    m_memory->AssignBlock(m_videoRam, VideoRamAddress);
    m_memory->AssignBlock(m_workRam, WorkRamAddress);
    m_memory->AssignBlock(m_oam, OamAddress);
    m_memory->AssignBlock(m_unusable, UnusableAddress);
    m_memory->AssignBlock(m_ioRegisters, IoRegistersAddress);
    m_memory->AssignBlock(m_highRam, HighRamAddress);
    m_memory->AssignBlock(m_rom, RomBank0Address, RomBank0Address, RomBank0Length + RomBank1Length);

    // Load the boot ROM into 0x0000-0x00FF (overlaying the cartridge ROM).
    m_memory->AssignBlock(m_bootRom, 0x0000, 0x0000, 0x0100);
    m_bootRomDisableFlag = false;

    // Reset CPU.
    m_cpu->Reset(0x0000);

    // Reset PPU.
    PPU_Reset();

    // Reset timer.
    Timer_Init();

    // Perform any necessary MBC initialization.
    MBC_Init();

    // Reset I/O registers.
    ResetIoRegisters();
    RegisterWriteServiced();
}

void GameBoyInstance::ResetIoRegisters() const {
    m_ioRegisters->SetByte(IOReg_InterruptFlag, 0b11100000);
}

void GameBoyInstance::UndefinedRegisters() const {
    /*
     * Sets all undefined/unimplemented registers to the appropriate values.
     */

    // Serial
    m_ioRegisters->SetByte(IOReg_SerialControl, 0b01111110);

    // Undefined
    m_ioRegisters->SetByte(0x03, 0b11111111);
    m_ioRegisters->SetByte(0x08, 0b11111111);
    m_ioRegisters->SetByte(0x09, 0b11111111);
    m_ioRegisters->SetByte(0x0A, 0b11111111);
    m_ioRegisters->SetByte(0x0B, 0b11111111);
    m_ioRegisters->SetByte(0x0C, 0b11111111);
    m_ioRegisters->SetByte(0x0D, 0b11111111);
    m_ioRegisters->SetByte(0x0E, 0b11111111);
}

