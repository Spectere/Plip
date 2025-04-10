/* GameBoyMapper.cpp
 *
 * A custom PlipMemoryMap that emulates the Game Boy's MBC chips.
 */

#include "GameBoyMapper.h"

#include <iostream>

#include "Mbc2Ram.h"

using Plip::Core::GameBoy::GameBoyMapper;

GameBoyMapper::GameBoyMapper(PlipMemory* bootRom, PlipMemory* cartRom, PlipMemory* videoRam, PlipMemory* workRam, PlipMemory* oam, PlipMemory* ioRegisters, PlipMemory* highRam)
                            : m_bootRom(bootRom), m_cartRom(cartRom), m_videoRam(videoRam), m_workRam(workRam), m_oam(oam), m_ioRegisters(ioRegisters), m_highRam(highRam) {
    // Pull invalid and unreadable values high.
    SetInvalidByte(0xFF);
}

Plip::PlipMemory* GameBoyMapper::ConfigureMapper(const MBC_Type mbcType, const int cartRamBanks) {
    m_mbcType = mbcType;
    m_cartRamBanks = cartRamBanks;

    // Catch unimplemented mappers.
    // ReSharper disable once CppIncompleteSwitchStatement
    switch(m_mbcType) {
        case MBC_Type::Mbc5: throw PlipEmulationException("Unsupported mapper: MBC5");
        case MBC_Type::Mbc6: throw PlipEmulationException("Unsupported mapper: MBC6");
        case MBC_Type::Mbc7: throw PlipEmulationException("Unsupported mapper: MBC7");
        case MBC_Type::Mmm01: throw PlipEmulationException("Unsupported mapper: MMM01");
        case MBC_Type::PocketCamera: throw PlipEmulationException("Unsupported mapper: Pocket Camera");
        case MBC_Type::BandaiTama5: throw PlipEmulationException("Unsupported mapper: Bandai TAMA5");
        case MBC_Type::HuC1: throw PlipEmulationException("Unsupported mapper: HuC1");
        case MBC_Type::HuC3: throw PlipEmulationException("Unsupported mapper: HuC3");
    }

    // For debugging purposes.
    switch(m_mbcType) {
        case MBC_Type::None: m_mbcName = "ROM ONLY"; break;
        case MBC_Type::Mbc1: m_mbcName = "MBC1"; break;
        case MBC_Type::Mbc2: m_mbcName = "MBC2"; break;
        case MBC_Type::Mbc3: m_mbcName = "MBC3"; break;
        case MBC_Type::Mbc5: m_mbcName = "MBC5"; break;
        case MBC_Type::Mbc6: m_mbcName = "MBC6"; break;
        case MBC_Type::Mbc7: m_mbcName = "MBC7"; break;
        case MBC_Type::Mmm01: m_mbcName = "MMM01"; break;
        case MBC_Type::PocketCamera: m_mbcName = "POCKET CAMERA"; break;
        case MBC_Type::BandaiTama5: m_mbcName = "BANDAI TAMA5"; break;
        case MBC_Type::HuC1: m_mbcName = "HuC1"; break;
        case MBC_Type::HuC3: m_mbcName = "HuC3"; break;
        default: m_mbcName = "UNKNOWN"; break;
    }

    // Not sure if these apply to all MBCs yet.
    m_register1SelectsRomBank = m_cartRom->GetLength() > 512 * 1024;
    if(m_cartRom->GetLength() > 16 * 1024) m_rom1Bank = 1;

    // Create cartridge RAM (if applicable).
    if(m_mbcType == MBC_Type::Mbc2) {
        // MBC2 has 512 nibbles of RAM built in.
        m_cartHasRam = true;
        m_cartRam = new Mbc2Ram(512, 0xFF);
    } else if(cartRamBanks) {
        m_cartHasRam = true;
        m_cartRam = new PlipMemoryRam(0x2000 * cartRamBanks, 0xFF);
    }

    // Return the created cart RAM, or null.
    return m_cartHasRam ? m_cartRam : nullptr;
}

void GameBoyMapper::DisableBootRom() {
    AssignBlock(m_cartRom, 0x0000, 0x0000, 0x0100);
}

