/* GameBoyMapper.cpp
 *
 * A custom PlipMemoryMap that emulates the Game Boy's MBC chips.
 */

#include <cassert>
#include <ctime>

#include "GameBoyMapper.h"

#include "Mbc2Ram.h"

using Plip::Core::GameBoy::GameBoyMapper;

GameBoyMapper::GameBoyMapper(PlipMemory* bootRom, PlipMemory* cartRom, PlipMemory* videoRam, PlipMemory* workRam, PlipMemory* oam, PlipMemory* ioRegisters, PlipMemory* highRam)
                            : m_bootRom(bootRom), m_cartRom(cartRom), m_videoRam(videoRam), m_workRam(workRam), m_oam(oam), m_ioRegisters(ioRegisters), m_highRam(highRam) {
    // Pull invalid and unreadable values high.
    SetInvalidByte(0xFF);
}

void GameBoyMapper::AssignBlock(PlipMemory* memory, uint32_t address, uint32_t offset) {
    throw PlipEmulationException("GameBoyMapper::AssignBlock must not be used.");
}

void GameBoyMapper::AssignBlock(PlipMemory* memory, uint32_t address, uint32_t offset, uint32_t length) {
    throw PlipEmulationException("GameBoyMapper::AssignBlock must not be used.");
}

void GameBoyMapper::UnassignAllBlocks() {
    throw PlipEmulationException("GameBoyMapper::UnassignAllBlocks must not be used.");
}

void GameBoyMapper::UnassignBlock(uint32_t address, uint32_t length) {
    throw PlipEmulationException("GameBoyMapper::UnassignBlock must not be used.");
}

