/* PlipMemoryRam.h
 *
 * A random access memory implementation.
 */

#pragma once

#include "PlipMemory.h"

namespace Plip {
    class PlipMemoryRam final : public PlipMemory {
    public:
        explicit PlipMemoryRam(uint32_t amount);
        ~PlipMemoryRam();

        uint8_t GetByte(uint32_t address, bool privileged) override;
        uint32_t GetLength() override;
        void SetByte(uint32_t address, uint8_t value, bool privileged) override;

    private:
        uint32_t m_length;
        uint8_t *m_data;
    };
}
