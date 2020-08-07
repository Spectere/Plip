/* PlipMemoryRam.h
 *
 * A random access memory implementation.
 */

#pragma once

#include "PlipMemory.h"

namespace Plip {
    class PlipMemoryRam : public PlipMemory {
    public:
        explicit PlipMemoryRam(uint32_t amount);
        ~PlipMemoryRam();

        uint8_t GetByte(uint32_t address) override;
        uint8_t GetBytePrivileged(uint32_t address);
        uint32_t GetLength() override;
        [[nodiscard]] bool GetReadable(bool value) const;
        [[nodiscard]] bool GetWritable(bool value) const;
        void SetByte(uint32_t address, uint8_t value) override;
        void SetBytePrivileged(uint32_t address, uint8_t value);
        void SetReadable(bool value);
        void SetWritable(bool value);

    private:
        uint32_t m_length;
        uint8_t *m_data;

        bool m_readable = true;
        bool m_writable = true;
    };
}
