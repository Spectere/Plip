// GuiState.h
//
// Defines the shared state between the Game and Gui classes.

#pragma once

#include <cstdint>
#include <set>
#include <vector>

namespace PlipSdl {
    struct GuiState {
        bool GuiShown = false;

        // Emulator state.
        bool PauseCore = false;
        bool SingleStep = false;
        bool TurboEnabled = false;
        double AverageFrameTime = 0;

        // Memory display.
        bool PerformRead = false;

        constexpr static int MemoryDisplayColumns = 16;
        constexpr static int MemoryDisplayRows = 3;
        uint32_t ReadAddress {};
        uint32_t MemoryDisplayBase {};
        uint8_t MemoryContents[MemoryDisplayColumns * MemoryDisplayRows] {};

        // Memory manipulation.
        bool PerformWrite = false;

        std::vector<uint64_t> PcAddresses;

        uint32_t WriteAddress {};
        uint8_t WriteValue {};

        // Breakpoints.
        std::set<uint64_t> Breakpoints {};
        uint64_t BreakpointHit = UINT64_MAX;
        bool BreakpointsUpdated = false;
    };
}
