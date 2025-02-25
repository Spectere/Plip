/* PlipMemoryRom.h
 *
 * A read-only memory implementation.
 */

#pragma once

#include "PlipMemory.h"

namespace Plip {
    class PlipMemoryRom final : public PlipMemory {
    public:
        explicit PlipMemoryRom(const void *data, uint32_t length, uint8_t unprivilegedValue = 0);
        ~PlipMemoryRom();

        uint8_t GetByte(uint32_t address, bool privileged = false) override;
        uint32_t GetLength() override;
        void SetByte(uint32_t address, uint8_t value, bool privileged = false) override {}

    private:
        uint32_t m_length;
        uint8_t *m_data;
    };
}
