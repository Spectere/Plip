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
        virtual ~PlipMemoryMap() = default;

        void AddBlock(PlipMemory *memory, uint32_t offset = 0);
        void AddBlock(PlipMemory *memory, uint32_t offset, uint32_t length);
        void AssignBlock(PlipMemory *memory, const uint32_t address) { AssignBlock(memory, address, 0); }
        virtual void AssignBlock(PlipMemory *memory, uint32_t address, uint32_t offset);
        virtual void AssignBlock(PlipMemory *memory, uint32_t address, uint32_t offset, uint32_t length);
        [[nodiscard]] uint8_t GetByte(const uint32_t address) { return GetByte(address, false); }
        [[nodiscard]] virtual uint8_t GetByte(uint32_t address, bool privileged) const;
        virtual uint32_t GetLength();
        void SetByte(const uint32_t address, const uint8_t value) { SetByte(address, value, false); }
        virtual void SetByte(uint32_t address, uint8_t value, bool privileged);
        void SetInvalidByte(uint8_t value);
        virtual void UnassignAllBlocks();
        virtual void UnassignBlock(uint32_t address, uint32_t length);

        uint32_t LastWrittenAddress = 0;
        uint8_t LastWrittenValue = 0;

    private:
        enum BlockRangeResult {
            NotInRange,
            PartiallyInRange,
            CompletelyInRange
        };

        void AssignBlockDirect(PlipMemory *memory, uint32_t address, uint32_t offset, uint32_t length);
        [[nodiscard]] std::tuple<PlipMemory*, uint32_t> FindAddress(uint32_t address) const;
        static inline BlockRangeResult IsBlockInRange(const PlipMemoryMapRange &block, uint32_t startAddress, uint32_t endAddress);

        std::list<PlipMemoryMapRange> m_range;
        uint8_t m_invalidByte = 0;
    };
}
