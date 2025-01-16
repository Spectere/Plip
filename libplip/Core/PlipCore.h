/* PlipCore.h
 *
 * Defines an emulation core.
 */

#pragma once

#include <string>
#include <vector>

#include "../PlipError.h"
#include "../PlipKeyValuePairCollection.h"
#include "../Audio/PlipAudio.h"
#include "../Input/PlipInput.h"
#include "../Video/PlipVideo.h"
#include "../Memory/PlipMemoryMap.h"

namespace Plip {
    enum class PlipValidCore {
        Chip8
    };

    struct PlipCoreDescription {
        const char *name;
        PlipValidCore descriptor;
        const char *description;
    };

    class PlipCore {
    public:
        virtual ~PlipCore() = default;

        static std::vector<PlipCoreDescription> GetSupportedCores();

        virtual void Delta(long ns) = 0;
        PlipMemoryMap* GetMemoryMap() const { return m_memory; }
        virtual PlipError Load(const std::string &path) = 0;

    protected:
        explicit PlipCore(PlipAudio *audio, PlipInput *input, PlipVideo *video, PlipKeyValuePairCollection config);

        PlipAudio *m_audio;
        PlipInput *m_input;
        PlipVideo *m_video;

        PlipKeyValuePairCollection m_config;

        PlipMemoryMap *m_memory = new PlipMemoryMap();

    private:
        static constexpr PlipCoreDescription m_supportedCores[] = {
            {
                "chip8",
                PlipValidCore::Chip8,
                "CHIP-8"
            }
        };
    };
}
