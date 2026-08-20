// PPU_FifoPixel.h
//
// TODO: Add description here.

#pragma once

#include <cstdint>

namespace Plip::Core::GameBoy {
    struct PPU_FifoPixel {
        uint8_t Color {};
        uint8_t Palette {};      // DMG: OBJ only; CGB: OBJ & BG
        uint8_t ObjPriority {};  // CGB
        bool BgPriority {};
    };
}
