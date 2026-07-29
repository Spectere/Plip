/* DmaState.h
 *
 * An enumeration that indicates which state a DMA transfer is in.
 */

#pragma once

namespace Plip::Core::GameBoy {
    enum class DmaStateOam {
        Inactive,
        Preparing,
        Transferring,
    };
}
