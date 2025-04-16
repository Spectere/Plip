/* GameBoyMapper.cpp
 *
 * A custom PlipMemoryMap that emulates the Game Boy's MBC chips.
 */

#include <ctime>

#include "GameBoyMapper.h"
#include "Mbc2Ram.h"

using Plip::Core::GameBoy::GameBoyMapper;

GameBoyMapper::GameBoyMapper(PlipMemory* bootRom, PlipMemory* cartRom, PlipMemory* videoRam, PlipMemory* workRam, PlipMemory* oam, PlipMemory* ioRegisters, PlipMemory* highRam)
                            : m_bootRom(bootRom), m_cartRom(cartRom), m_videoRam(videoRam), m_workRam(workRam), m_oam(oam), m_ioRegisters(ioRegisters), m_highRam(highRam) {
    // Pull invalid and unreadable values high.
    SetInvalidByte(0xFF);
}

Plip::PlipMemory* GameBoyMapper::ConfigureMapper(const MBC_Type mbcType, const bool hasRtc, const int cartRamBanks) {
    m_mbcType = mbcType;
    m_hasRtc = hasRtc;
    m_cartRamBanks = cartRamBanks;

    // Catch unimplemented mappers.
    // ReSharper disable once CppIncompleteSwitchStatement
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch(m_mbcType) {
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
    // Remap bank 0. This allows this to work with all models with no muss or fuss.
    UnassignBlock(0x0000, RomBank0Length);
    AssignBlock(m_cartRom, 0x0000, 0x0000, RomBank0Length);
}

void GameBoyMapper::EnableCartridgeRam(const bool enable) {
    m_ramEnabled = enable;

    if(m_cartHasRam) {
        m_cartRam->SetReadable(m_ramEnabled);
        m_cartRam->SetWritable(m_ramEnabled);
    }
}

uint8_t GameBoyMapper::GetByte(const uint32_t address, const bool privileged) const {
    switch(m_mbcType) {
        case MBC_Type::Mbc3:
            return GetByte_Mbc3(address, privileged);
        case MBC_Type::None:
        case MBC_Type::Mbc1:
        case MBC_Type::Mbc2:
        case MBC_Type::Mbc5:
        case MBC_Type::Mbc6:
        case MBC_Type::Mbc7:
        case MBC_Type::Mmm01:
        case MBC_Type::PocketCamera:
        case MBC_Type::BandaiTama5:
        case MBC_Type::HuC1:
        case MBC_Type::HuC3:
        default:
            return PlipMemoryMap::GetByte(address, privileged);
    }
}

uint8_t GameBoyMapper::GetByte_Mbc3(const uint32_t address, const bool privileged) const {
    if((address >= 0xA000 && address <= 0xBFFF) && (m_ramBank >= 0x08 && m_ramBank <= 0x0C)) {
        return RTC_RegisterGet(m_ramBank);
    }

    return PlipMemoryMap::GetByte(address, privileged);
}

std::map<std::string, Plip::DebugValue> GameBoyMapper::GetMbcDebugInfo() const {
    std::map<std::string, DebugValue> debugList {
        { "Type", DebugValue(m_mbcName) },
        { "Banking Mode", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankingMode)) },
        { "Bank Register 0", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankRegister0)) },
        { "Bank Register 1", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankRegister1)) },
        { "Bank Register 2", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankRegister2)) },
        { "RAM Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ramBank)) },
        { "RAM Bank Count", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_cartRamBanks)) },
        { "RAM Enabled", DebugValue(m_ramEnabled) },
        { "ROM $0000 Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_rom0Bank)) },
        { "ROM $4000 Bank", DebugValue(DebugValueType::Int16Le, static_cast<uint64_t>(m_rom1Bank)) },
        { "VRAM Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_vramBank) )},
        { "WRAM Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_wramBank) )},
    };

    if(m_hasRtc) {
        debugList.insert(std::pair{"RTC Sub-Seconds", DebugValue(DebugValueType::Int32Le, static_cast<uint64_t>(m_rtcMachineCycles))});
        debugList.insert(std::pair{"RTC $08 - Seconds", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_rtcRegisters.Seconds))});
        debugList.insert(std::pair{"RTC $09 - Minutes", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_rtcRegisters.Minutes))});
        debugList.insert(std::pair{"RTC $0A - Hours", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_rtcRegisters.Hours))});
        debugList.insert(std::pair{"RTC $0B - Days", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_rtcRegisters.Days))});
        debugList.insert(std::pair{"RTC $0C - Flags", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_rtcRegisters.Flags))});
    }

    return debugList;
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

    // Load the boot ROM into 0x0000-0x00FF (and 0x0200-0x08FF for CGB) (overlaying the cartridge ROM).
    AssignBlock(m_bootRom, 0x0000, 0x0000, 0x0100);
    m_largeBootRom = m_bootRom->GetLength() > 0x0100;

    if(m_largeBootRom) {
        // Boot ROM is extended. Map the rest of it to 0x0200.

        if(m_bootRom->GetLength() > 0x0800) {
            // 0x0100-0x0200 are probably 0x00. Skip those and map from 0x0200-0x08FF.
            AssignBlock(m_bootRom, 0x0200, 0x0200, 0x0700);
        } else {
            // The ROM is (hopefully) 2048 bytes long. Translate 0x0100-0x07FF to 0x0200-0x08FF.
            AssignBlock(m_bootRom, 0x0200, 0x0100, 0x0700);
        }
    }
}