void GameBoyMapper::EnableCartridgeRam(const bool enable) {
    m_ramEnabled = enable;

    if(m_cartHasRam) {
        m_cartRam->SetReadable(m_ramEnabled);
        m_cartRam->SetWritable(m_ramEnabled);
    }
}

std::map<std::string, Plip::DebugValue> GameBoyMapper::GetMbcDebugInfo() const {
    return {
            { "Type", DebugValue(m_mbcName) },
            { "Banking Mode", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankingMode)) },
            { "Bank Register 0", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankRegister0)) },
            { "Bank Register 1", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankRegister1)) },
            { "RAM Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ramBank)) },
            { "RAM Bank Count", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_cartRamBanks)) },
            { "RAM Enabled", DebugValue(m_ramEnabled) },
            { "ROM $0000 Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_rom0Bank)) },
            { "ROM $4000 Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_rom1Bank)) },
        };
}

void GameBoyMapper::RemapMemory(const bool remapRom, const bool remapRam) {
    if(remapRom) {
        UnassignBlock(RomBank0Address, RomBank0Length + RomBank1Length);

        const auto romLength = m_cartRom->GetLength();
        const auto rom0Offset = (RomBank0Length * m_rom0Bank) % romLength;
        const auto rom1Offset = (RomBank1Length * m_rom1Bank) % romLength;

        AssignBlock(m_cartRom, RomBank0Address, rom0Offset, RomBank0Length);
        AssignBlock(m_cartRom, RomBank1Address, rom1Offset, RomBank1Length);
    }

    if(remapRam && m_cartHasRam) {
        UnassignBlock(CartRamAddress, CartRamLength);

        const auto ramLength = m_cartRam->GetLength();
        const auto ramOffset = (CartRamLength * m_ramBank) % ramLength;
        AssignBlock(m_cartRam, CartRamAddress, ramOffset, CartRamLength);
    }
}

void GameBoyMapper::Reset() {
    UnassignAllBlocks();

    // Initialize system memory map.
    AssignBlock(m_videoRam, VideoRamAddress);
    AssignBlock(m_workRam, WorkRamAddress);
    AssignBlock(m_oam, OamAddress);
    AssignBlock(m_unusable, UnusableAddress);
    AssignBlock(m_ioRegisters, IoRegistersAddress);
    AssignBlock(m_highRam, HighRamAddress);
    AssignBlock(m_cartRom, RomBank0Address, 0, RomBank0Length + RomBank1Length);

    // Map cartridge memory (if it exists) and disable it by default.
    if(m_cartHasRam) {
        if(m_mbcType == MBC_Type::Mbc2) {
            // MBC2 has a very limited amount of memory, so it's expected to repeatedly
            // wrap around the $A000-BFFF region.
            const auto mbc2RamLength = m_cartRam->GetLength();
            for(auto i = CartRamAddress; i < CartRamAddress + CartRamLength; i += mbc2RamLength) {
                AssignBlock(m_cartRam, i, 0, mbc2RamLength);
            }
        } else {
            AssignBlock(m_cartRam, CartRamAddress, 0, CartRamLength);
        }
        
        EnableCartridgeRam(false);
    }

    // Load the boot ROM into 0x0000-0x00FF (overlaying the cartridge ROM).
    AssignBlock(m_bootRom, 0x0000, 0x0000, 0x0100);
}

void GameBoyMapper::RestoreCartridgeMemoryAccessibility() const {
    if(m_cartHasRam) {
        m_cartRam->SetReadable(m_ramEnabled);
        m_cartRam->SetWritable(m_ramEnabled);
    }
}

void GameBoyMapper::SetByte(const uint32_t address, const uint8_t value, const bool privileged) {
    bool mbcHandledWrite = false;

    switch(m_mbcType) {
        case MBC_Type::Mbc1:
            mbcHandledWrite = SetByte_Mbc1(address, value);
            break;
        case MBC_Type::Mbc2:
            mbcHandledWrite = SetByte_Mbc2(address, value);
            break;
        case MBC_Type::Mbc3:
            mbcHandledWrite = SetByte_Mbc3(address, value);
            break;
        case MBC_Type::Mbc5:
        case MBC_Type::Mbc6:
        case MBC_Type::Mbc7:
        case MBC_Type::Mmm01:
        case MBC_Type::PocketCamera:
        case MBC_Type::BandaiTama5:
        case MBC_Type::HuC1:
        case MBC_Type::HuC3:
        case MBC_Type::None:
        default:
            break;
    }

    if(!mbcHandledWrite) {
        PlipMemoryMap::SetByte(address, value, privileged);
    }
}

