/* NesMapper.h
 *
 * Describes an NES cartridge memory mapper.
 */

#pragma once

#include "Memory/PlipMemory.h"

namespace Plip::Core::Nes {
    class NesMapper {
    public:
        static NesMapper* CreateMapper(bool legacyINes, int mapperId, int submapperId, PlipMemory* ppuRam,
            PlipMemory* prgRom, PlipMemory* chrRom, PlipMemory* trainer, int prgRamSize, int chrRamSize,
            int prgNvramSize, int chrNvramSize);

        [[nodiscard]] uint8_t GetBytePpu(const uint32_t address) const { return GetBytePpu(address, false); }
        [[nodiscard]] virtual uint8_t GetBytePpu(uint32_t address, bool privileged) const = 0;
        void SetBytePpu(const uint32_t address, const uint8_t value) { SetBytePpu(address, value, false); }
        virtual void SetBytePpu(uint32_t address, uint8_t value, bool privileged) = 0;

        [[nodiscard]] uint8_t GetByteSys(const uint32_t address) const { return GetByteSys(address, false); }
        [[nodiscard]] virtual uint8_t GetByteSys(uint32_t address, bool privileged) const = 0;
        void SetByteSys(const uint32_t address, const uint8_t value) { SetByteSys(address, value, false); }
        virtual void SetByteSys(uint32_t address, uint8_t value, bool privileged) = 0;

    protected:
        NesMapper(int mapperId, int submapperId, PlipMemory* ppuRam, PlipMemory* prgRom, PlipMemory* chrRom, PlipMemory* trainer);
        ~NesMapper() = default;

        int m_mapperId {};
        int m_submapperId {};
        PlipMemory* m_prgRom {};
        PlipMemory* m_chrRom {};
        PlipMemory* m_trainer {};
        PlipMemory* m_ppuRam;
    };
}
