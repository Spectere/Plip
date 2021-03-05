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

        uint8_t GetByte(uint32_t address, bool privileged = false) override;
        uint32_t GetLength() override;
        [[nodiscard]] bool GetReadable() const override;
        [[nodiscard]] uint8_t GetUnprivilegedValue() const override;
        [[nodiscard]] bool GetWritable() const override;
        void SetByte(uint32_t address, uint8_t value, bool privileged = false) override;
        void SetReadable(bool value) override;
        void SetUnprivilegedValue(uint8_t value) override;
        void SetWritable(bool value) override;

        uint8_t GetWrittenByte(uint32_t address);

    private:
        uint32_t m_length;
        uint8_t *m_data;
        uint8_t *m_writtenData;

        bool m_readable = true;
        uint8_t m_unprivileged = 0x00;
    };
}
