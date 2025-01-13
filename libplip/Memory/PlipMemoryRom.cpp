/* PlipMemoryRom.cpp
 *
 * A read-only memory implementation.
 */

#include <cstring>

#include "PlipMemoryRom.h"

namespace Plip {
    PlipMemoryRom::PlipMemoryRom(const void *data, const uint32_t length) {
        m_length = length;
        m_data = new uint8_t[m_length];

        std::memcpy(m_data, data, m_length);
    }

    PlipMemoryRom::~PlipMemoryRom() {
        delete m_data;
    }

    uint8_t PlipMemoryRom::GetByte(const uint32_t address) {
        return m_data[address];
    }

    uint32_t PlipMemoryRom::GetLength() {
        return m_length;
    }
}
