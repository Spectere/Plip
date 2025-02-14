/* PlipCore.h
 *
 * Defines an emulation core.
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "../PlipError.h"
#include "../PlipKeyValuePairCollection.h"
#include "../Audio/PlipAudio.h"
#include "../Input/PlipInput.h"
#include "../Video/PlipVideo.h"
#include "../Memory/PlipMemoryMap.h"

namespace Plip {
    namespace Cpu {
        struct RegisterValue;
    }

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
        void Step();

        virtual void Delta(long ns) = 0;
        virtual std::map<std::string, std::map<std::string, Cpu::RegisterValue>> GetDebugInfo() const = 0;
        [[nodiscard]] PlipMemoryMap* GetMemoryMap() const { return m_memory; }
        virtual PlipError Load(const std::string &path) = 0;

    protected:
        explicit PlipCore(PlipAudio *audio, PlipInput *input, PlipVideo *video, PlipKeyValuePairCollection config);

        PlipAudio *m_audio;
        PlipInput *m_input;
        PlipVideo *m_video;

        PlipKeyValuePairCollection m_config;

        PlipMemoryMap *m_memory = new PlipMemoryMap();

        long m_singleStepTime = {};

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