Plip::PlipMemory* GameBoyMapper::ConfigureMapper(const MBC_Type mbcType, const bool hasRtc, const int cartRamBanks) {
    m_mbcType = mbcType;
    m_hasRtc = hasRtc;
    m_cartRamBankCount = cartRamBanks;
    m_cartRomBankCount = m_cartRom->GetLength() / RomBank0Length;

    // Catch unimplemented mappers.
    switch(m_mbcType) {
        case MBC_Type::Mbc6: throw PlipEmulationException("Unsupported mapper: MBC6");
        case MBC_Type::Mbc7: throw PlipEmulationException("Unsupported mapper: MBC7");
        case MBC_Type::Mmm01: throw PlipEmulationException("Unsupported mapper: MMM01");
        case MBC_Type::PocketCamera: throw PlipEmulationException("Unsupported mapper: Pocket Camera");
        case MBC_Type::BandaiTama5: throw PlipEmulationException("Unsupported mapper: Bandai TAMA5");
        case MBC_Type::HuC3: throw PlipEmulationException("Unsupported mapper: HuC3");

        // Shut up, clang.
        case MBC_Type::Mbc1:
        case MBC_Type::Mbc1M:
        case MBC_Type::Mbc2:
        case MBC_Type::Mbc3:
        case MBC_Type::Mbc30:
        case MBC_Type::Mbc5:
        case MBC_Type::HuC1:
        case MBC_Type::None:
        case MBC_Type::Unknown:
            break;
    }

    // For debugging purposes.
    switch(m_mbcType) {
        case MBC_Type::None: m_mbcName = "ROM ONLY"; break;
        case MBC_Type::Mbc1: m_mbcName = "MBC1"; break;
        case MBC_Type::Mbc1M: m_mbcName = "MBC1M"; break;
        case MBC_Type::Mbc2: m_mbcName = "MBC2"; break;
        case MBC_Type::Mbc3: m_mbcName = "MBC3"; break;
        case MBC_Type::Mbc30: m_mbcName = "MBC30"; break;
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
    if(m_cartRom->GetLength() > 16 * 1024) m_rom1Bank = 1;

    // MBC1(M) allows the second register to act as high bits for the ROM bank selection.
    if(m_mbcType == MBC_Type::Mbc1 || m_mbcType == MBC_Type::Mbc1M) {
        m_register1SelectsRomBank = m_cartRom->GetLength() > 512 * 1024;
    }

    // Create cartridge RAM (if applicable).
    if(m_mbcType == MBC_Type::Mbc2) {
        // MBC2 has 512 nibbles of RAM built in.
        m_cartHasRam = true;
        m_cartRam = new Mbc2Ram(512, 0xFF);
    } else if(cartRamBanks) {
        m_cartHasRam = true;
        m_cartRam = new PlipMemoryRam(0x2000 * cartRamBanks, 0xFF);
    }

    // Cartridge RAM for HuC1 is always writable.
    if(m_mbcType == MBC_Type::HuC1 && m_cartHasRam) m_ramEnabled = true;

    // Return the created cart RAM, or null.
    return m_cartHasRam ? m_cartRam : nullptr;
}

void GameBoyMapper::DisableBootRom() {
    m_bootRomAccessible = false;
}

void GameBoyMapper::EnableCartridgeRam(const bool enable) {
    m_ramEnabled = enable;

    if(m_cartHasRam) {
        m_cartRam->SetReadable(m_ramEnabled);
        m_cartRam->SetWritable(m_ramEnabled);
    }
}

uint8_t GameBoyMapper::GetByte(const uint32_t address, const bool privileged) const {
    // Boot ROM handler.
    if(m_bootRomAccessible) {
        if(address < 0x100) {
            return m_bootRom->GetByte(address, privileged);
        }

        if(m_largeBootRom && (address >= 0x200 && address < 0x900)) {
            // CGB extensions. 0x100-0x1FF is zeroed on all the dumps I've seen (and SameBoy's ROM set),
            // so we don't need to adjust the address.
            return m_bootRom->GetByte(address, privileged);
        }
    }

    // GB memory map.
    if(address == InterruptEnableAddress) return m_ie;
    if(address >= HighRamAddress) return m_highRam->GetByte(address & 0x7F, privileged);
    if(address >= IoRegistersAddress) return m_ioRegisters->GetByte(address & 0x7F, privileged);
    if(address >= UnusableAddress) return m_unusable->GetByte(address - 0xFEA0, privileged);
    if(address >= OamAddress) return m_oam->GetByte(address & 0xFF, privileged);
    if(address >= WorkRamAddress) {
        // WRAM and Echo RAM.
        if(address & 0x1000) {
            // Upper 4 KiB. CGB WRAM bank switching must be respected.
            return m_workRam->GetByte((0x1000 * m_wramBank) + (address & 0xFFF), privileged);
        }

        // Lower 4 KiB. Easy-peasy.
        return m_workRam->GetByte(address & 0xFFF, privileged);
    }
    if(address >= CartRamAddress) {
        // Mapper registers.
        if(m_mbcType == MBC_Type::Mbc3 || m_mbcType == MBC_Type::Mbc30) {
            if(m_cartRamBank >= 0x08 && m_cartRamBank <= 0x0C) {
                // RTC
                return RTC_RegisterGet(m_cartRamBank);
            }
        } else if(m_mbcType == MBC_Type::HuC1) {
            if(m_hucIrMode) {
                // IR receiver
                return 0xC0;  // IR receiver does not see light.
            }
        }

        // 8 KiB cart RAM mapping (if applicable).
        if(!m_cartHasRam) {
            // No cart RAM--open bus.
            return 0xFF;
        }

        return m_cartRam->GetByte((m_cartRamBank * CartRamLength) + (address & 0x1FFF), privileged);
    }
    if(address >= VideoRamAddress) {
        // VRAM
        return m_videoRam->GetByte((m_vramBank * VideoRamLength) + (address & 0x1FFF), privileged);
    }
    if(address >= RomBank1Address) return m_cartRom->GetByte((m_rom1Bank * RomBank1Length) + (address & 0x3FFF), privileged);
    return m_cartRom->GetByte((m_rom0Bank * RomBank0Length) + (address & 0x3FFF), privileged);
}

std::map<std::string, Plip::DebugValue> GameBoyMapper::GetMbcDebugInfo() const {
    std::map<std::string, DebugValue> debugList {
        { "Type", DebugValue(m_mbcName) },
        { "Banking Mode", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankingMode)) },
        { "Bank Register 0", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankRegister0)) },
        { "Bank Register 1", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankRegister1)) },
        { "Bank Register 2", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_bankRegister2)) },
        { "RAM Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_cartRamBank)) },
        { "RAM Bank Count", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_cartRamBankCount)) },
        { "RAM Enabled", DebugValue(m_ramEnabled) },
        { "ROM $0000 Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_rom0Bank)) },
        { "ROM $4000 Bank", DebugValue(DebugValueType::Int16Le, static_cast<uint64_t>(m_rom1Bank)) },
        { "ROM Bank Count", DebugValue(DebugValueType::Int16Le, static_cast<uint64_t>(m_cartRomBankCount)) },
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

void GameBoyMapper::Reset() {
    // Reset VRAM/WRAM banks.
    m_vramBank = 0;
    m_wramBank = 1;

    // Disable cart RAM by default (unless we're using HuC1).
    if(m_cartHasRam) {
        if(m_mbcType != MBC_Type::HuC1) EnableCartridgeRam(false);
    }

    // Reset boot ROM accessibility.
    m_bootRomAccessible = true;
    m_largeBootRom = m_bootRom->GetLength() > 0x0100;
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
    file.put(std::bit_cast<char>(m_rtcRegisters.Seconds));
    file.put(std::bit_cast<char>(m_rtcRegisters.Minutes));
    file.put(std::bit_cast<char>(m_rtcRegisters.Hours));
    file.put(std::bit_cast<char>(m_rtcRegisters.Days));
    file.put(std::bit_cast<char>(m_rtcRegisters.Flags));
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
            m_rtcRegisters.Seconds = value & 0b00111111;
            RTC_ResetSubSecondClock();
            break;

        case 0x09:
            m_rtcRegisters.Minutes = value & 0b00111111;
            break;

        case 0x0A:
            m_rtcRegisters.Hours = value & 0b00011111;
            break;

        case 0x0B:
            m_rtcRegisters.Days = value & 0b11111111;
            break;

        case 0x0C:
            m_rtcRegisters.Flags = value & 0b11000001;
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
    // Mapper register writes.
    bool mbcHandledWrite = false;
    switch(m_mbcType) {
        case MBC_Type::Mbc1:
        case MBC_Type::Mbc1M:
            mbcHandledWrite = SetByte_Mbc1(address, value);
            break;
        case MBC_Type::Mbc2:
            mbcHandledWrite = SetByte_Mbc2(address, value);
            break;
        case MBC_Type::Mbc3:
        case MBC_Type::Mbc30:
            mbcHandledWrite = SetByte_Mbc3(address, value);
            break;
        case MBC_Type::Mbc5:
            mbcHandledWrite = SetByte_Mbc5(address, value);
            break;
        case MBC_Type::HuC1:
            mbcHandledWrite = SetByte_HuC1(address, value);
            break;
        case MBC_Type::Mbc6:
        case MBC_Type::Mbc7:
        case MBC_Type::Mmm01:
        case MBC_Type::PocketCamera:
        case MBC_Type::BandaiTama5:
        case MBC_Type::HuC3:
        case MBC_Type::None:
        default:
            break;
    }

    if(mbcHandledWrite) return;

    if(address == InterruptEnableAddress) m_ie = value;
    else if(address >= HighRamAddress) m_highRam->SetByte(address & 0x7F, value, privileged);
    else if(address >= IoRegistersAddress) m_ioRegisters->SetByte(address & 0x7F, value, privileged);
    else if(address >= UnusableAddress) m_unusable->SetByte(address - 0xFEA0, value, privileged);
    else if(address >= OamAddress) m_oam->SetByte(address & 0xFF, value, privileged);
    else if(address >= WorkRamAddress) {
        // WRAM and Echo RAM.
        if(address & 0x1000) {
            // Upper 4 KiB. CGB WRAM bank switching must be respected
            m_workRam->SetByte((0x1000 * m_wramBank) + (address & 0xFFF), value, privileged);
        } else {
            // Lower 4 KiB
            m_workRam->SetByte(address & 0xFFF, value, privileged);
        }
    }
    else if(address >= CartRamAddress && m_cartHasRam) {
        // Cart RAM
        m_cartRam->SetByte((m_cartRamBank * CartRamLength) + (address & 0x1FFF), value, privileged);
    }
    else if(address >= VideoRamAddress) {
        // VRAM
        m_videoRam->SetByte((m_vramBank * VideoRamLength) + (address & 0x1FFF), value, privileged);
    }

    // ROM "writes" would be handled by the mapper (above).
}

bool GameBoyMapper::SetByte_HuC1(const uint32_t address, const uint8_t value) {
    if(address < 0x2000) {
        // IR select. Enable IR with $0E and disable (allow RAM access) with anything else.
        m_hucIrMode = (value & 0xF) == 0xE;
        m_ramEnabled = !m_hucIrMode;
    } else if(address < 0x4000) {
        // ROM bank selection.
        m_rom1Bank = m_bankRegister0 = value & 0b111111;
    } else if(address < 0x6000) {
        // RAM bank selection.
        m_cartRamBank = m_bankRegister1 = value & 0b11;
    } else if(address < 0x8000) {
        // Unknown--does not appear to do anything. Ignore this write.
    } else if(m_hucIrMode && address >= 0xA000 && address < 0xC000) { // NOLINT(*-branch-clone)
        // IR register. Ignore this write.
        return false;
    } else {
        return false;
    }

    return true;
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

        if(m_mbcType == MBC_Type::Mbc1M) {
            // On MBC1M, the bit 4 can be used to get around the 0/1 rule above, but addresses
            // are only 4-bits wide. Adjust here.
            m_rom1Bank &= 0b1111;
        }

        if(m_register1SelectsRomBank) {
            // ROM is greater than 512 megabits. Register 1 selects higher ROM banks.
            m_rom1Bank |= m_bankRegister1 << (m_mbcType == MBC_Type::Mbc1M ? 4 : 5);
        }

        if(m_bankingMode == 0) {
            // Simple banking mode--only bank switch $4000-$7FFF. RAM is locked on bank 0.
            m_rom0Bank = 0;
            m_cartRamBank = 0;
        } else {
            // Advanced banking mode--bank switch $0000-$3FFF (or RAM on $A000-BFFF) using register 1.
            if(m_register1SelectsRomBank) {
                m_rom0Bank = m_bankRegister1 << (m_mbcType == MBC_Type::Mbc1M ? 4 : 5);
            } else {
                m_cartRamBank = m_bankRegister1;
            }
        }
    }

    return true;
}

