/* GameBoyInstance.Mbc.cpp
 *
 * Emulates the GameBoy's various Memory Bank Controllers (MBC).
 */

#include <iostream>

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::MBC_Cycle() {
    if(m_mbc == MBC_Type::None) return;

    // Only worry about MBC1 for now since some tests (blargg) require them.
    // We'll refactor this as necessary when we add support for more.
    const auto lastWrittenAddress = m_memory->LastWrittenAddress;
    const auto lastWrittenValue = m_memory->LastWrittenValue;

    bool bankSwitch = false;

    if(lastWrittenAddress > 0x7FFF) return;

    if(lastWrittenAddress < 0x2000) {
        // RAM enable.
        m_mbcRamEnable = (lastWrittenValue & 0xF) == 0xA;

        if(m_hasRam) {
            m_cartRam->SetReadable(m_mbcRamEnable);
            m_cartRam->SetWritable(m_mbcRamEnable);
        }
    } else if(lastWrittenAddress < 0x4000) {
        // Bank register 0 (ROM bank selector).
        m_mbcBankRegister0 = lastWrittenValue & 0b11111;
        bankSwitch = true;
    } else if(lastWrittenAddress < 0x6000) {
        // Bank register 1 (RAM bank selector or upper ROM bank selector).
        m_mbcBankRegister1 = lastWrittenValue & 0b11;
        bankSwitch = true;
    } else {
        // Banking mode select.
        m_mbcBankingMode = lastWrittenValue & 0b1;
        bankSwitch = true;
    }

    // Clear the last written values to ensure that the request isn't immediately
    // serviced again.
    m_memory->LastWrittenAddress = 0xFFFF;
    m_memory->LastWrittenValue = 0;

    // Swap banks if requested.
    if(bankSwitch) {
        m_mbcRom1Bank = m_mbcBankRegister0;
        if(m_mbcRom1Bank == 0) {
            // If register 0 is 0, automatically bump it to 1.
            m_mbcRom1Bank = 1;
        }

        if(m_mbcRegister1SelectsRomBank) {
            // ROM is greater than 512MB, register 1 selects higher ROM banks.
            m_mbcRom1Bank |= m_mbcBankRegister1 << 5;
        }

        if(m_mbcBankingMode == 0) {
            // Simple banking mode--only bank switch $4000-$7FFF. RAM is locked on bank 0.
            m_mbcRom0Bank = 0;
            m_mbcRamBank = 0;
        } else {
            // Advanced banking mode--bank switch $0000-$3FFF (or RAM on $A000-BFFF) using register 1.
            if(m_mbcRegister1SelectsRomBank) {
                m_mbcRom0Bank = m_mbcBankRegister1 << 5;
            } else {
                m_mbcRamBank = m_mbcBankRegister1;
            }
        }

        // Remap memory.
        m_memory->UnassignBlock(RomBank0Address, RomBank0Length + RomBank1Length);
        m_memory->UnassignBlock(CartRamAddress, CartRamLength);

        const auto romLength = m_rom->GetLength();
        const auto rom0Offset = (RomBank0Length * m_mbcRom0Bank) % romLength;
        const auto rom1Offset = (RomBank1Length * m_mbcRom1Bank) % romLength;

        m_memory->AssignBlock(m_rom, RomBank0Address, rom0Offset, RomBank0Length);
        m_memory->AssignBlock(m_rom, RomBank1Address, rom1Offset, RomBank1Length);

        if(m_hasRam) {
            const auto ramLength = m_cartRam->GetLength();
            const auto ramOffset = (CartRamLength * m_mbcRamBank) % ramLength;
            m_memory->AssignBlock(m_cartRam, CartRamAddress, ramOffset, CartRamLength);
        }
    }
}

void GameBoyInstance::MBC_Init() {
    switch(m_mbc) {
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

    m_mbcRegister1SelectsRomBank = m_rom->GetLength() > 512 * 1024;

    if(m_rom->GetLength() > 16 * 1024) m_mbcRom1Bank = 1;
}

std::map<std::string, Plip::DebugValue> GameBoyInstance::MBC_GetDebugInfo() const {
    return {
        { "Type", DebugValue(m_mbcName) },
        { "Banking Mode", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_mbcBankingMode)) },
        { "Bank Register 0", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_mbcBankRegister0)) },
        { "Bank Register 1", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_mbcBankRegister1)) },
        { "RAM Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_mbcRamBank)) },
        { "RAM Enabled", DebugValue(m_mbcRamEnable) },
        { "ROM $0000 Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_mbcRom0Bank)) },
        { "ROM $4000 Bank", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_mbcRom1Bank)) },
    };
}
