/* GameBoyInstance.cpp
 *
 * A GameBoy core for Plip.
 */

#include <cstring>
#include <sstream>

#include "../../PlipEmulationException.h"
#include "../../PlipInitializationException.h"
#include "../../PlipIo.h"
#include "../../PlipUtility.h"

#include "GameBoyInstance.h"

#define READ_INPUT(idx) do { if(m_input->GetInput(idx).digital) m_keypad |= 1 << idx; } while(0)

namespace Plip::Core::GameBoy {
    GameBoyInstance::GameBoyInstance(PlipAudio *audio, PlipInput *input, PlipVideo *video, PlipConfig *config)
    : PlipCore(audio, input, video, config) {
        using io = Plip::PlipIo;

        // Initialize input.
        RegisterInput();
        m_memory->SetByte(m_regJoypad, 0b11111111);

        // Load the boot ROM.
        m_bootRomPath = m_config->GetOption("bootRom");
        if(m_bootRomPath == m_config->empty)
            throw PlipInitializationException("Boot ROM must be specified in the core config.");
        if(!io::FileExists(m_bootRomPath))
            throw PlipInitializationException("Specified Boot ROM could not be found.");

        auto size = io::GetSize(m_bootRomPath);
        auto data = io::ReadFile(m_bootRomPath, size);
        m_bootRom = new Plip::PlipMemoryRom(data.data(), size);

        // Initialize framebuffers and video subsystem.
        m_video->Resize(m_screenWidth, m_screenHeight);
        m_videoFmt = Plip::PlipVideo::GetFormatInfo(video->GetFormat());
        m_videoBufferSize = m_videoFmt.pixelWidth * m_screenWidth * m_screenWidth;
        m_videoBuffer = new uint8_t[m_videoBufferSize];
        m_videoMode = m_videoModeOamSearch;

        m_spriteList = new uint8_t[m_maxSpritesPerScanline] {};
        m_spriteListSorted = new uint8_t[m_maxSpritesPerScanline] {};

        // Paint the frame buffer white.
        memset(m_videoBuffer, 0xFF, m_videoBufferSize);

        // Initialize system RAM.
        m_videoRam = new Plip::PlipMemoryRam(0x2000);
        m_workRam = new Plip::PlipMemoryRam(0x2000);
        m_oam = new Plip::PlipMemoryRam(0xA0);
        m_unusable = new Plip::PlipMemoryRom(&m_unusableContents, 0x60);
        m_ioRegisters = new Plip::PlipMemoryRam(0x80);
        m_highRam = new Plip::PlipMemoryRam(0x80);

        m_memory->AssignBlock(m_videoRam, m_addrVideoRam);
        m_memory->AssignBlock(m_workRam, m_addrWorkRam);
        m_memory->AssignBlock(m_workRam, m_addrEchoRam);  // ECHO RAM
        m_memory->AssignBlock(m_oam, m_addrOam);
        m_memory->AssignBlock(m_unusable, m_addrUnusable);
        m_memory->AssignBlock(m_ioRegisters, m_addrRegisters);
        m_memory->AssignBlock(m_highRam, m_addrHighRam);

        // Initialize CPU.
        m_cpu = new Cpu::SharpLr35902(BaseClockRate / 4, m_memory);
        m_cycleTime = m_cpu->GetCycleTime();
    }

    GameBoyInstance::~GameBoyInstance() {
        delete m_cpu;
        delete m_spriteList;
        delete m_videoBuffer;
    }

    void GameBoyInstance::ClearBreakpoint() {
        m_bp = 0xFFFFFFFF;
    }

