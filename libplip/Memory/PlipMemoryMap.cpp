/* PlipMemoryMap.cpp
 *
 * A flexible memory mapper.
 */

#include <tuple>

#include "PlipMemoryMap.h"

namespace Plip {
    void PlipMemoryMap::AddBlock(PlipMemory *memory, uint32_t offset) {
        AddBlock(memory, offset, memory->GetLength() - offset);
    }

    void PlipMemoryMap::AddBlock(PlipMemory *memory, uint32_t offset, uint32_t length) {
        uint32_t start = 0;

        if(m_rangeList.begin() != m_rangeList.end()) {
            auto last = m_rangeList.back();
            start = last.startAddress + last.length;
        }

        m_rangeList.push_back({
            .startAddress = start,
            .memory = memory,
            .offset = offset,
            .length = length
        });

        UpdateVector();
    }

    void PlipMemoryMap::AssignBlock(PlipMemory *memory, uint32_t address, uint32_t offset) {
        AssignBlock(memory, address, offset, memory->GetLength() - offset);
    }

    void PlipMemoryMap::AssignBlock(PlipMemory *memory, uint32_t address, uint32_t offset, uint32_t length) {
        UnassignBlock(address, length);
        AssignBlockDirect(memory, address, offset, length);
    }

    void PlipMemoryMap::AssignBlockDirect(PlipMemory *memory, uint32_t address, uint32_t offset, uint32_t length) {
        auto it = m_rangeList.begin();
        auto end = m_rangeList.end();

        while(it != end) {
            if(it->startAddress > address) break;
            it++;
        }

        m_rangeList.insert(it, {
                .startAddress = address,
                .memory = memory,
                .offset = offset,
                .length = length
        });

        UpdateVector();
    }

    uint8_t PlipMemoryMap::GetByte(uint32_t address) {
        auto block = FindAddress(address);

        if(block.memory == nullptr) return 0;
        return block.memory->GetByte(block.offset);
    }

    uint32_t PlipMemoryMap::GetLength() {
        return m_range.size();
    }

    PlipMemoryMap::BlockRangeResult PlipMemoryMap::IsBlockInRange(
            const PlipMemoryMapRange &block, uint32_t startAddress, uint32_t endAddress) {
        auto blockEnd = block.startAddress + block.length - 1;

        if(block.startAddress >= startAddress && blockEnd <= endAddress)
            return CompletelyInRange;
        else if(block.startAddress > endAddress || blockEnd < startAddress)
            return NotInRange;

        return PartiallyInRange;
    }

    void PlipMemoryMap::SetByte(uint32_t address, uint8_t value) {
        auto block = FindAddress(address);

        if(block.memory == nullptr) return;
        block.memory->SetByte(block.offset, value);
    }

    void PlipMemoryMap::UnassignBlock(uint32_t address, uint32_t length) {
        /* This function must handle the following cases:
         *
         *     1. Handle empty, unmapped areas of memory.
         *     2. Remove blocks entirely if necessary.
         *     3. Adjust offsets/lengths for clipped blocks.
         *     4. Potentially handle multiple blocks at once.
         *     5. Split blocks in half if necessary.
         */

        auto endAddress = address + length - 1;  // end address

        auto it = m_rangeList.begin();
        auto end = m_rangeList.end();
        auto foundBlock = false;

        while(it != end) {
            switch(IsBlockInRange(*it, address, endAddress)) {
                case NotInRange:
                    if(foundBlock) {
                        // We're now out of range. It's safe to abort.
                        it = end;
                        continue;
                    }

                    // Block is not in range. Ignore.
                    it++;
                    continue;

                case CompletelyInRange:
                    // This block can be removed.
                    foundBlock = true;
                    it = m_rangeList.erase(it);
                    continue;

                case PartiallyInRange:
                    // Remove the portion of the block that overlaps.
                    foundBlock = true;
                    auto blockEnd = it->startAddress + it->length - 1;

                    if(address <= it->startAddress) {
                        // Unassign range overlaps the beginning of the memory
                        // block. Move the starting address and offset forward,
                        // and bump the length back.
                        auto adjust = endAddress - it->startAddress + 1;
                        it->startAddress += adjust;
                        it->offset += adjust;
                        it->length -= adjust;
                    } else if(address > it->startAddress && endAddress >= blockEnd) {
                        // Unassign range overlaps the end of the memory block.
                        // Pull the block length back.
                        it->length -= endAddress - address + 1;
                    } else {
                        // Unassign range falls in the middle of this memory
                        // block. Truncate the block, then push in a new block
                        // with an adjusted offset/length.
                        it->length = address - it->startAddress;

                        auto newStart = endAddress + 1;
                        auto newLength = blockEnd - endAddress;
                        auto offset = endAddress - it->startAddress + 1;
                        AssignBlockDirect(it->memory, newStart, offset, newLength);

                        // There's no way there are any block beyond this, so
                        // break here.
                        it = end;
                        break;
                    }

                    it++;
                    continue;
            }
        }

        UpdateVector();
    }

    void PlipMemoryMap::UpdateVector() {
        m_range.clear();
        for(auto const &range : m_rangeList)
            m_range.push_back(range);
    }
}
