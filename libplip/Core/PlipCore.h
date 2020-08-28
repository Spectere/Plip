/* PlipCore.h
 *
 * Defines an emulation core.
 */

#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../PlipConfig.h"
#include "../PlipError.h"
#include "../Audio/PlipAudio.h"
#include "../Input/PlipInput.h"
#include "../Video/PlipVideo.h"
#include "../Memory/PlipMemoryMap.h"

namespace Plip {
    enum class PlipValidCore {
        Chip8,
        GameBoy
    };

    struct PlipCoreDescription {
        const char *name;
        PlipValidCore descriptor;
        const char *description;
    };

    class PlipCore {
    public:
        static std::vector<PlipCoreDescription> GetSupportedCores();

        [[nodiscard]] bool GetPaused() const;
        [[nodiscard]] long GetStepTime() const;
        void SetPaused(bool value);

        virtual void Delta(long ns) = 0;
        virtual std::string DumpRegisters() = 0;
        virtual PlipMemoryMap* GetMemoryMap() final { return m_memory; }
        virtual PlipError Load(const std::string &path) = 0;
        virtual void Redraw() = 0;

    protected:
        PlipCore(PlipAudio *audio, PlipInput *input, PlipVideo *video, PlipConfig *config);

        PlipAudio *m_audio;
        PlipConfig *m_config;
        PlipInput *m_input;
        PlipVideo *m_video;

        PlipMemoryMap *m_memory = new PlipMemoryMap();

        long m_cycleTime {};
        bool m_paused = false;

    private:
        static constexpr PlipCoreDescription m_supportedCores[] = {
                {
                        "chip8",
                        PlipValidCore::Chip8,
                        "CHIP-8"
                },
                {
                        "gameboy",
                        PlipValidCore::GameBoy,
                        "Nintendo(R) GameBoy(TM)"
                }
        };
    };
}
