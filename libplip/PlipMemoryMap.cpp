/* PlipMemoryMap.cpp
 *
 * A flexible memory mapper.
 */

#include <tuple>

#include "PlipMemoryMap.h"

namespace Plip {
    void PlipMemoryMap::AddBlock(PlipMemory *memory, uint32_t offset, uint32_t length) {
        uint32_t start = 0;

        if(m_range.begin() != m_range.end()) {
            auto last = m_range.back();
            start = last.startAddress + last.length;
        }

        m_range.push_back({
            .startAddress = start,
            .memory = memory,
            .offset = offset,
            .length = length
        });
    }

    std::tuple<PlipMemory*, uint32_t> PlipMemoryMap::FindAddress(uint32_t address) {
        for(auto const &memory : m_range) {
            if(address < memory.startAddress || address > memory.startAddress + memory.length)
                continue;

            return { memory.memory, address - memory.startAddress };
        }

        return { nullptr, 0 };
    }

    uint8_t PlipMemoryMap::GetByte(uint32_t address) {
        auto [ memory, offset ] = FindAddress(address);

        if(memory == nullptr) return 0;
        return memory->GetByte(offset);
    }

    uint32_t PlipMemoryMap::GetLength() {
        if(m_range.begin() == m_range.end()) return 0;

        auto last = m_range.back();
        return last.startAddress + last.length;
    }

    void PlipMemoryMap::SetByte(uint32_t address, uint8_t value) {
        auto [ memory, offset ] = FindAddress(address);

        if(memory == nullptr) return;
        memory->SetByte(offset, value);
    }
}
