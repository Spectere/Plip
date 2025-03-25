/* PlipCore.h
 *
 * Defines an emulation core.
 */

#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "../DebugValue.h"
#include "../PlipError.h"
#include "../PlipKeyValuePairCollection.h"
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
        virtual ~PlipCore() = default;

        static std::vector<PlipCoreDescription> GetSupportedCores();
        void Step();

        void ClearActiveBreakpoint() { m_activeBreakpoint = UINT64_MAX; }
        virtual void Delta(long ns) = 0;
        uint64_t GetActiveBreakpoint() const { return m_activeBreakpoint; }
        virtual std::map<std::string, std::map<std::string, DebugValue>> GetDebugInfo() const = 0;
        [[nodiscard]] PlipMemoryMap* GetMemoryMap() const { return m_memory; }
        virtual std::vector<uint64_t> GetPcs() const = 0;
        virtual PlipError Load(const std::string &path) = 0;
        virtual void Reset() = 0;
        void SetBreakpoints(const std::set<uint64_t> &breakpoints) { m_breakpoints = breakpoints; }

    protected:
        explicit PlipCore(PlipAudio *audio, PlipInput *input, PlipVideo *video, PlipKeyValuePairCollection config);

        void SetActiveBreakpoint(const uint64_t addr) { m_activeBreakpoint = addr; }

        PlipAudio *m_audio;
        PlipInput *m_input;
        PlipVideo *m_video;

        PlipKeyValuePairCollection m_config;

        PlipMemoryMap *m_memory = new PlipMemoryMap();

        std::set<uint64_t> m_breakpoints {};

        long m_singleStepTime = {};

    private:
        uint64_t m_activeBreakpoint = UINT64_MAX;

        static constexpr PlipCoreDescription m_supportedCores[] = {
            {
                "chip8",
                PlipValidCore::Chip8,
                "CHIP-8"
            },
            {
                "gameboy",
                PlipValidCore::GameBoy,
                "Nintendo(R) Game Boy(TM)"
            }
        };
    };
}
