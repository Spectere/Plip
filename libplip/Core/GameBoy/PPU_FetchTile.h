// PPU_FetchTile.h
//
// TODO: Add description here.

#pragma once

#include <cstdint>

namespace Plip::Core::GameBoy {
    struct PPU_FetchTile {
        uint16_t TileAddress {};
        uint8_t TileDataLow {};
        uint8_t TileDataHigh {};
    };
}