void GameBoyMapper::RestoreCartridgeMemoryAccessibility() const {
    if(m_cartHasRam) {
        m_cartRam->SetReadable(m_ramEnabled);
        m_cartRam->SetWritable(m_ramEnabled);
    }
}

void GameBoyMapper::RTC_Clock() {
    if(BIT_TEST(m_rtcRegisters.Flags, 6)) return;  // RTC is halted.

    if(--m_rtcMachineCycles == 0) {
        RTC_ResetSubSecondClock();
        RTC_Increment();
    }
}

void GameBoyMapper::RTC_Dump(std::fstream &file) const {
    auto timestamp = std::time(nullptr);
    file.write(reinterpret_cast<std::istream::char_type*>(&timestamp), sizeof(timestamp));
    file.put(m_rtcRegisters.Seconds);
    file.put(m_rtcRegisters.Minutes);
    file.put(m_rtcRegisters.Hours);
    file.put(m_rtcRegisters.Days);
    file.put(m_rtcRegisters.Flags);
    file.flush();
}

void GameBoyMapper::RTC_Increment() {
    if(++m_rtcRegisters.Seconds == 60) {
        m_rtcRegisters.Seconds = 0;
        
        if(++m_rtcRegisters.Minutes == 60) {
            m_rtcRegisters.Minutes = 0;
            
            if(++m_rtcRegisters.Hours == 24) {
                m_rtcRegisters.Hours = 0;
                
                if(++m_rtcRegisters.Days == 0) {
                    // The low days counter wrapped around.
                    m_rtcRegisters.Flags ^= 0b1;

                    if((m_rtcRegisters.Flags & 0b1) == 0) {
                        // The days counter overflowed. Set the carry bit.
                        m_rtcRegisters.Flags |= 0b10000000;
                    }
                }
            }
        }
    }

    // Normalize the registers (ensure that they don't exceed their intended bit size, and that they roll around
    // to zero at 64/32 instead of 256).
    m_rtcRegisters.Seconds &= 0b00111111;
    m_rtcRegisters.Minutes &= 0b00111111;
    m_rtcRegisters.Hours   &= 0b00011111;
}

void GameBoyMapper::RTC_LatchRegisters() {
    m_rtcLatchedRegisters.Days = m_rtcRegisters.Days;
    m_rtcLatchedRegisters.Flags = m_rtcRegisters.Flags;
    m_rtcLatchedRegisters.Hours = m_rtcRegisters.Hours;
    m_rtcLatchedRegisters.Minutes = m_rtcRegisters.Minutes;
    m_rtcLatchedRegisters.Seconds = m_rtcRegisters.Seconds;
}

void GameBoyMapper::RTC_Load(std::fstream &file) {
    const auto now = std::time(nullptr);
    time_t savedTimestamp;
    uint8_t rtcSeconds, rtcMinutes, rtcHours, rtcDays, rtcFlags;

    file.read(reinterpret_cast<std::istream::char_type*>(&savedTimestamp), sizeof(savedTimestamp));
    file.get(reinterpret_cast<std::istream::char_type&>(rtcSeconds));
    file.get(reinterpret_cast<std::istream::char_type&>(rtcMinutes));
    file.get(reinterpret_cast<std::istream::char_type&>(rtcHours));
    file.get(reinterpret_cast<std::istream::char_type&>(rtcDays));
    file.get(reinterpret_cast<std::istream::char_type&>(rtcFlags));

    // If the timer halt bit is set, don't attempt to catch up. Just write the values into the
    // registers as-is.
    if(BIT_TEST(rtcFlags, 6)) {
        m_rtcRegisters.Seconds = rtcSeconds;
        m_rtcRegisters.Minutes = rtcMinutes;
        m_rtcRegisters.Hours = rtcHours;
        m_rtcRegisters.Days = rtcDays;
        m_rtcRegisters.Flags = rtcFlags;
        return;
    }

    // Figure out how much time passed between the RTC dump and now, create a timestamp from the GB's RTC,
    // and work out what the final timestamp should be.
    const auto diff = now - savedTimestamp;
    const auto gbRtcTotalDays = ((rtcFlags & 0b1) << 8) | rtcDays;
    auto gbRtcTimestamp = (gbRtcTotalDays * 60 * 60 * 24)
                        + (rtcHours * 60 * 60)
                        + (rtcMinutes * 60)
                        + rtcSeconds
                        + diff;

    // Stuff the final value into the RTC fields.
    const uint16_t days = gbRtcTimestamp / 60 / 60 / 24;
    m_rtcRegisters.Days = days & 0xFF;
    m_rtcRegisters.Flags = (rtcFlags & 0b11111110) | ((days >> 8) & 0b1);
    gbRtcTimestamp -= days * 60 * 60 * 24;

    m_rtcRegisters.Hours = gbRtcTimestamp / 60 / 60;
    gbRtcTimestamp -= m_rtcRegisters.Hours * 60 * 60;

    m_rtcRegisters.Minutes = gbRtcTimestamp / 60;
    gbRtcTimestamp -= m_rtcRegisters.Minutes * 60;

    m_rtcRegisters.Seconds = gbRtcTimestamp;
}

