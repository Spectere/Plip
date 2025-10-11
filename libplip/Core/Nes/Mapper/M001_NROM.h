/* M001_NROM.h
 *
 * Implements an NROM "mapper."
 */

#pragma once

#include "../NesMapper.h"
#include "../../../Memory/PlipMemoryRam.h"

namespace Plip::Core::Nes::Mapper {
    class Mapper001_NROM final : public NesMapper {
    public:
        Mapper001_NROM(bool legacyINes, int mapperId, int submapperId,
            PlipMemory* prgRom, PlipMemory* chrRom, PlipMemory* trainer,
            int prgRamSize, int chrRamSize, int prgNvramSize, int chrNvramSize);
        
        [[nodiscard]] uint8_t GetByteChr(uint32_t address, bool privileged) const override;
        void SetByteChr(uint32_t address, uint8_t value, bool privileged) override;

        [[nodiscard]] uint8_t GetByteSys(uint32_t address, bool privileged) const override;
        void SetByteSys(uint32_t address, uint8_t value, bool privileged) override;

    private:
        int m_prgRamSize {};
        PlipMemoryRam* m_prgRam {};
    };
}