    void GameBoyInstance::Delta(long ns) {
        PlipMemoryValue lastWrite {};
        m_cycleRemaining += ns;
        m_dotCyclesRemaining += m_dotsPerCycle;
        ReadInput();

        do {
            // Run a single CPU cycle.
            m_memory->ClearLastWrite();
            m_cpu->Cycle();
            lastWrite = m_memory->GetLastWrite();

            // Emulate MBC functionality.
            if(m_mbc != None) MbcCycle(lastWrite);

            // Divider
            auto divFallingEdge = false;
            if(lastWrite.address == m_addrRegisters + m_regDivider) {
                // Falling edge detector quirk.
                divFallingEdge = BIT_TEST(m_divider, 0);

                // Reset timer.
                m_divider = m_dividerTick = 0;
                m_ioRegisters->SetByte(m_regDivider, m_divider);
            } else {
                m_dividerTick += 4;
                if(m_dividerTick == 0) {
                    // Increment divider when the divider tick wraps around
                    // (4194304 / 16384 == 256).
                    m_divider++;
                    m_ioRegisters->SetByte(m_regDivider, m_divider);
                }
            }

            // Timer
            auto oldTac = m_tac;
            m_tac = m_ioRegisters->GetByte(m_regTac);
            if(BIT_TEST(m_tac, 2)) {
                m_timerIntBlocked = false;

                // Check for a scheduled interrupt.
                if(m_timerIntScheduled) {
                    auto tma = m_ioRegisters->GetByte(m_regTma);
                    m_timer = tma;
                    m_ioRegisters->SetByte(m_regTima, m_timer);
                    m_cpu->Interrupt(INTERRUPT_TIMER);
                    m_timerIntScheduled = false;
                }

                // If the falling edge detector triggered on DIV, increment TIMA.
                if(divFallingEdge) IncrementTimer();

                if(lastWrite.address == m_addrRegisters + m_regTima)
                    m_timerIntBlocked = true;

                if(lastWrite.address == m_addrRegisters + m_regTac) {
                    // If the falling edge detector triggered on TAC, increment TIMA.
                    if(((oldTac & 0b11) == 0b01) && ((m_tac & 0b11) == 0b00))
                        IncrementTimer();
                }

                // Increment internal timer and increment TIMA if necessary.
                ++m_timerTick;
                switch(m_tac & 0b11) {
                    case 0b00:  // 4096hz / 1024 clocks / 256 mcycles
                        if(m_timerTick == 0)
                            IncrementTimer();
                        break;
                    case 0b01:  // 262144hz / 16 clocks / 4 mcycles
                        if(m_timerTick % 4 == 0)
                            IncrementTimer();
                        break;
                    case 0b10:  // 65536hz / 64 clocks / 16 mcycles
                        if(m_timerTick % 16 == 0)
                            IncrementTimer();
                        break;
                    case 0b11:  // 16384hz / 256 clocks / 64 mcycles
                        if(m_timerTick % 64 == 0)
                            IncrementTimer();
                        break;
                }
            }

            // Check the input register.
            // TODO: Simulate DMG/SGB propagation delay.
            auto inputReg = m_ioRegisters->GetByte(m_regJoypad);
            if(!BIT_TEST(inputReg, 5)) {
                // Button keys selected.
                BIT_SET(inputReg, 4);
                inputReg &= ~(m_keypad >> 4);  // Pull the inputs low.
            } else if(!BIT_TEST(inputReg, 4)) {
                // Direction keys selected.
                BIT_SET(inputReg, 5);
                inputReg &= ~(m_keypad & 0b00001111);
            }
            m_ioRegisters->SetByte(m_regJoypad, inputReg);

            if(!m_bootRomFlag && m_cpu->GetRegisters().pc >= 0x100) {
                m_bootRomFlag = true;

                // Swap the boot ROM out for the cartridge ROM.
                m_memory->AssignBlock(m_rom, 0x0000, 0x0000, 0x0100);
            }

            auto lcdc = m_ioRegisters->GetByte(m_regLcdControl);

            // Run 4 dot clock cycles (4.19MHz) if the display is enabled.
            if(BIT_TEST(m_videoLastLcdc, 7) && !BIT_TEST(lcdc, 7)) {
                // The LCD display was disabled during this CPU cycle. Flag all
                // video memory as being writable.
                m_oam->SetWritable(true);
                m_videoRam->SetWritable(true);

                // When the LCD is disabled, the screen should go blank.
                memset(m_videoBuffer, 0xFF, m_videoBufferSize);
                m_video->BeginDraw();
                m_video->Draw(m_videoBuffer);
                m_video->EndDraw();
                m_video->Render();
            } else if(!BIT_TEST(m_videoLastLcdc, 7) && BIT_TEST(lcdc, 7)) {
                // The LCD display was enabled during this CPU cycle. Set the
                // video memory accessibility appropriately.
                VideoSetMemoryPermissions();
                m_lcdBlankFrame = true;
            }

            if(BIT_TEST(lcdc, 7)) {
                for(auto dotCycle = 0; dotCycle < m_dotsPerCycle; dotCycle++) {
                    VideoCycle();
                }
            }

            m_ioRegisters->SetByte(m_regLy, m_videoLy);
            m_videoLastLcdc = lcdc;

            m_cycleRemaining -= m_cycleTime;

            if(m_cpu->GetPc() == m_bp) {
                m_cycleRemaining = 0;
                m_paused = true;
                break;
            }
        } while(m_cycleTime < m_cycleRemaining);
    }

    std::string GameBoyInstance::DumpRegisters() {
        return m_cpu->DumpRegisters();
    }

    uint16_t GameBoyInstance::GetRomBankCount() {
        auto romSizeByte = m_rom->GetByte(0x0148);

        switch(romSizeByte) {
            case 0x00: return 0;   // 32KB
            case 0x01: return 4;   // 64KB
            case 0x02: return 8;   // 128KB
            case 0x03: return 16;  // 256KB
            case 0x04: return 32;  // 512 KB
            case 0x05: return 64;  // 1024KB
            case 0x06: return 128; // 2048KB
            case 0x07: return 256; // 4096KB
            case 0x08: return 512; // 8192KB
            case 0x52: return 72;  // 1152KB
            case 0x53: return 80;  // 1280KB
            case 0x54: return 96;  // 1536KB
            default:
                std::stringstream ex;
                ex << "invalid ROM size byte: "
                   << PlipUtility::FormatHex(romSizeByte, 2);
                throw Plip::PlipEmulationException(ex.str().c_str());
        }
    }

