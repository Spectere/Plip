/* M001_NROM.cpp
 *
 * Implements an NROM "mapper."
 */

#include "M001_NROM.h"

namespace Plip::Core::Nes::Mapper {
    Mapper001_NROM::Mapper001_NROM(
        [[maybe_unused]] const bool legacyINes, const int mapperId, const int submapperId,
        PlipMemory* prgRom, PlipMemory* chrRom, PlipMemory* trainer,
        const int prgRamSize, [[maybe_unused]] const int chrRamSize,
        [[maybe_unused]] const int prgNvramSize, [[maybe_unused]] const int chrNvramSize
    ) : NesMapper(mapperId, submapperId, prgRom, chrRom, trainer) {
        
        if(m_prgRamSize = prgRamSize; m_prgRamSize > 0) {
            m_prgRam = new PlipMemoryRam(m_prgRamSize);
        }
    }

    uint8_t Mapper001_NROM::GetByteChr(uint32_t address, bool privileged) const {
        return m_chrRom->GetByte(address % m_chrRom->GetLength(), privileged);
    }

    void Mapper001_NROM::SetByteChr(uint32_t address, uint8_t value, bool privileged) { }

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
}