bool GameBoyMapper::SetByte_Mbc2(const uint32_t address, const uint8_t value) {
    if(address > 0x3FFF) return false;

    if(BIT_TEST(address, 8)) {
        // ROM bank switch.
        m_bankRegister0 = value & 0b1111;
        m_rom1Bank = m_bankRegister0 > 0 ? m_bankRegister0 : 1;
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
        m_bankRegister0 = value & (m_mbcType == MBC_Type::Mbc3 ? 0b01111111 : 0xFF);
        bankSwitchRom = true;
    } else if(address < 0x6000) {
        // Bank register 1 (RAM bank or RTC register selector).
        m_bankRegister1 = value;
        m_cartRamBank = m_bankRegister1;
        if(value < 0x04) {
            bankSwitchRam = true;
        }
    } else if(address < 0x8000) {
        // RTC register latch.
        if(BIT_TEST(value, 0) && !BIT_TEST(m_rtcLatchLastValueWritten, 0)) {
            RTC_LatchRegisters();
        }
        m_rtcLatchLastValueWritten = value;
    } else if(address >= 0xA000 && address < 0xC000) {
        // RTC register 08-0C.
        if(m_cartRamBank >= 0x08 && m_cartRamBank <= 0x0C) {
            RTC_RegisterSet(m_cartRamBank, value);
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
    }

    return true;
}

void GameBoyMapper::SetVideoRamBank(const int bank) {
    assert(m_largeBootRom);  // Invalid on DMG.  TODO: We should assert this based on active system type, NOT ROM size.

    m_vramBank = bank & 0b1;
}

void GameBoyMapper::SetWorkRamBank(const int bank) {
    assert(m_largeBootRom);  // Invalid on DMG.  TODO: We should assert this based on active system type, NOT ROM size.

    m_wramBank = bank & 0b111;
    if(m_wramBank == 0) m_wramBank = 1;  // Shift to bank 1.
}
