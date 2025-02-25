/* PlipMemoryRom.cpp
 *
 * A read-only memory implementation.
 */

#include <cstring>

#include "PlipMemoryRom.h"

using Plip::PlipMemoryRom;

PlipMemoryRom::PlipMemoryRom(const void *data, const uint32_t length, const uint8_t unprivilegedValue) {
    m_length = length;
    m_data = new uint8_t[m_length];

    std::memcpy(m_data, data, m_length);

    m_unprivilegedValue = unprivilegedValue;
}

PlipMemoryRom::~PlipMemoryRom() {
    delete m_data;
}

uint8_t PlipMemoryRom::GetByte(const uint32_t address, const bool privileged) {
    if(m_readable || privileged) {
        return m_data[address];
    }

    return m_unprivilegedValue;
}

uint32_t PlipMemoryRom::GetLength() {
    return m_length;
}
