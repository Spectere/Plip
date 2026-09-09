// PPU_Object.h
//
// An OAM object.

#pragma once

#include <cstdint>

namespace Plip::Core::GameBoy {
    struct PPU_Object {
        int Id;
        uint8_t Y;
        uint8_t X;
        uint8_t TileIndex;
        uint8_t Flags;
    };
}
