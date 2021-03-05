/* PlipMemory.h
 *
 * An interface representing some type of memory.
 */

#pragma once

#include <cstdint>

namespace Plip {
    class PlipMemory {
    public:
        uint8_t operator[] (uint32_t offset) {
            return GetByte(offset);
        };

        virtual uint8_t GetByte(uint32_t address, bool privileged = false) = 0;
        virtual uint32_t GetLength() = 0;
        [[nodiscard]] virtual bool GetReadable() const = 0;
        [[nodiscard]] virtual uint8_t GetUnprivilegedValue() const = 0;
        [[nodiscard]] virtual bool GetWritable() const = 0;
        virtual void SetByte(uint32_t address, uint8_t value, bool privileged = false) = 0;
        virtual void SetReadable(bool value) = 0;
        virtual void SetUnprivilegedValue(uint8_t value) = 0;
        virtual void SetWritable(bool value) = 0;

    protected:
        PlipMemory() = default;
    };
}
