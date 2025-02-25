/* PlipMemoryRam.cpp
 *
 * A random access memory implementation.
 */

#include "PlipMemoryRam.h"

using Plip::PlipMemoryRam;

PlipMemoryRam::PlipMemoryRam(const uint32_t amount, const uint8_t unprivilegedValue) {
    m_data = new uint8_t[amount] {};
    m_length = amount;

    m_unprivilegedValue = unprivilegedValue;
}

PlipMemoryRam::~PlipMemoryRam() {
    delete m_data;
}

uint8_t PlipMemoryRam::GetByte(const uint32_t address, const bool privileged) {
    if(m_readable || privileged) {
        return m_data[address];
    }

    return m_unprivilegedValue;
}

uint32_t PlipMemoryRam::GetLength() {
    return m_length;
}

void PlipMemoryRam::SetByte(const uint32_t address, const uint8_t value, const bool privileged) {
    if(m_writable || privileged) {
        m_data[address] = value;
    }
}
