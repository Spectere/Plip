/* NesMapper.cpp
 *
 * Describes an NES cartridge memory mapper.
 */

#include "NesMapper.h"

#include "../../PlipInitializationException.h"
#include "Mapper/M001_NROM.h"

namespace Plip::Core::Nes {
    NesMapper::NesMapper(int mapperId, int submapperId, PlipMemory* prgRom, PlipMemory* chrRom, PlipMemory* trainer) :
    m_mapperId(mapperId), m_submapperId(submapperId), m_prgRom(prgRom), m_chrRom(chrRom), m_trainer(trainer) {}
    
    NesMapper* NesMapper::CreateMapper(bool legacyINes, int mapperId, int submapperId, PlipMemory* prgRom, PlipMemory* chrRom, PlipMemory* trainer, int prgRamSize, int chrRamSize, int prgNvramSize, int chrNvramSize) {
        switch(mapperId) { // NOLINT(*-multiway-paths-covered)
            case 0: {
                return new Mapper::Mapper001_NROM(legacyINes, mapperId, submapperId, prgRom, chrRom, trainer, prgRamSize, chrRamSize, prgNvramSize, chrNvramSize);
            }
            default: {
                throw PlipInitializationException("Unsupported mapper.");
            }
        }
    }

}
