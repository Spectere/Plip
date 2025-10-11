/* NesTiming.h
 *
 * Describes the system that a game should run on.
 */

#pragma once

namespace Plip::Core::Nes {
    enum class NesTiming {
        Ntsc = 0b00,
        Pal = 0b01,
        MultiRegion = 0b10,
        Dendy = 0b11,
    };
}