    PlipError GameBoyInstance::Load(const std::string &path) {
        using io = Plip::PlipIo;
        if(!io::FileExists(path)) return PlipError::FileNotFound;

        auto size = io::GetSize(path);
        auto data = io::ReadFile(path, size);
        m_rom = new Plip::PlipMemoryRom(data.data(), size);
        m_memory->AssignBlock(m_rom, m_addrRom, 0x0000, 0x8000);

        ReadCartFeatures();
        if(m_mbc != None) m_romBanks = GetRomBankCount();
        if(m_hasRam) InitCartRam();
        MbcInit();

        // Load the boot ROM into 0x0000-0x00FF.
        m_memory->AssignBlock(m_bootRom, 0x0000, 0x0000, 0x0100);
        m_bootRomFlag = false;

        return PlipError::Success;
    }

    void GameBoyInstance::InitCartRam() {
        auto ramSizeByte = m_rom->GetByte(0x0149);

        switch(ramSizeByte) {
            case 0x00:  // 0KB
            case 0x01:  // 2KB
            case 0x02:  // 8KB
                m_cartRamBanks = 0; break;
            case 0x03: m_cartRamBanks = 4;  break;  // 32KB
            case 0x04: m_cartRamBanks = 16; break;  // 128KB
            case 0x05: m_cartRamBanks = 8;  break;  // 64KB
            default:
                std::stringstream ex;
                ex << "invalid cart RAM size byte: "
                   << PlipUtility::FormatHex(ramSizeByte, 2);
                throw Plip::PlipEmulationException(ex.str().c_str());
        }

        m_cartRam = new Plip::PlipMemoryRam(8192 * m_cartRamBanks);

        if(ramSizeByte == 0x00) {
            m_memory->AssignBlock(m_cartRam, m_addrCartRam);
        } else {
            m_memory->AssignBlock(m_cartRam, m_addrCartRam, 0x0000, 0x2000);
        }
    }

    void GameBoyInstance::ReadCartFeatures() {
        auto cartType = m_rom->GetByte(0x0147);

        // MBC
        switch(cartType) {
            case 0x00: case 0x08: case 0x09:
                m_mbc = None; break;

            case 0x01: case 0x02: case 0x03:
                m_mbc = Mbc1; break;

            default:
                std::stringstream ex;
                ex << "invalid/unsupported memory bank controller: "
                   << PlipUtility::FormatHex(cartType, 2);
                throw Plip::PlipEmulationException(ex.str().c_str());
        }

        // RAM
        switch(cartType) {
            case 0x02: case 0x03: case 0x08: case 0x09: case 0x0C: case 0x0D:
            case 0x0F: case 0x10: case 0x12: case 0x13: case 0x1A: case 0x1B:
            case 0x1D: case 0x1E: case 0x22: case 0xFF:
                m_hasRam = true; break;
            default:
                m_hasRam = false; break;
        }

        // Battery
        switch(cartType) {
            case 0x03: case 0x06: case 0x09: case 0x0D: case 0x0F: case 0x10:
            case 0x13: case 0x1B: case 0x1E: case 0x22: case 0xFF:
                m_hasBattery = true; break;
            default:
                m_hasBattery = false; break;
        }

        // RTC
        switch(cartType) {
            case 0x0F: case 0x10:
                m_hasRtc = true; break;
            default:
                m_hasRtc = false; break;
        }

        // Camera
        m_hasCamera = cartType == 0xFC;

        // Sensor
        m_hasSensor = cartType == 0x22;
    }

    void GameBoyInstance::ReadInput() {
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

    void GameBoyInstance::Redraw() {
        m_video->BeginDraw();
        m_video->Draw(m_videoBuffer);
        m_video->EndDraw();
        m_video->Render();
    }

    void GameBoyInstance::RegisterInput() {
        m_input->AddInput(InputRight,
                          PlipInputDefinition(
                                  PlipInputType::Digital,
                                  "Right"),
                          { .digital = false });

        m_input->AddInput(InputLeft,
                          PlipInputDefinition(
                                  PlipInputType::Digital,
                                  "Left"),
                          { .digital = false });

        m_input->AddInput(InputUp,
                          PlipInputDefinition(
                                  PlipInputType::Digital,
                                  "Up"),
                          { .digital = false });

        m_input->AddInput(InputDown,
                          PlipInputDefinition(
                                  PlipInputType::Digital,
                                  "Down"),
                          { .digital = false });

        m_input->AddInput(InputA,
                          PlipInputDefinition(
                                  PlipInputType::Digital,
                                  "A"),
                          { .digital = false });

        m_input->AddInput(InputB,
                          PlipInputDefinition(
                                  PlipInputType::Digital,
                                  "B"),
                          { .digital = false });

        m_input->AddInput(InputSelect,
                          PlipInputDefinition(
                                  PlipInputType::Digital,
                                  "Select"),
                          { .digital = false });

        m_input->AddInput(InputStart,
                          PlipInputDefinition(
                                  PlipInputType::Digital,
                                  "Start"),
                          { .digital = false });
    }

    void GameBoyInstance::SetBreakpoint(uint32_t pc) {
        m_bp = pc;
    }
}
