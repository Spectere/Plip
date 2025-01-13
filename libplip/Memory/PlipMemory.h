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

        virtual uint8_t GetByte(uint32_t address) = 0;
        virtual uint32_t GetLength() = 0;
        virtual void SetByte(uint32_t address, uint8_t value) = 0;

    protected:
        PlipMemory() = default;
        ~PlipMemory() = default;
    };
}
