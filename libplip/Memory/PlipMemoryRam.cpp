/* PlipMemoryRam.cpp
 *
 * A random access memory implementation.
 */

#include "PlipMemoryRam.h"

using Plip::PlipMemoryRam;

PlipMemoryRam::PlipMemoryRam(const uint32_t amount) {
    m_data = new uint8_t[amount] {};
    m_length = amount;
}

PlipMemoryRam::~PlipMemoryRam() {
    delete m_data;
}

uint8_t PlipMemoryRam::GetByte(const uint32_t address, const bool privileged) {
    if(m_readable || privileged) {
        return m_data[address];
    }

    return 0x00;
}

uint32_t PlipMemoryRam::GetLength() {
    return m_length;
}

void PlipMemoryRam::SetByte(const uint32_t address, const uint8_t value, const bool privileged) {
    if(m_writable || privileged) {
        m_data[address] = value;
    }
}
