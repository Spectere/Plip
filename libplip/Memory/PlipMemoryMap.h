/* PlipMemoryMap.h
 *
 * A flexible memory mapper.
 */

#pragma once

#include <cstdint>
#include <list>
#include <vector>

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
        uint8_t operator[] (uint32_t offset) {
            return GetByte(offset);
        };

        void AddBlock(PlipMemory *memory, uint32_t offset = 0);
        void AddBlock(PlipMemory *memory, uint32_t offset, uint32_t length);
        void AssignBlock(PlipMemory *memory, uint32_t address, uint32_t offset = 0);
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

        struct FindResults {
            PlipMemory* memory;
            uint32_t offset;
        };

        inline FindResults FindAddress(uint32_t address) {
            for(auto i = 0; i < m_rangeCount; i++) {
                auto range = m_range[i];
                if(address < range.startAddress || address > range.startAddress + range.length - 1)
                    continue;

                return { range.memory, address - range.startAddress + range.offset };
            }

            return { nullptr, 0 };
        };

        void AssignBlockDirect(PlipMemory *memory, uint32_t address, uint32_t offset, uint32_t length);
        static inline BlockRangeResult IsBlockInRange(const PlipMemoryMapRange &block, uint32_t startAddress, uint32_t endAddress);
        void UpdateVector();

        std::list<PlipMemoryMapRange> m_rangeList;
        std::vector<PlipMemoryMapRange> m_range;
        int m_rangeCount;
    };
}
