/* NesInstance.cpp
 *
 * An NES emulation core.
 */

#include "NesInstance.h"

#include "../../PlipIo.h"

using Plip::Core::Nes::NesInstance;

#define READ_INPUT1(idx) { \
    if(m_input->GetInput(idx).digital) \
    m_input1 |= 1 << idx; \
}

#define READ_INPUT2(idx) { \
    if(m_input->GetInput(idx - 8).digital) \
    m_input2 |= 1 << (idx - 8); \
}

NesInstance::NesInstance(PlipAudio* audio, PlipInput* input, PlipVideo* video, const PlipKeyValuePairCollection& config)
: PlipCore(audio, input, video, config) {
    // Initialize framebuffer and video subsystem.
    m_video->ResizeOutput(ScreenWidth, ScreenHeight, 1.0, 1.0);
    m_videoFormat = PlipVideo::GetFormatInfo(video->GetFormat());
    m_videoBufferSize = m_videoFormat.pixelWidth * ScreenWidth * ScreenHeight;
    m_videoBuffer = new uint8_t[m_videoBufferSize];
    
    // Initialize input.
    RegisterInput();
}

NesInstance::~NesInstance() {
    delete m_cpu;
    delete m_videoBuffer;
}

void NesInstance::Delta(const long ns) {
    const auto cycleTime = m_cpu->GetCycleTime();
    auto timeRemaining = ns;

    do {
        // Run CPU for one cycle.
        m_cpu->Cycle();

        // Input
        ReadControllers();

        // PPU
        // Ticks thrice for every CPU cycle on NTSC (master clock divided by 4) and Dendy.
        // TODO: Emulate PAL behavior (master clock divided by 5; 3.2 PPU ticks per CPU cycle).
        for(auto i = 0; i < 3; ++i) {
            PPU_Cycle();
        }

        timeRemaining -= cycleTime;
    } while(cycleTime < timeRemaining);
}

std::map<std::string, std::map<std::string, Plip::DebugValue>> NesInstance::GetDebugInfo() const {
    return {
        { "CPU Registers", m_cpu->GetRegisters() },
        { "PPU", PPU_GetDebugInfo() },
        { "PPU Registers", m_ppuRegisters->GetDebugInfo() },
    };
}

std::vector<uint64_t> NesInstance::GetPcs() const {
    return { m_cpu->GetPc() };
}

Plip::PlipError NesInstance::Load(const std::string& path) {
    m_cartPath = path;
    if(!PlipIo::FileExists(m_cartPath)) {
        return PlipError::FileNotFound;
    }

    const auto romSize = PlipIo::GetSize(m_cartPath);
    auto romFile = PlipIo::LoadFile(m_cartPath);

    // Read the ROM header.
    if(romSize < 16) {
        return PlipError::RomFileTruncated;
    }
    
    if(const auto romHeader = PlipIo::ReadFile(romFile, 16); !ReadRomHeader(romHeader)) {
        return PlipError::UnrecognizedMedia;
    }

    // Make sure the ROM file is actually large enough, then update the titlebar.
    const auto trainerSize = m_cartHasTrainer ? m_trainerSize : 0;
    if(romSize < (m_cartPrgRomSize + m_cartChrRomSize + 16 + trainerSize)) {
        return PlipError::RomFileTruncated;
    }
    m_video->SetTitle("NES: " + PlipIo::GetFilename(m_cartPath));

    // Load the ROMs.
    // Trainer > PRG ROM > CHR ROM
    if(m_cartHasTrainer) {
        const auto trainerRom = PlipIo::ReadSequential(romFile, m_trainerSize);
        m_trainerRom = new PlipMemoryRom(trainerRom.data(), m_trainerSize, 0xFF);
    }

    const auto prgRom = PlipIo::ReadSequential(romFile, m_cartPrgRomSize);
    const auto chrRom = PlipIo::ReadSequential(romFile, m_cartChrRomSize);
    PlipIo::CloseFile(romFile);

    m_prgRom = new PlipMemoryRom(prgRom.data(), m_cartPrgRomSize, 0xFF);
    m_chrRom = new PlipMemoryRom(chrRom.data(), m_cartChrRomSize, 0xFF);

    // Initialize the system and PPU memory.
    m_workRam = new PlipMemoryRam(m_workRamAmount);
    m_ppuRam = new PlipMemoryRam(m_ppuRamAmount);
    m_ppuRegisters = new NesPpuRegisters(m_ppuRam);
    m_apuRegisters = new NesApuRegisters();

    // Set up the memory map.
    m_mapper = NesMapper::CreateMapper(
        m_cartUsesINesHeader,
        m_cartMapper, m_cartSubmapper,
        m_prgRom, m_chrRom, m_trainerRom,
        m_cartPrgRamSize, m_cartChrRamSize,
        m_cartPrgNvramSize, m_cartChrNvramSize
    );

    m_nesMemory = new NesMemory(m_workRam, m_ppuRegisters, m_apuRegisters, m_mapper, m_ppuRam);
    delete m_memory;
    m_memory = m_nesMemory;

    // Initialize the CPU.
    int cpuClock;
    switch(m_cartTimingMode) {
        case NesTiming::Ntsc:
        case NesTiming::MultiRegion:
        default:
            cpuClock = CpuClockNtsc;
            break;
        case NesTiming::Pal:
            cpuClock = CpuClockPal;
            break;
        case NesTiming::Dendy:
            cpuClock = CpuClockDendy;
            break;
    }
    m_cpu = new Cpu::Mos6502(cpuClock, m_memory, Cpu::Mos6502Version::Ricoh2A03);

    // Pass the CPU to the PPU registers object to allow it to raise NMIs.
    m_ppuRegisters->SetCpu(m_cpu);
    
    return PlipError::Success;
}

