/* PlipCore.h
 *
 * Defines an emulation core.
 */

#pragma once

#include <string>
#include <vector>

#include "PlipError.h"
#include "Input/PlipInput.h"
#include "Memory/PlipMemoryMap.h"

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
        static std::vector<PlipCoreDescription> GetSupportedCores();

        virtual PlipMemoryMap* GetMemoryMap() final { return m_memoryMap; }
        virtual PlipError Load(const std::string &path) = 0;

    protected:
        explicit PlipCore(PlipInput *input);

        PlipInput *m_input;
        PlipMemoryMap *m_memoryMap = new PlipMemoryMap();

        static constexpr PlipCoreDescription m_supportedCores[] = {
                {
                        "chip8",
                        PlipValidCore::Chip8,
                        "CHIP-8"
                }
        };
    };
}
