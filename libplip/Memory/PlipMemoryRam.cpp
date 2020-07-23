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
        return m_data[address];
    }

    uint32_t PlipMemoryRam::GetLength() {
        return m_length;
    }

    void PlipMemoryRam::SetByte(uint32_t address, uint8_t value) {
        m_data[address] = value;
    }
}