void NesInstance::ReadControllers() {
    if(!m_apuRegisters->GetControllerStrobe()) return;
    
    m_input1 = m_input2 = 0;

    READ_INPUT1(InputP1_A);
    READ_INPUT1(InputP1_B);
    READ_INPUT1(InputP1_Select);
    READ_INPUT1(InputP1_Start);
    READ_INPUT1(InputP1_Up);
    READ_INPUT1(InputP1_Down);
    READ_INPUT1(InputP1_Left);
    READ_INPUT1(InputP1_Right);
    m_apuRegisters->SetControllerPort1(m_input1);

    READ_INPUT2(InputP2_A);
    READ_INPUT2(InputP2_B);
    READ_INPUT2(InputP2_Select);
    READ_INPUT2(InputP2_Start);
    READ_INPUT2(InputP2_Up);
    READ_INPUT2(InputP2_Down);
    READ_INPUT2(InputP2_Left);
    READ_INPUT2(InputP2_Right);
    m_apuRegisters->SetControllerPort2(m_input2);
}

bool NesInstance::ReadRomHeader(const std::vector<char>& headerData) {
    // iNES/NES 2.0 headers only, plz :)
    if(!(headerData[0] == 'N' && headerData[1] == 'E' && headerData[2] == 'S' && headerData[3] == 0x1A)) {
        return false;
    }

    // Read data common to both header types.
    m_cartNametableHorizontal = headerData[6] & 0x01;
    m_cartHasBattery = headerData[6] & 0x02;
    m_cartHasTrainer = headerData[6] & 0x04;
    m_cartUsesAltNamespace = headerData[6] & 0x08;

    // Determine the header format and use the appropriate read method.
    (headerData[6] & 0b00001100) == 0b00001000
        ? ReadRomHeaderNes2(headerData)
        : ReadRomHeaderINes(headerData);

    return true;
}

void NesInstance::ReadRomHeaderINes(const std::vector<char>& headerData) {
    // Reference: https://www.nesdev.org/wiki/INES
    m_cartUsesINesHeader = true;
    
    m_cartPrgRomSize = (static_cast<uint8_t>(headerData[4])) * 16384;
    m_cartChrRomSize = static_cast<uint8_t>(headerData[5]) * 8192;

    // Some rippers/tools used the formerly "unused" header bytes.
    // If there's anything in bytes 11-14, we need to be a bit more careful.
    const auto trustHigherBytes = !(headerData[11] == 0 && headerData[12] == 0 && headerData[13] == 0 && headerData[14]);
    
    m_cartMapper = (headerData[7] & 0xF0) | ((headerData[6] & 0xF0) >> 4);
    if(!trustHigherBytes) {
        m_cartMapper &= 0x0F;  // Well, so much for that...
    }
}

