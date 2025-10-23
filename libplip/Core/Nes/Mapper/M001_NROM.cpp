/* M001_NROM.cpp
 *
 * Implements an NROM "mapper."
 */

#include "M001_NROM.h"

using Plip::Core::Nes::Mapper::Mapper001_NROM;

Mapper001_NROM::Mapper001_NROM(
    [[maybe_unused]] const bool legacyINes, const int mapperId, const int submapperId,
    PlipMemory* ppuRam, PlipMemory* prgRom, PlipMemory* chrRom, PlipMemory* trainer,
    const int prgRamSize, [[maybe_unused]] const int chrRamSize,
    [[maybe_unused]] const int prgNvramSize, [[maybe_unused]] const int chrNvramSize
) : NesMapper(mapperId, submapperId, ppuRam, prgRom, chrRom, trainer) {
    
    if(m_prgRamSize = prgRamSize; m_prgRamSize > 0) {
        m_prgRam = new PlipMemoryRam(m_prgRamSize);
    }
}

uint8_t Mapper001_NROM::GetBytePpu(uint32_t address, const bool privileged) const {
    if(address >= 0x2000) {
        // PPU RAM
        address -= 0x2000;
        return m_ppuRam->GetByte(address % m_ppuRam->GetLength(), privileged);
    }

    return m_chrRom->GetByte(address % m_chrRom->GetLength(), privileged);
}

void Mapper001_NROM::SetBytePpu(uint32_t address, const uint8_t value, const bool privileged) {
    if(address >= 0x2000) {
        // PPU RAM
        address -= 0x2000;
        m_ppuRam->SetByte(address % m_ppuRam->GetLength(), value, privileged);
    }
}

uint8_t Mapper001_NROM::GetByteSys(uint32_t address, const bool privileged) const {
    if(address < 0x6000) {
        return 0xFF;
    }
    
    if(m_prgRamSize > 0 && address < 0x8000) {
        // PRG RAM access. Mirror if less than 8KiB.
        address -= 0x6000;
        return m_prgRam->GetByte(address % m_prgRamSize, privileged);
    }

    address -= 0x8000;
    return m_prgRom->GetByte(address % m_prgRom->GetLength(), privileged);
}

void Mapper001_NROM::SetByteSys(uint32_t address, uint8_t value, bool privileged) {
    if(m_prgRamSize > 0 && address >= 0x6000 && address < 0x8000) {
        address -= 0x6000;
        m_prgRam->SetByte(address % m_prgRamSize, value, privileged);
    }
}
