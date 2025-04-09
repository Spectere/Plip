/* PPU_OutputStage.h
*
 * An enumeration of the Game Boy's pixel drawing stages.
 */

#pragma once

namespace Plip::Core::GameBoy {
    enum class PPU_OutputStage {
        BackgroundScrolling,
        Drawing,
        WindowPreparation,
        ObjectPenalty,
    };
}
