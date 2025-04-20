/* AudioNoiseDefinition.h
 *
 * Defines the parameters for the APU noise channel.
 */

#pragma once

namespace Plip::Core::GameBoy {
    struct AudioNoiseDefinition {
        int InitialLengthTimer;
        int InitialVolume;
        int EnvelopeSweepPace;
        int ClockShift;
        bool LfsrWidthWide;
        int ClockDivider;
        bool LengthEnable;
    };
}
