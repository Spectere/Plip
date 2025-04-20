/* AudioPulseDefinition.h
 *
 * Defines the parameters for the APU pulse channels.
 */

#pragma once

namespace Plip::Core::GameBoy {
    struct AudioPulseDefinition {
        int PeriodSweepPace;
        int PeriodSweepStep;
        int DutyCycle;
        int InitialLengthTimer;
        int InitialVolume;
        int EnvelopeSweepPace;
        int Period;
        bool LengthEnable;
        
        int CurrentPeriod;
        int CurrentLengthTimer;
    };
}
