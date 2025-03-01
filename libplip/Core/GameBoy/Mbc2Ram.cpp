/* Mbc2Ram.cpp
 *
 * Implements MBC2's on-die memory.
 */

#include "Mbc2Ram.h"

using Plip::Core::GameBoy::Mbc2Ram;

uint8_t Mbc2Ram::GetByte(const uint32_t address, const bool privileged) {
    // MBC2's memory are 4-bit nibbles instead of bytes, so only the lower 4-bits
    // should be accurately represented.
    const auto value = PlipMemoryRam::GetByte(address, privileged);
    return (value & 0xF) | (m_unprivilegedValue & 0xF0);
}
