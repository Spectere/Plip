/* PlipMemory.h
 *
 * An interface representing some type of memory.
 */

#pragma once

#include <cstdint>

namespace Plip {
    class PlipMemory {
    public:
        uint8_t operator[] (const uint32_t offset) {
            return GetByte(offset, false);
        }

        [[nodiscard]] uint8_t GetByte(const uint32_t address) { return GetByte(address, false); }
        virtual uint8_t GetByte(uint32_t address, bool privileged) = 0;
        virtual uint32_t GetLength() = 0;
        [[nodiscard]] virtual bool GetReadable() const { return m_readable; }
        [[nodiscard]] virtual bool GetWritable() const { return m_writable; }
        void SetByte(const uint32_t address, const uint8_t value) { SetByte(address, value, false); }
        virtual void SetByte(uint32_t address, uint8_t value, bool privileged) = 0;
        virtual void SetReadable(const bool readable) { m_readable = readable; }
        void SetUnprivilegedReadValue(const uint8_t value) { m_unprivilegedValue = value; }
        virtual void SetWritable(const bool writable) { m_writable = writable; }

    protected:
        PlipMemory() = default;
        ~PlipMemory() = default;

        bool m_readable = true;
        bool m_writable = true;
        uint8_t m_unprivilegedValue {};
    };
}
