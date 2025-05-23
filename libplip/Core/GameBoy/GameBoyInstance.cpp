/* GameBoyInstance.cpp
 *
 * A Game Boy emulation core.
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

    // Get the model that we should emulate and set up its I/O registers.
    if(const auto gbModel = m_config.GetValue("Model"); gbModel == "cgb") {
        m_model = GameBoyModel::CGB;
        m_cgbMode = true;
    } else {
        m_model = GameBoyModel::DMG;
    }

    // Set up memory objects.
    if(m_model == GameBoyModel::CGB) {
        m_videoRam = new PlipMemoryRam(0x4000, 0xFF);
        m_workRam = new PlipMemoryRam(0x8000, 0xFF);
    } else {
        m_videoRam = new PlipMemoryRam(0x2000, 0xFF);
        m_workRam = new PlipMemoryRam(0x2000, 0xFF);
    }
    m_ioRegisters = new GameBoyIoRegisters(m_model, m_ppuCgbBgPaletteRam, m_ppuCgbObjPaletteRam);

    // Initialize framebuffer and video subsystem.
    m_video->ResizeOutput(ScreenWidth, ScreenHeight, 1.0, 1.0);
    m_videoFormat = PlipVideo::GetFormatInfo(video->GetFormat());
    m_videoBufferSize = m_videoFormat.pixelWidth * ScreenWidth * ScreenHeight;
    m_videoBuffer = new uint8_t[m_videoBufferSize];

    // Initialize input.
    RegisterInput();
}

GameBoyInstance::~GameBoyInstance() {
    delete m_cpu;
    delete m_videoBuffer;
}

void GameBoyInstance::BootRomFlagHandler() {
    if(!m_ioRegisters->GetBootRomDisabled()) return;
    
    // Swap the boot ROM out for the cartridge ROM.
    m_bootRomDisableFlag = true;
    m_gbMemory->DisableBootRom();

    // If we're emulating a CGB, check KEY0 to see if we should set the system to full CGB mode.
    m_cgbMode = m_model == GameBoyModel::CGB && !BIT_TEST(m_ioRegisters->GetByte(IoRegister::CpuModeSelect), 2);
}

void GameBoyInstance::Delta(const long ns) {
    auto timeRemaining = ns;
    auto cycleTime = m_cpu->GetCycleTime();

    ReadJoypad();
    ClearActiveBreakpoint();

    do {
        // Run CPU for one cycle.
        if(!m_dmaBlockCpu)
            m_cpu->Cycle();

        if(const auto cpuDoubleSpeed = m_cpu->IsDoubleSpeed(); m_doubleSpeed != cpuDoubleSpeed) {
            m_doubleSpeed = cpuDoubleSpeed;
            m_ioRegisters->SetDoubleSpeed(m_doubleSpeed);
            cycleTime = m_cpu->GetCycleTime();
            m_ioRegisters->Timer_Reset();
        }
        
        // Timer
        if(!m_dmaBlockCpu && !m_cpu->IsChangingSpeed())
            m_ioRegisters->Timer_Cycle();

        // DMA
        if(m_dmaState == DmaState::Inactive) {
            DmaCheck();
        } else {
            if(m_dmaTransferMode == DmaTransferMode::Oam) DmaCheck();  // OAM DMA transfers can be restarted.
            DmaCycle();
        }

        // Input
        m_ioRegisters->Joypad_SetMatrix(m_keypad);
        m_ioRegisters->Joypad_Cycle();

        // PPU
        PPU_Cycle();

        // RTC
        if(m_hasRtc) {
            m_gbMemory->RTC_Clock();
        }

        // CGB-specific Operations
        if(m_model == GameBoyModel::CGB) {
            // VRAM/WRAM Bank Switches
            if(const auto wramBank = m_ioRegisters->GetPerformWorkRamBankSwitch(); wramBank >= 0) {
                m_gbMemory->SetWorkRamBank(wramBank);
                m_ioRegisters->AcknowledgeWorkRamBankSwitch();
            }

            if(const auto vramBank = m_ioRegisters->Video_GetPerformVramBankSwitch(); vramBank >= 0) {
                m_gbMemory->SetVideoRamBank(vramBank);
                m_ioRegisters->Video_AcknowledgeVideoRamBankSwitch();
            }
        }

        // Handle ROM disable flag.
        if(!m_bootRomDisableFlag) {
            // See if we need to disable the boot ROM.
            BootRomFlagHandler();
        }

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

void GameBoyInstance::DmaCheck() {
    // OAM DMA
    if(const auto oamDmaSourceAddress = m_ioRegisters->Video_GetOamDmaCopyAddress(); oamDmaSourceAddress >= 0) {
        DmaInitOam(oamDmaSourceAddress);
        m_ioRegisters->Video_AcknowledgeOamDmaCopy();
    }

    if(m_model == GameBoyModel::CGB) {
        // HDMA/GDMA
        if(const auto transferMode = m_ioRegisters->Video_GetHdmaTransferMode(); transferMode != DmaTransferMode::Inactive) {
            DmaInitCgb(transferMode);
        }
    }
}

void GameBoyInstance::DmaComplete() const {
    switch(m_dmaTransferMode) {
        case DmaTransferMode::Oam: {
            DmaCompleteOam();
            break;
        }
        
        default: break;
    }
}

void GameBoyInstance::DmaCompleteOam() const {
    // This is the cycle that the copy would normally complete. Flag the inaccessible
    // memory as accessible again.
    m_cartRom->SetReadable(true);

    m_workRam->SetReadable(true);
    m_workRam->SetWritable(true);

    if(m_cartRamBanks > 0) {
        m_gbMemory->RestoreCartridgeMemoryAccessibility();
    }

    PPU_SetMemoryPermissions();  // Sets the writable state of VRAM/OAM.
}

void GameBoyInstance::DmaCycle() {
    if(m_cpu->IsHalted()) return;

    switch(m_dmaState) {
        case DmaState::Preparing: {
            if(--m_dmaPreparationCycles == 0) {
                m_dmaState = DmaState::Transferring;
                DmaFinishPreparations();
            }
            break;
        }

        case DmaState::WaitingForHBlank: {
            if(m_ppuMode != PPU_Mode::HBlank || m_batchLastPpuMode == PPU_Mode::HBlank) {
                m_batchLastPpuMode = m_ppuMode;
                break;
            }
            
            m_batchLastPpuMode = m_ppuMode;
            m_dmaState = DmaState::Preparing;
            m_dmaBatchLength = HBlankDmaBatchLength;
            m_dmaBlockCpu = m_dmaCgb;
            break;
        }
        
        case DmaState::Transferring: {
            if(m_dmaTransferMode != DmaTransferMode::Oam && m_ioRegisters->Video_GetHdmaTransferCancelled()) {
                m_ioRegisters->Video_AcknowledgeHdmaCancellation();
                m_dmaState = DmaState::Inactive;
                break;
            }

            const auto thisByte = m_dmaCopyInvalidBytes
                ? 0xFF
                : m_memory->GetByte(m_dmaSourceAddress + m_dmaCurrentOffset, true);
            
            m_memory->SetByte(m_dmaDestinationAddress + m_dmaCurrentOffset, thisByte, true);

            if(m_dmaCgb) {
                m_ioRegisters->Video_SetHdmaTransferRemaining(m_dmaCopyLength - m_dmaCurrentOffset);
            }

            if(++m_dmaCurrentOffset >= m_dmaCopyLength) {
                m_dmaState = DmaState::Finalize;
                m_dmaPreparationCycles = 1;
                m_dmaBlockCpu = false;

                if(m_dmaCgb) {
                    m_ioRegisters->Video_SetHdmaTransferComplete();
                }
            }

            if(m_dmaBatched && --m_dmaBatchLength == 0) {
                m_dmaState = DmaState::WaitingForHBlank;
            }
            
            break;
        }

        case DmaState::Finalize: {
            if(--m_dmaPreparationCycles == 0) {
                m_dmaState = DmaState::Inactive;
                DmaComplete();
            }
            break;
        }
        
        case DmaState::Inactive:
        default:
            break;
    }
}

void GameBoyInstance::DmaFinishPreparations() const {
    if(m_dmaTransferMode == DmaTransferMode::Oam) {
        // Flag ROM, WRAM, cart RAM, and OAM as inaccessible until the process is complete.
        if(m_cartRamBanks > 0) {
            m_cartRam->SetReadable(false);
            m_cartRam->SetWritable(false);
        }

        m_videoRam->SetReadable(false);
        m_videoRam->SetWritable(false);

        m_oam->SetReadable(false);
        m_oam->SetWritable(false);

        if(m_model == GameBoyModel::CGB) {
            // On CGB, cart ROM and WRAM are on separate buses, so flag them selectively.
            if(m_dmaSourceAddress < 0x8000) {
                m_cartRom->SetReadable(false);
            } else if(m_dmaSourceAddress >= 0xC000 && m_dmaSourceAddress < 0xFE00) {
                m_workRam->SetReadable(false);
                m_workRam->SetWritable(false);
            }
        } else {
            m_cartRom->SetReadable(false);

            m_workRam->SetReadable(false);
            m_workRam->SetWritable(false);
        }
    }
}

void GameBoyInstance::DmaInitCgb(const DmaTransferMode transferMode) {
    // Common initialization.
    m_dmaState = DmaState::Preparing;
    m_dmaPreparationCycles = 1;
    m_dmaBlockCpu = m_dmaCgb = true;
    m_dmaSourceAddress = m_ioRegisters->Video_GetHdmaSourceAddress();
    m_dmaDestinationAddress = m_gbMemory->VideoRamAddress | m_ioRegisters->Video_GetHdmaDestinationAddress();
    m_dmaCopyLength = m_ioRegisters->Video_GetHdmaTransferLength();
    m_dmaCurrentOffset = 0;

    // GDMA/HDMA can only copy from certain locations.
    m_dmaCopyInvalidBytes = (m_dmaSourceAddress >= 0x8000 && m_dmaSourceAddress < 0xA000) || m_dmaSourceAddress > 0xE000;

    m_dmaTransferMode = transferMode;
    switch(m_dmaTransferMode) {
        case DmaTransferMode::GeneralPurpose: {
            break;
        }
        
        case DmaTransferMode::HBlank: {
            m_dmaBatched = true;
            m_dmaBatchLength = HBlankDmaBatchLength;
            m_dmaState = DmaState::WaitingForHBlank;
            break;
        }
        
        case DmaTransferMode::Inactive:
        default:
            break;
    }
}

void GameBoyInstance::DmaInitOam(const int sourceAddress) {
    m_dmaState = DmaState::Preparing;
    m_dmaPreparationCycles = 1;
    m_dmaBlockCpu = m_dmaCgb = false;
    m_dmaSourceAddress = sourceAddress << 8;
    m_dmaDestinationAddress = GameBoyMapper::OamAddress;
    m_dmaCopyLength = OamDmaLength;
    m_dmaTransferMode = DmaTransferMode::Oam;
    m_dmaCurrentOffset = 0;
    m_dmaBatched = false;
    m_dmaCopyInvalidBytes = false;
}

std::map<std::string, std::map<std::string, Plip::DebugValue>> GameBoyInstance::GetDebugInfo() const {
    return {
        { "CPU Registers", m_cpu->GetRegisters() },
        { "CPU (Other)", m_cpu->GetDebugInfo() },
        { "MBC", m_gbMemory->GetMbcDebugInfo() },
        { "PPU", PPU_GetDebugInfo() },
        { "Timer", {
            { "DIV", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::Divider))) },
            { "TIMA", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::TimerCounter))) },
            { "TMA", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::TimerModulo))) },
            { "TAC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::TimerControl))) },
            { "TIMA Reload", DebugValue(m_timaQueueReload) },
        }},
        { "System", {
            { "Keypad", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_keypad)) },
            { "Boot ROM Enabled", DebugValue(!m_bootRomDisableFlag) },
            { "DMA State", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_dmaState)) },
            { "IE", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_highRam->GetByte(0x80))) },
            { "IF", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::InterruptFlag))) },
            { "CGB Mode", DebugValue(m_cgbMode) },
        }},
    };
}

std::vector<uint64_t> GameBoyInstance::GetPcs() const {
    return { m_cpu->GetPc() };
}

Plip::PlipError GameBoyInstance::Load(const std::string &path) {
    m_cartPath = path;
    if(!PlipIo::FileExists(m_cartPath)) {
        return PlipError::FileNotFound;
    }

    // Load the ROM.
    const auto size = PlipIo::GetSize(m_cartPath);
    const auto data = PlipIo::ReadFile(m_cartPath, size);
    m_cartRom = new PlipMemoryRom(data.data(), size, 0xFF);

    // Read the ROM header.
    ReadCartridgeFeatures();

    // Update titlebar.
    m_video->SetTitle("GameBoy: " + PlipIo::GetFilename(m_cartPath));

    // Set up the memory map.
    m_gbMemory = new GameBoyMapper(m_bootRom, m_cartRom, m_videoRam, m_workRam, m_oam, m_ioRegisters, m_highRam);
    delete m_memory;
    m_memory = m_gbMemory;

    m_cartRam = m_gbMemory->ConfigureMapper(m_mbc, m_hasRtc, m_cartRamBanks);

    // Create CPU.
    m_cpu = new Cpu::SharpLr35902(BaseClockRate / 4, m_memory, m_model == GameBoyModel::CGB);
    
    // Reset system.
    Reset();

    // Load the battery file if necessary.
    if(m_hasBattery) {
        m_batteryPath = m_cartPath;
        m_batteryPath.replace_extension(".sav");

        if(PlipIo::FileExists(m_batteryPath)) {
            if(const auto sramSize = PlipIo::GetSize(m_batteryPath); sramSize == m_cartRam->GetLength()) {
                // Only load SRAM if the file is the correct size.
                const auto sramData = PlipIo::ReadFile(m_batteryPath, sramSize);

                for(auto i = 0; i < sramSize; ++i) {
                    m_cartRam->SetByte(i, sramData[i], true);
                }
            }
        }
    }

    // Load the RTC clock data.
    if(m_hasRtc) {
        m_rtcDataPath = m_cartPath;
        m_rtcDataPath.replace_extension(".rtc");

        if(PlipIo::FileExists(m_rtcDataPath)) {
            auto rtcDataFile = PlipIo::LoadFile(m_rtcDataPath);
            m_gbMemory->RTC_Load(rtcDataFile);
            rtcDataFile.close();
        }
    }

    return PlipError::Success;
}

int GameBoyInstance::GetCartridgeRamBankCount() const {
    if(!m_hasCartRam) return 0;

    switch(const auto ramSize = m_cartRom->GetByte(CartRamSizeOffset)) {
        case 0x00:  // 0KB
        case 0x01:  // 2KB
        case 0x02:  // 8KB
            return 1;

        case 0x03:  // 32KB
            return 4;

        case 0x04:  // 128KB
            return 16;

        case 0x05:  // 64KB
            return 8;

        default: {
            std::stringstream ex;
            ex << "invalid cart RAM size byte: "
               << PlipUtility::FormatHex(ramSize, 2);
            throw PlipEmulationException(ex.str().c_str());
        }
    }
}

void GameBoyInstance::ReadCartridgeFeatures() {
    const auto cartType = m_cartRom->GetByte(0x0147);

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
        case 0x0F: case 0x10: case 0x11: case 0x12: case 0x13:
            m_mbc = MBC_Type::Mbc3;
            break;
        case 0x0B: case 0x0C: case 0x0D:
            m_mbc = MBC_Type::Mmm01;
            break;
        case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E:
            m_mbc = MBC_Type::Mbc5;
            break;
        case 0x20:
            m_mbc = MBC_Type::Mbc6;
            break;
        case 0x22:
            m_mbc = MBC_Type::Mbc7;
            break;
        case 0xFC:
            m_mbc = MBC_Type::PocketCamera;
            break;
        case 0xFD:
            m_mbc = MBC_Type::BandaiTama5;
            break;
        case 0xFE:
            m_mbc = MBC_Type::HuC3;
            break;
        case 0xFF:
            m_mbc = MBC_Type::HuC1;
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

    m_cartRamBanks = GetCartridgeRamBankCount();

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
        m_workRam->SetByte(i, 0xFF, true);

    for(auto i = 0; i < m_videoRam->GetLength(); i++)
        m_videoRam->SetByte(i, 0xFF, true);

    for(auto i = 0; i < m_oam->GetLength(); i++)
        m_oam->SetByte(i, 0xFF, true);

    for(auto i = 0; i < m_highRam->GetLength(); i++)
        m_highRam->SetByte(i, 0xFF, true);

    if(m_cartRamBanks > 0)
        for(auto i = 0; i < m_cartRam->GetLength(); i++)
            m_cartRam->SetByte(i, 0xFF, true);

    // Initialize the mapper.
    m_gbMemory->Reset();

    // Reset CPU.
    m_cpu->Reset(0x0000);

    // Reset PPU.
    PPU_Reset();

    // Initialize RTC counter (if applicable).
    if(m_hasRtc) {
        m_gbMemory->RTC_SetCpuClockRate(m_cpu->GetHz());
        m_gbMemory->RTC_ResetSubSecondClock();
    }

    // Reset I/O registers.
    m_ioRegisters->Reset();
    RegisterWriteServiced();
}

void GameBoyInstance::Shutdown() {
    // Save SRAM (if applicable).
    if(m_hasBattery) {
        PlipIo::DumpMemoryToDisk(m_batteryPath, m_cartRam);
    }

    // Save RTC data (if applicable).
    if(m_hasRtc) {
        auto rtcDataFile = PlipIo::CreateFile(m_rtcDataPath);
        m_gbMemory->RTC_Dump(rtcDataFile);
        rtcDataFile.close();
    }
}
