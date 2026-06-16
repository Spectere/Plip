/* PlipMemoryRom.cpp
 *
 * A read-only memory implementation.
 */

#include <cstring>

#include "PlipMemoryRom.h"

using Plip::PlipMemoryRom;

PlipMemoryRom::PlipMemoryRom(const void *data, const uint32_t length, const uint8_t unprivilegedValue) {
    m_length = length;
    m_data = std::make_unique<uint8_t[]>(length);

    std::memcpy(m_data.get(), data, m_length);

    m_unprivilegedValue = unprivilegedValue;
}

uint8_t PlipMemoryRom::GetByte(const uint32_t address, const bool privileged) {
    if(m_readable || privileged) {
        return m_data[address % m_length];
    }

    return m_unprivilegedValue;
}

uint32_t PlipMemoryRom::GetLength() {
    return m_length;
}
