/* DmaModeVdma.h
 *
 * An enumeration that indicates which state a VDMA transfer is in.
 */

#pragma once

namespace Plip::Core::GameBoy {
    enum class DmaModeVdma {
        Inactive,
        GeneralPurpose,
        HBlank,
    };
}
