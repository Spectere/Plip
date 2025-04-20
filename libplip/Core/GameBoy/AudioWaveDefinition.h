/* AudioWaveDefinition.h
 *
 * Defines the parameters for the APU wave channel.
 */

#pragma once

namespace Plip::Core::GameBoy {
    struct AudioWaveDefinition {
        int InitialLengthTimer;
        int OutputVolume;
        int Period;
        bool LengthEnable;
    };
}
