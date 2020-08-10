/* GameBoyInstance.Mbc.cpp
 *
 * Handles the emulation of the various memory bank controllers (MBC)
 * contained within game cartridges.
 */

#include <sstream>

#include "../../PlipEmulationException.h"
#include "../../PlipUtility.h"

#include "GameBoyInstance.h"

namespace Plip::Core::GameBoy {
    void GameBoyInstance::MbcInit() {
        switch(m_mbc) {
            case None: break;
            case Mbc1:
                m_mbcRomBank = 1;
                if(m_hasRam) {
                    m_cartRam->SetReadable(false);
                    m_cartRam->SetWritable(false);
                }
                break;
            default:
                std::stringstream ex;
                ex << "invalid/unsupported memory bank controller: "
                   << PlipUtility::FormatHex((int)m_mbc, 2);
                throw Plip::PlipEmulationException(ex.str().c_str());
        }
    }

    void GameBoyInstance::MbcCycle(PlipMemoryValue lastWrite) {
        switch(m_mbc) {
            case None: break;
            case Mbc1: Mbc1Cycle(lastWrite); break;
            default:
                std::stringstream ex;
                ex << "invalid/unsupported memory bank controller: "
                   << PlipUtility::FormatHex((int)m_mbc, 2);
                throw Plip::PlipEmulationException(ex.str().c_str());
        }
    }

    void GameBoyInstance::Mbc1Cycle(PlipMemoryValue lastWrite) {
        if(lastWrite.address >= 0x8000) return;

        if(lastWrite.address < 0x2000) {
            // RAM enable register.
            if(m_hasRam) {
                m_mbcRamEnabled = (lastWrite.value & 0xF) == 0xA;
                m_cartRam->SetReadable(m_mbcRamEnabled);
                m_cartRam->SetWritable(m_mbcRamEnabled);
            }
        } else if(lastWrite.address < 0x4000) {
            // ROM bank number. If this is set to zero, this will automatically
            // be bumped up by one, regardless of what's in the high bit.
            m_mbcRomBank &= 0b11100000;
            auto newBank = lastWrite.value & 0b11111;
            m_mbcRomBank |= newBank + (newBank == 0 ? 1 : 0);
            m_mbcRomBank &= (m_romBanks - 1);
            m_memory->AssignBlock(m_rom, m_addrBankedRom,
                                  0x4000 * m_mbcRomBank, 0x4000);
        } else if(lastWrite.address < 0x6000) {
            // RAM bank number, or the upper bits of the ROM bank number on
            // 1MB carts.
            if(m_romBanks >= 64) {
                // Selects the upper bits of ROM.
                m_mbcRomBank &= 0b11100000;
                m_mbcRomBank |= (lastWrite.value & 0b11) << 5;
                m_mbcRomBank &= (m_romBanks - 1);
                m_memory->AssignBlock(m_rom, m_addrBankedRom,
                                      0x4000 * m_mbcRomBank, 0x4000);

                // If the advanced ROM banking mode bit is set, this bit impacts
                // bank 0.
                if(m_mbcMode) {
                    m_memory->AssignBlock(m_rom, m_addrRom,
                                          0x2000 * m_mbcRomBank & 0b01100000,
                                          0x4000);
                }
            } else if(m_hasRam && m_mbcRamEnabled && m_mbcMode == 1 && m_cartRamBanks > 1) {
                // Selects the RAM bank.
                m_mbcRamBank = (lastWrite.value & 0b11) & (m_cartRamBanks - 1);
                m_memory->AssignBlock(m_cartRam, m_addrCartRam,
                                      0x2000 * m_mbcRamBank, 0x2000);
            }
        } else {
            // Banking mode select.
            m_mbcMode = lastWrite.value & 0b1;
        }
    }
}
