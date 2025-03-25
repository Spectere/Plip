/* PPU_Mode.h
 *
 * An enumeration of the Game Boy's video modes.
 */

#pragma once

namespace Plip::Core::GameBoy {
    enum class PPU_Mode {
        HBlank,
        VBlank,
        OamScan,
        Output
    };
}
