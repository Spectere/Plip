/* NesMapper.h
 *
 * Describes an NES cartridge memory mapper.
 */

#pragma once

#include "../../Memory/PlipMemory.h"

namespace Plip::Core::Nes {
    class NesMapper {
    public:
        static NesMapper* CreateMapper(bool legacyINes, int mapperId, int submapperId, PlipMemory* prgRom, PlipMemory* chrRom,
            PlipMemory* trainer, int prgRamSize, int chrRamSize, int prgNvramSize, int chrNvramSize);
        
        [[nodiscard]] virtual uint8_t GetByteChr(uint32_t address, bool privileged) const = 0;
        virtual void SetByteChr(uint32_t address, uint8_t value, bool privileged) = 0;

        [[nodiscard]] virtual uint8_t GetByteSys(uint32_t address, bool privileged) const = 0;
        virtual void SetByteSys(uint32_t address, uint8_t value, bool privileged) = 0;

    protected:
        NesMapper(int mapperId, int submapperId, PlipMemory* prgRom, PlipMemory* chrRom, PlipMemory* trainer);
        ~NesMapper() = default;

        int m_mapperId {};
        int m_submapperId {};
        PlipMemory* m_prgRom {};
        PlipMemory* m_chrRom {};
        PlipMemory* m_trainer {};
    };
}
