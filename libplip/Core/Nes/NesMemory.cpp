/* NesMemory.cpp
 *
 * Manages the NES's memory layout.
 */

#include "NesMemory.h"

using Plip::Core::Nes::NesMemory;

NesMemory::NesMemory(PlipMemory* workRam, NesPpuRegisters* ppuRegisters, NesApuRegisters* apuRegisters, NesMapper* mapper, PlipMemory* ppuRam)
    : m_workRam(workRam), m_ppuRegisters(ppuRegisters), m_apuRegisters(apuRegisters), m_mapper(mapper), m_ppuRam(ppuRam) { }

uint8_t NesMemory::GetByte(uint32_t address, const bool privileged) const {
    if(address < 0x2000) {
        return m_workRam->GetByte(address % WorkRamLength, privileged);
    }

    if(address >= 0x2000 && address < 0x4000) {
        address -= 0x2000;
        return m_ppuRegisters->GetByte(address % 8, privileged);
    }

    if(address >= 0x4000 && address < 0x4020) {
        address -= 0x4000;
        return m_apuRegisters->GetByte(address, privileged);
    }

    return m_mapper->GetByteSys(address, privileged);
}

void NesMemory::SetByte(uint32_t address, uint8_t value, bool privileged) {
    if(address < 0x2000) {
        m_workRam->SetByte(address % WorkRamLength, value, privileged);
        return;
    }

    if(address >= 0x2000 && address < 0x4000) {
        address -= 0x2000;
        m_ppuRegisters->SetByte(address % 8, value, privileged);
        return;
    }

    if(address >= 0x4000 && address < 0x4020) {
        address -= 0x4000;
        m_apuRegisters->SetByte(address, value, privileged);
        return;
    }

    return m_mapper->SetByteSys(address, value, privileged);
}
