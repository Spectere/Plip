/* PlipMemoryRam.h
 *
 * A random access memory implementation.
 */

#pragma once

#include "PlipMemory.h"

namespace Plip {
    class PlipMemoryRam : public PlipMemory {
    public:
        explicit PlipMemoryRam(uint32_t amount, uint8_t unprivilegedValue = 0);
        virtual ~PlipMemoryRam();

        uint8_t GetByte(uint32_t address, bool privileged = false) override;
        uint32_t GetLength() override;
        void SetByte(uint32_t address, uint8_t value, bool privileged = false) override;

    protected:
        uint32_t m_length;
        uint8_t *m_data;
    };
}
