/* PlipMemoryRom.h
 *
 * A read-only memory implementation.
 */

#pragma once

#include "PlipMemory.h"

namespace Plip {
    class PlipMemoryRom : public PlipMemory {
    public:
        explicit PlipMemoryRom(void *data, uint32_t length);
        ~PlipMemoryRom();

        uint8_t GetByte(uint32_t address) override;
        uint32_t GetLength() override;
        void SetByte(uint32_t address, uint8_t value) override;

        uint8_t GetWrittenByte(uint32_t address);

    private:
        uint32_t m_length;
        uint8_t *m_data;
        uint8_t *m_writtenData;
    };
}