uint8_t GameBoyMapper::RTC_RegisterGet(const int index) const {
    switch(index) {
        case 0x08: return m_rtcLatchedRegisters.Seconds;
        case 0x09: return m_rtcLatchedRegisters.Minutes;
        case 0x0A: return m_rtcLatchedRegisters.Hours;
        case 0x0B: return m_rtcLatchedRegisters.Days;
        case 0x0C: return m_rtcLatchedRegisters.Flags;
        default: return 0xFF;
    }
}

void GameBoyMapper::RTC_RegisterSet(const int index, const uint8_t value) {
    switch(index) {
        case 0x08:
            m_rtcLatchedRegisters.Seconds = m_rtcRegisters.Seconds = value & 0b00111111;
            RTC_ResetSubSecondClock();
            break;

        case 0x09:
            m_rtcLatchedRegisters.Minutes = m_rtcRegisters.Minutes = value & 0b00111111;
            break;

        case 0x0A:
            m_rtcLatchedRegisters.Hours = m_rtcRegisters.Hours = value & 0b00011111;
            break;

        case 0x0B:
            m_rtcLatchedRegisters.Days = m_rtcRegisters.Days = value & 0b11111111;
            break;

        case 0x0C:
            m_rtcLatchedRegisters.Flags = m_rtcRegisters.Flags = value & 0b11000001;
            break;
        
        default: break;
    }
}

void GameBoyMapper::RTC_ResetSubSecondClock() {
    m_rtcMachineCycles = m_rtcCpuClockRate;
}

void GameBoyMapper::RTC_SetCpuClockRate(const int clockRate) {
    m_rtcCpuClockRate = clockRate;
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
            mbcHandledWrite = SetByte_Mbc5(address, value);
            break;
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
        m_bankRegister1 = value;
        m_ramBank = m_bankRegister1;
        if(value < 0x04) {
            bankSwitchRam = true;
        }
    } else if(address < 0x8000) {
        // RTC register latch.
        if(value == 1 && m_rtcLatchLastValueWritten == 0) {
            RTC_LatchRegisters();
        }
    } else if(address >= 0xA000 && address < 0xC000) {
        // RTC register 08-0C.
        if(m_ramBank >= 0x08 && m_ramBank <= 0x0C) {
            RTC_RegisterSet(m_ramBank, value);
        } else {
            return false;
        }
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
        
        // Remap memory.
        RemapMemory(bankSwitchRom, bankSwitchRam);
    }

    return true;
}

bool GameBoyMapper::SetByte_Mbc5(const uint32_t address, const uint8_t value) {
    bool bankSwitchRom = false;
    bool bankSwitchRam = false;

    if(address >= 0x6000) return false;

    if(address < 0x2000) {
        // RAM enable.
        EnableCartridgeRam((value & 0xF) == 0xA);
    } else if(address < 0x3000) {
        // Bank register 0 (ROM bank low bits).
        m_bankRegister0 = value;
        bankSwitchRom = true;
    } else if(address < 0x4000) {
        // Bank register 1 (ROM bank high bit).
        m_bankRegister1 = value & 0b1;
        bankSwitchRom = true;
    } else {
        // RAM bank selector.
        m_bankRegister2 = value & 0b1111;
        bankSwitchRam = true;
    }

    // Swap banks if requested.
    if(bankSwitchRom || bankSwitchRam) {
        if(bankSwitchRom) {
            m_rom1Bank = (m_bankRegister1 << 8) | m_bankRegister0;
        }
        
        // Remap memory.
        RemapMemory(bankSwitchRom, bankSwitchRam);
    }

    return true;
}

void GameBoyMapper::SetVideoRamBank(const int bank) {
    m_vramBank = bank;

    UnassignBlock(VideoRamAddress, VideoRamLength);
    AssignBlock(m_videoRam, VideoRamAddress, VideoRamLength * m_vramBank, VideoRamLength);
}

void GameBoyMapper::SetWorkRamBank(const int bank) {
    m_wramBank = bank;
    
    UnassignBlock(WorkRamAddress, WorkRamLength);
    AssignBlock(m_workRam, WorkRamAddress, WorkRamLength * m_wramBank, WorkRamLength);
}
