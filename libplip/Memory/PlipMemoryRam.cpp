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

    uint8_t PlipMemoryRam::GetByte(uint32_t address, bool privileged) {
        return m_readable || privileged ? m_data[address] : m_unprivileged;
    }

    uint32_t PlipMemoryRam::GetLength() {
        return m_length;
    }

    bool PlipMemoryRam::GetReadable() const {
        return m_readable;
    }

    uint8_t PlipMemoryRam::GetUnprivilegedValue() const {
        return m_unprivileged;
    }

    bool PlipMemoryRam::GetWritable() const {
        return m_writable;
    }

    void PlipMemoryRam::SetByte(uint32_t address, uint8_t value, bool privileged) {
        if(m_writable || privileged) m_data[address] = value;
    }

    void PlipMemoryRam::SetReadable(bool value) {
        m_readable = value;
    }

    void PlipMemoryRam::SetUnprivilegedValue(uint8_t value) {
        m_unprivileged = value;
    }

    void PlipMemoryRam::SetWritable(bool value) {
        m_writable = value;
    }
}
