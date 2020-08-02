/* PlipMemoryRom.cpp
 *
 * A read-only memory implementation.
 */

#include <cstring>

#include "PlipMemoryRom.h"

namespace Plip {
    PlipMemoryRom::PlipMemoryRom(void *data, uint32_t length) {
        m_length = length;
        m_data = new uint8_t[m_length];

        std::memcpy(m_data, data, m_length);
        m_writtenData = new uint8_t[m_length];
    }

    PlipMemoryRom::~PlipMemoryRom() {
        delete m_data;
        delete m_writtenData;
    }

    uint8_t PlipMemoryRom::GetByte(uint32_t address) {
        return m_data[address];
    }

    uint8_t PlipMemoryRom::GetWrittenByte(uint32_t address) {
        return m_writtenData[address];
    }

    uint32_t PlipMemoryRom::GetLength() {
        return m_length;
    }

    void PlipMemoryRom::SetByte(uint32_t address, uint8_t value) {
        m_writtenData[address] = value;
    }
}
