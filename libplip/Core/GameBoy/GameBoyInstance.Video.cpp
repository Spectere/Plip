/* GameBoyInstance.Video.cpp
 *
 * Handles functions related to emulating the Game Boy's PPU.
 */

#include <algorithm>
#include <cassert>
#include <cstring>

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::PPU_Cycle() {
    // TODO: Clock the PPU every other cycle when running in double speed mode.
}

std::map<std::string, Plip::DebugValue> GameBoyInstance::PPU_GetDebugInfo() const {
    return {
        { "LCDC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::LcdControl))) },
        { "LY", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::LcdYCoordinate))) },
        { "LYC", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::LcdYCompare))) },
        { "STAT", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::LcdStatus))) },
        { "SCX", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::ScrollX))) },
        { "SCY", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::ScrollY))) },
        { "WX", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::WindowX))) },
        { "WY", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::WindowY))) },
        { "BGP", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ioRegisters->GetByte(IoRegister::BgPalette))) },
        { "Clock", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ppuDotClock)) },
        { "Mode", DebugValue(DebugValueType::Int8, static_cast<uint64_t>(m_ppuMode)) },
    };
}

void GameBoyInstance::PPU_Plot_CGB(const bool objPalette, const int palette, const int color, const int pos) const {
    assert(pos < (ScreenWidth * ScreenHeight));

    if(m_ppuLcdOff) {
        m_videoFormat.plot(m_videoBuffer, pos, 0, 0, 0);
        return;
    }

    const auto paletteRam = objPalette ? m_ppuCgbObjPaletteRam : m_ppuCgbBgPaletteRam;
    const auto index = (((palette & 0b111) * 4) + (color & 0b11)) * 2;
    const uint16_t colorDefinition = (paletteRam->GetByte(index + 1, true) << 8)
                                   | paletteRam->GetByte(index, true);
    const uint8_t red = PPU_ScaleColorChannel(colorDefinition & 0b11111);
    const uint8_t green = PPU_ScaleColorChannel((colorDefinition >> 5) & 0b11111);
    const uint8_t blue = PPU_ScaleColorChannel((colorDefinition >> 10) & 0b11111);
    m_videoFormat.plot(m_videoBuffer, pos, red, green, blue);
}

void GameBoyInstance::PPU_Plot_DMG(int color, const int pos) const {
    assert(pos < (ScreenWidth * ScreenHeight));
    assert(color < 0b100);

    if(m_ppuLcdOff) color = 0b00;

    switch(color) {
        case 0b00:  // White
            m_videoFormat.plot(m_videoBuffer, pos, 0xFF, 0xFF, 0xFF);
            break;

        case 0b01:  // Light Grey
            m_videoFormat.plot(m_videoBuffer, pos, 0xAA, 0xAA, 0xAA);
            break;

        case 0b10:  // Dark Gray
            m_videoFormat.plot(m_videoBuffer, pos, 0x55, 0x55, 0x55);
            break;

        case 0b11:  // Black
            m_videoFormat.plot(m_videoBuffer, pos, 0x00, 0x00, 0x00);
            break;

        default:  // wtf? Red, I guess?
            m_videoFormat.plot(m_videoBuffer, pos, 0xFF, 0x00, 0x00);
            break;
    }
}

void GameBoyInstance::PPU_Reset() {
    // Paint the framebuffer white.
    memset(m_videoBuffer, 0xFF, m_videoBufferSize);

    // Reset PPU variables.
    m_ppuDotClock = 0;
    m_ppuLcdOff = false;
    m_ppuOamAccess = true;
    m_ppuVramAccess = true;

#ifndef NDEBUG
    m_DBG_ppuFrameDotClocks = 0;
#endif // NDEBUG
}

void GameBoyInstance::PPU_SetMemoryPermissions() {
    switch(m_ppuMode) {
        case PPU_Mode::HBlank:
        case PPU_Mode::VBlank: {
            m_ppuOamAccess = true;
            m_ppuVramAccess = true;
            UpdateSharedPermissions();

            m_ppuCgbBgPaletteRam->SetReadable(true);
            m_ppuCgbBgPaletteRam->SetWritable(true);
            m_ppuCgbObjPaletteRam->SetReadable(true);
            m_ppuCgbObjPaletteRam->SetWritable(true);
            break;
        }
        case PPU_Mode::OamScan: {
            m_ppuOamAccess = false;
            m_ppuVramAccess = true;
            UpdateSharedPermissions();

            m_ppuCgbBgPaletteRam->SetReadable(true);
            m_ppuCgbBgPaletteRam->SetWritable(true);
            m_ppuCgbObjPaletteRam->SetReadable(true);
            m_ppuCgbObjPaletteRam->SetWritable(true);
            break;
        }
        case PPU_Mode::Output: {
            m_ppuOamAccess = false;
            m_ppuVramAccess = false;
            UpdateSharedPermissions();

            m_ppuCgbBgPaletteRam->SetReadable(false);
            m_ppuCgbBgPaletteRam->SetWritable(false);
            m_ppuCgbObjPaletteRam->SetReadable(false);
            m_ppuCgbObjPaletteRam->SetWritable(false);
            break;
        }
    }
}