void NesInstance::ReadRomHeaderNes2(const std::vector<char>& headerData) {
    // Reference: https://www.nesdev.org/wiki/NES_2.0
    m_cartUsesINesHeader = false;

    // If the PRG/CHR-ROM upper nibble (in byte 9) is 0-E, use that to calculate the header size.
    // If it's F, use the following format (assuming header data 4/5 is EEEEEEMM): 2^E * ((M * 2) + 1)
    if(const int cartPrgRomUpper = (headerData[9] & 0x0F) << 8; cartPrgRomUpper == 0xF00) {
        const auto exponent = (headerData[4] & 0b11111100) >> 2;
        const auto multiplier = headerData[4] & 0b11;
        m_cartPrgRomSize = (1 << exponent) * ((multiplier * 2) + 1);
    } else {
        m_cartPrgRomSize = (cartPrgRomUpper | headerData[4]) * 16384;
    }

    if(const int cartChrRomUpper = (headerData[9] & 0xF0) << 4; cartChrRomUpper == 0xF00) {
        const auto exponent = (headerData[4] & 0b11111100) >> 2;
        const auto multiplier = headerData[4] & 0b11;
        m_cartChrRomSize = (1 << exponent) * ((multiplier * 2) + 1);
    } else {
        m_cartChrRomSize = (cartChrRomUpper | headerData[5]) * 8192;
    }

    // Mappers, flags, etc.
    m_cartMapper = ((headerData[8] & 0x0F) << 4) | (headerData[7] & 0xF0) | ((headerData[6] & 0xF0) >> 4);
    m_cartSubmapper = ((headerData[8] & 0xF0) >> 4);

    if(const auto prgRamShiftCount = (headerData[10] & 0xF); prgRamShiftCount > 0) {
        m_cartPrgRamSize = 64 << prgRamShiftCount;
    }

    if(const auto prgNvramShiftCount = ((headerData[10] & 0xF0) >> 4); prgNvramShiftCount > 0) {
        m_cartPrgNvramSize = 64 << prgNvramShiftCount;
    }

    if(const auto chrRamShiftCount = (headerData[11] & 0xF); chrRamShiftCount > 0) {
        m_cartPrgRamSize = 64 << chrRamShiftCount;
    }

    if(const auto chrNvramShiftCount = ((headerData[11] & 0xF0) >> 4); chrNvramShiftCount > 0) {
        m_cartPrgNvramSize = 64 << chrNvramShiftCount;
    }

    m_cartTimingMode = static_cast<NesTiming>(headerData[12] & 0b11);
}

void NesInstance::RegisterInput() const {
    m_input->AddInput(InputP1_A, PlipInputDefinition(PlipInputType::Digital, "P1-A"), { .digital = false });
    m_input->AddInput(InputP1_B, PlipInputDefinition(PlipInputType::Digital, "P1-B"), { .digital = false });
    m_input->AddInput(InputP1_Select, PlipInputDefinition(PlipInputType::Digital, "P1-Select"), { .digital = false });
    m_input->AddInput(InputP1_Start, PlipInputDefinition(PlipInputType::Digital, "P1-Start"), { .digital = false });
    m_input->AddInput(InputP1_Up, PlipInputDefinition(PlipInputType::Digital, "P1-Up"), { .digital = false });
    m_input->AddInput(InputP1_Down, PlipInputDefinition(PlipInputType::Digital, "P1-Down"), { .digital = false });
    m_input->AddInput(InputP1_Left, PlipInputDefinition(PlipInputType::Digital, "P1-Left"), { .digital = false });
    m_input->AddInput(InputP1_Right, PlipInputDefinition(PlipInputType::Digital, "P1-Right"), { .digital = false });

    m_input->AddInput(InputP2_A, PlipInputDefinition(PlipInputType::Digital, "P2-A"), { .digital = false });
    m_input->AddInput(InputP2_B, PlipInputDefinition(PlipInputType::Digital, "P2-B"), { .digital = false });
    m_input->AddInput(InputP2_Select, PlipInputDefinition(PlipInputType::Digital, "P2-Select"), { .digital = false });
    m_input->AddInput(InputP2_Start, PlipInputDefinition(PlipInputType::Digital, "P2-Start"), { .digital = false });
    m_input->AddInput(InputP2_Up, PlipInputDefinition(PlipInputType::Digital, "P2-Up"), { .digital = false });
    m_input->AddInput(InputP2_Down, PlipInputDefinition(PlipInputType::Digital, "P2-Down"), { .digital = false });
    m_input->AddInput(InputP2_Left, PlipInputDefinition(PlipInputType::Digital, "P2-Left"), { .digital = false });
    m_input->AddInput(InputP2_Right, PlipInputDefinition(PlipInputType::Digital, "P2-Right"), { .digital = false });
}

void NesInstance::Reset() {
    m_ppuRegisters->Reset();
}
