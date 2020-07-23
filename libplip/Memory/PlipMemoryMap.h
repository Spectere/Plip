/* PlipMemoryMap.h
 *
 * A flexible memory mapper.
 */

#pragma once

#include <cstdint>
#include <list>

#include "PlipMemory.h"

namespace Plip {
    struct PlipMemoryMapRange {
        uint32_t startAddress;
        PlipMemory *memory;
        uint32_t offset;
        uint32_t length;
    };

    class PlipMemoryMap {
    public:
        void AddBlock(PlipMemory *memory, uint32_t offset = 0);
        void AddBlock(PlipMemory *memory, uint32_t offset, uint32_t length);
        void AssignBlock(PlipMemory *memory, uint32_t address, uint32_t offset, uint32_t length);
        uint8_t GetByte(uint32_t address);
        uint32_t GetLength();
        void SetByte(uint32_t address, uint8_t value);
        void UnassignBlock(uint32_t address, uint32_t length);

    private:
        enum BlockRangeResult {
            NotInRange,
            PartiallyInRange,
            CompletelyInRange
        };

        void AssignBlockDirect(PlipMemory *memory, uint32_t address, uint32_t offset, uint32_t length);
        std::tuple<PlipMemory*, uint32_t> FindAddress(uint32_t address);
        static inline BlockRangeResult IsBlockInRange(const PlipMemoryMapRange &block, uint32_t startAddress, uint32_t endAddress);

        std::list<PlipMemoryMapRange> m_range;
    };
}
