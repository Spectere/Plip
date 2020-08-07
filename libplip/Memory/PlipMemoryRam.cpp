/* PlipMemoryRam.cpp
 *
 * A random access memory implementation.
 */

#include "PlipMemoryRam.h"

namespace Plip {
    PlipMemoryRam::PlipMemoryRam(uint32_t amount) {
        m_data = new uint8_t[amount] {};
        m_length = amount;
    }

    PlipMemoryRam::~PlipMemoryRam() {
        delete m_data;
    }

    uint8_t PlipMemoryRam::GetByte(uint32_t address) {
        return m_readable ? m_data[address] : 0;
    }

    uint8_t PlipMemoryRam::GetBytePrivileged(uint32_t address) {
        return m_data[address];
    }

    uint32_t PlipMemoryRam::GetLength() {
        return m_length;
    }

    bool PlipMemoryRam::GetReadable(bool value) const {
        return m_readable;
    }

    bool PlipMemoryRam::GetWritable(bool value) const {
        return m_writable;
    }

    void PlipMemoryRam::SetByte(uint32_t address, uint8_t value) {
        if(m_writable) m_data[address] = value;
    }

    void PlipMemoryRam::SetBytePrivileged(uint32_t address, uint8_t value) {
        m_data[address] = value;
    }

    void PlipMemoryRam::SetReadable(bool value) {
        m_readable = value;
    }

    void PlipMemoryRam::SetWritable(bool value) {
        m_writable = value;
    }
}
