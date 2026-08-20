// PPU_FifoStage.h
//
// The current stage of the FIFO pixel fetcher.

#pragma once

namespace Plip::Core::GameBoy {
    enum class PPU_FifoStage {
        GetTileT1,          // Delay
        GetTileT2,          // Fetch
        GetTileDataLowT1,   // Delay
        GetTileDataLowT2,   // Fetch
        GetTileDataHighT1,  // Delay
        GetTileDataHighT2,  // Fetch
        Push,
    };
}