bool GameBoyMapper::SetByte_Mbc1(const uint32_t address, const uint8_t value) {
    bool bankSwitch = false;

    if(address > 0x7FFF) return false;

    if(address < 0x2000) {
        // RAM enable.
        EnableCartridgeRam((value & 0xF) == 0xA);
    } else if(address < 0x4000) {
        // Bank register 0 (ROM bank selector).
        m_bankRegister0 = value & 0b11111;
        bankSwitch = true;
    } else if(address < 0x6000) {
        // Bank register 1 (RAM bank selector or upper ROM bank selector).
        m_bankRegister1 = value & 0b11;
        bankSwitch = true;
    } else {
        // Banking mode select.
        m_bankingMode = value & 0b1;
        bankSwitch = true;
    }

    // Swap banks if requested.
    if(bankSwitch) {
        m_rom1Bank = m_bankRegister0;
        if(m_rom1Bank == 0) {
            // If register 0 is 0, automatically bump it to 1.
            m_rom1Bank = 1;
        }

        if(m_register1SelectsRomBank) {
            // ROM is greater than 512 megabits. Register 1 selects higher ROM banks.
            m_rom1Bank |= m_bankRegister1 << 5;
        }

        if(m_bankingMode == 0) {
            // Simple banking mode--only bank switch $4000-$7FFF. RAM is locked on bank 0.
            m_rom0Bank = 0;
            m_ramBank = 0;
        } else {
            // Advanced banking mode--bank switch $0000-$3FFF (or RAM on $A000-BFFF) using register 1.
            if(m_register1SelectsRomBank) {
                m_rom0Bank = m_bankRegister1 << 5;
            } else {
                m_ramBank = m_bankRegister1;
            }
        }

        // Remap memory.
        RemapMemory(true, true);
    }

    return true;
}

bool GameBoyMapper::SetByte_Mbc2(const uint32_t address, const uint8_t value) {
    if(address > 0x3FFF) return false;

    if(BIT_TEST(address, 8)) {
        // ROM bank switch.
        m_bankRegister0 = value & 0b1111;
        m_rom1Bank = m_bankRegister0 > 0 ? m_bankRegister0 : 1;
        RemapMemory(true, false);
    } else {
        // RAM enable/disable.
        EnableCartridgeRam((value & 0xF) == 0xA);
    }

    return true;
}

bool GameBoyMapper::SetByte_Mbc3(const uint32_t address, const uint8_t value) {
    bool bankSwitchRom = false;
    bool bankSwitchRam = false;

    if(address < 0x2000) {
        // RAM enable.
        EnableCartridgeRam((value & 0xF) == 0xA);
    } else if(address < 0x4000) {
        // Bank register 0 (ROM bank selector).
        m_bankRegister0 = value & 0b01111111;
        bankSwitchRom = true;
    } else if(address < 0x6000) {
        // Bank register 1 (RAM bank or RTC register selector).
        if(value < 0x08) {
            m_bankRegister1 = value;
            bankSwitchRam = true;
        } else {
            // RTC register.
            // TODO: Implement RTC.
        }
    } else if(address < 0x8000) {
        // Latch clock data.
        // TODO: Implement RTC.
    } else if(address >= 0xA000 && address < 0xC000) {
        // RTC register 08-0C.
        // TODO: Implement RTC.
        return false;
    } else {
        return false;
    }

    // Swap banks if requested.
    if(bankSwitchRom || bankSwitchRam) {
        if(bankSwitchRom) {
            m_rom1Bank = m_bankRegister0;

            if(m_rom1Bank == 0) {
                // If register 0 is 0, automatically bump it to 1.
                m_rom1Bank = 1;
            }
        }

        if(bankSwitchRam) {
            m_ramBank = m_bankRegister1;
        }
        
        // Remap memory.
        RemapMemory(bankSwitchRom, bankSwitchRam);
    }

    return true;
}
