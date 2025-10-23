/* NesPpuRegisters.cpp
 *
 * Implements the NES's PPU registers.
 */

#include "NesPpuRegisters.h"

using Plip::Core::Nes::NesPpuRegisters;

uint8_t NesPpuRegisters::GetByte(const PpuRegister ppuRegister, [[maybe_unused]] bool privileged) {
    constexpr uint8_t openBus = 0xFF;  // TODO: I don't think this is accurate on the NES. Correct this later.
    
    switch(ppuRegister) {
        case PpuRegister::PpuStatus: {
            const uint8_t returnValue =
                   (m_ppuStatusVBlank         ? 0b10000000 : 0)
                 | (m_ppuStatusSprite0Hit     ? 0b01000000 : 0)
                 | (m_ppuStatusSpriteOverflow ? 0b00100000 : 0)
                 | (openBus                   & 0b00011111);
            
            m_ppuRegWriteLatch = false;  // Reading PPUSTATUS clears the w register...
            m_ppuStatusVBlank = false;    // ...and VBlank.
            return returnValue;
        }

        case PpuRegister::OamData: {
            return GetByteOam(m_oamAddress);
        }
        
        case PpuRegister::PpuData: {
            // TODO: Emulate glitchy behavior when reading from PPUDATA during rendering.
            if(m_ppuAddress >= 0x3F00 && m_ppuAddress < 0x4000) {
                return m_palette[(m_ppuAddress - 0x3F00) % 0x20];
            }

            const uint8_t returnValue = m_ppuDataBuffer;
            m_ppuDataBuffer = m_mapper->GetBytePpu(m_ppuAddress);
            IncrementPpuAddress();
            return returnValue;
        }

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

uint8_t NesPpuRegisters::GetByteOam(const uint8_t address) const {
    return m_oam[address];
}

uint32_t NesPpuRegisters::GetLength() {
    return 0x08;
}

void NesPpuRegisters::IncrementPpuAddress() {
    m_ppuAddress += (m_ppuCtrlVramIncrementVertical ? 32 : 1);
    m_ppuAddress &= 0x3FFF;
}

void NesPpuRegisters::Reset() {
    // PPUCTRL
    m_ppuCtrlBaseNamespaceAddress = 0;
    m_ppuCtrlVramIncrementVertical = false;
    m_ppuCtrlSpriteTableAddressHigh = false;
    m_ppuCtrlBackgroundAddressHigh = false;
    m_ppuCtrlTallSprites = false;
    m_ppuCtrlOutputToExt = false;
    m_ppuCtrlNmiEnabled = false;

    // PPUMASK
    m_ppuMaskGreyscale = false;
    m_ppuMaskDisableBackgroundMask = false;
    m_ppuMaskDisableSpriteMask = false;
    m_ppuMaskRenderBackground = false;
    m_ppuMaskRenderSprites = false;
    m_ppuMaskEmphasizeRed = false;
    m_ppuMaskEmphasizeGreen = false;
    m_ppuMaskEmphasizeBlue = false;

    // PPUSTATUS
    m_ppuStatusSpriteOverflow = true;
    m_ppuStatusSprite0Hit = false;
    m_ppuStatusVBlank = true;

    // et al
    m_ppuScrollX = m_ppuScrollY = 0;
    
    m_ppuRegWriteLatch = false;

    m_oddFrame = false;
}

void NesPpuRegisters::SetByte(const PpuRegister ppuRegister, const uint8_t value, [[maybe_unused]] bool privileged) {
    switch(ppuRegister) {
        case PpuRegister::PpuControl: {
            m_ppuDebugCtrl = value;
            m_ppuCtrlBaseNamespaceAddress = value & 0b11;
            m_ppuCtrlVramIncrementVertical = value & (1 << 2);
            m_ppuCtrlSpriteTableAddressHigh = value & (1 << 3);
            m_ppuCtrlBackgroundAddressHigh = value & (1 << 4);
            m_ppuCtrlTallSprites = value & (1 << 5);
            m_ppuCtrlOutputToExt = value & (1 << 6);
            m_ppuCtrlNmiEnabled = value & (1 << 7);

            // If NMIs are enabled and the VBlank flag is set in PPUSTATUS, immediately raise an NMI.
            if(m_ppuStatusVBlank) {
                m_cpu->FlagNmi();
            }
            
            break;
        }

        case PpuRegister::PpuMask: {
            m_ppuDebugMask = value;
            m_ppuMaskGreyscale = value & 1;
            m_ppuMaskDisableBackgroundMask = value & (1 << 1);
            m_ppuMaskDisableSpriteMask = value & (1 << 2);
            m_ppuMaskRenderBackground = value & (1 << 3);
            m_ppuMaskRenderSprites = value & (1 << 4);
            m_ppuMaskEmphasizeRed = value & (1 << 5);
            m_ppuMaskEmphasizeGreen = value & (1 << 6);
            m_ppuMaskEmphasizeBlue = value & (1 << 7);
            break;
        }

        case PpuRegister::PpuStatus: {
            // Read-only
            break;
        }

        case PpuRegister::OamAddress: {
            // TODO: Emulate 2C02G OAM corruption.
            m_oamAddress = value;
            break;
        }

        case PpuRegister::OamData: {
            SetByteOam(m_oamAddress++, value);
            break;
        }

        case PpuRegister::PpuScroll: {
            if(m_ppuRegWriteLatch) {
                m_ppuScrollY = value;
            } else {
                m_ppuScrollX = value;
            }
            m_ppuRegWriteLatch = !m_ppuRegWriteLatch;
            break;
        }

        case PpuRegister::PpuAddress: {
            if(m_ppuRegWriteLatch) {
                m_ppuAddress &= 0xFF00;
                m_ppuAddress |= value;
            } else {
                m_ppuAddress &= 0x00FF;
                m_ppuAddress |= value << 8;
            }
            m_ppuRegWriteLatch = !m_ppuRegWriteLatch;
            break;
        }

        case PpuRegister::PpuData: {
            // TODO: Emulate glitchy behavior when writing to PPUDATA during rendering.
            if(m_ppuAddress >= 0x3F00 && m_ppuAddress < 0x4000) {
                m_palette[(m_ppuAddress - 0x3F00) % 0x20] = value;
            } else {
                m_mapper->SetBytePpu(m_ppuAddress, value);
            }
            IncrementPpuAddress();
            break;
        }
    }
}

void NesPpuRegisters::SetByteOam(const uint8_t address, const uint8_t value) {
    m_oam[address] = value;
}

std::map<std::string, Plip::DebugValue> NesPpuRegisters::GetDebugInfo() const {
    return {
        { "PPUCTRL", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ppuDebugCtrl)) },
        { "PPUMASK", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ppuDebugMask)) },
        { "PPUADDR", DebugValue(DebugValueType::Int16Le, static_cast<uint64_t>(m_ppuAddress)) },
    };
}

