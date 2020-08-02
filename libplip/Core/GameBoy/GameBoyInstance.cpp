/* GameBoyInstance.cpp
 *
 * A GameBoy core for Plip.
 */

#include <sstream>

#include "../../PlipEmulationException.h"
#include "../../PlipIo.h"
#include "../../PlipUtility.h"

#include "GameBoyInstance.h"

namespace Plip::Core::GameBoy {
    GameBoyInstance::GameBoyInstance(PlipAudio *audio, PlipInput *input, PlipVideo *video)
    : PlipCore(audio, input, video) {
        m_videoRam = new Plip::PlipMemoryRam(0x2000);
        m_workRam = new Plip::PlipMemoryRam(0x2000);
        m_oam = new Plip::PlipMemoryRam(0xA0);
        m_unusable = new Plip::PlipMemoryRom(&m_unusableContents, 0x60);
        m_ioRegisters = new Plip::PlipMemoryRam(0x80);
        m_highRam = new Plip::PlipMemoryRam(0x80);

        m_memory->AssignBlock(m_videoRam, 0x8000);
        m_memory->AssignBlock(m_workRam, 0xC000);
        m_memory->AssignBlock(m_workRam, 0xE000);  // ECHO RAM
        m_memory->AssignBlock(m_oam, 0xFE00);
        m_memory->AssignBlock(m_unusable, 0xFEA0);
        m_memory->AssignBlock(m_ioRegisters, 0xFF00);
        m_memory->AssignBlock(m_highRam, 0xFF80);

        m_cpu = new Cpu::SharpLr35902(ClockRate, m_memory);
    }

    void GameBoyInstance::Delta(long ns) {}

    uint16_t GameBoyInstance::GetRomBankCount() {
        auto romSizeByte = m_rom->GetByte(0x0148);

        switch(romSizeByte) {
            case 0x00: return 0;   // 32KB
            case 0x01: return 4;   // 64KB
            case 0x02: return 8;   // 128KB
            case 0x03: return 16;  // 256KB
            case 0x04: return 32;  // 512 KB
            case 0x05: return m_mbc == Mbc1 ? 63 : 64;   // 1024KB
            case 0x06: return m_mbc == Mbc1 ? 125 : 128; // 2048KB
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
        m_memory->AssignBlock(m_rom, 0x0000, 0x0000, 0x8000);

        InitMbc();
        if(m_mbc != None) m_romBanks = GetRomBankCount();
        if(m_hasRam) InitCartRam();
        // Assign: 0xA000-0xBFFF (cart RAM)

        m_running = true;
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
            m_memory->AssignBlock(m_cartRam, 0xA000);
        } else {
            m_memory->AssignBlock(m_cartRam, 0xA000, 0x0000, 0x2000);
        }
    }

    void GameBoyInstance::InitMbc() {
        auto cartType = m_rom->GetByte(0x0147);

        // MBC
        switch(cartType) {
            case 0x00: case 0x08: case 0x09:
                m_mbc = None;
                break;

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
}
