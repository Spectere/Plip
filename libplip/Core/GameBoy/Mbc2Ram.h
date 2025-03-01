/* Mbc2Ram.h
 *
 * Implements MBC2's on-die memory.
 */

#pragma once

#include "../../Memory/PlipMemory.h"
#include "../../Memory/PlipMemoryRam.h"

namespace Plip::Core::GameBoy {
    class Mbc2Ram final : public PlipMemoryRam {
    public:
        Mbc2Ram(const uint32_t amount, const uint8_t unprivilegedValue) : PlipMemoryRam(amount, unprivilegedValue) { }

        uint8_t GetByte(uint32_t address, bool privileged = false) override;
    };
}
