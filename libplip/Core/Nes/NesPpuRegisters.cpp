/* NesPpuRegisters.cpp
 *
 * Implements the NES's PPU registers.
 */

#include "NesPpuRegisters.h"

using Plip::Core::Nes::NesPpuRegisters;

uint8_t NesPpuRegisters::GetByte(const PpuRegister ppuRegister, [[maybe_unused]] bool privileged) const {
    constexpr uint8_t openBus = 0xFF;  // TODO: I don't think this is accurate on the NES. Correct this later.
    
    switch(ppuRegister) {
        case PpuRegister::PpuStatus: { return m_ppuStatus; }
        case PpuRegister::OamData: { return m_oamData; }
        case PpuRegister::PpuData: { return m_ppuData; }

        case PpuRegister::PpuControl:
        case PpuRegister::PpuMask:
        case PpuRegister::OamAddress:
        case PpuRegister::PpuScroll:
        case PpuRegister::PpuAddress:
        default: {
            return openBus;
        }
    }
}

uint32_t NesPpuRegisters::GetLength() {
    return 0x08;
}

void NesPpuRegisters::SetByte(const PpuRegister ppuRegister, const uint8_t value, [[maybe_unused]] bool privileged) {
    switch(ppuRegister) {
        case PpuRegister::PpuControl: {
            m_ppuControl = value;
            break;
        }

        case PpuRegister::PpuMask: {
            m_ppuMask = value;
            break;
        }

        case PpuRegister::PpuStatus: {
            break;
        }

        case PpuRegister::OamAddress: {
            m_oamAddress = value;
            break;
        }

        case PpuRegister::OamData: {
            m_oamData = value;
            break;
        }

        case PpuRegister::PpuScroll: {
            if(m_ppuScrollWriteY) {
                m_ppuScrollY = value;
                m_ppuScrollWriteY = false;
            } else {
                m_ppuScrollX = value;
                m_ppuScrollWriteY = true;
            }
            break;
        }

        case PpuRegister::PpuAddress: {
            if(m_ppuAddressWriteLsb) {
                m_ppuAddress &= 0xFF00;
                m_ppuAddressWriteLsb |= value;
                m_ppuAddressWriteLsb = false;
            } else {
                m_ppuAddress &= 0x00FF;
                m_ppuAddress |= value << 8;
                m_ppuAddressWriteLsb = true;
            }
            break;
        }

        case PpuRegister::PpuData: {
            m_ppuData = value;
            break;
        }
    }
}
