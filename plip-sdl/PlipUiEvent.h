// PlipUiEvent.h
//
// Declares the UI events that can occur in Plip.

#pragma once

namespace PlipSdl {
    enum class PlipUiEvent {
        None,
        Quit,
        WindowResized,
        ToggleGui,
        TogglePause,
        PauseEnable,
        PauseDisable,
        TurboEnable,
        TurboDisable,
        Step,
    };
}
