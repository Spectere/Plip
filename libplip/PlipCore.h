/* PlipCore.h
 *
 * Defines an emulation core.
 */

#pragma once

#include <string>

#include "PlipError.h"
#include "Input/PlipInput.h"
#include "Memory/PlipMemoryMap.h"

namespace Plip {
    enum class PlipValidCore {
        Chip8
    };

    class PlipCore {
    public:
        virtual const PlipMemoryMap* GetMemoryMap() final { return m_memoryMap; }
        virtual PlipError Load(const std::string &path) = 0;

    protected:
        explicit PlipCore(PlipInput *input);

        PlipInput *m_input;
        PlipMemoryMap *m_memoryMap = new PlipMemoryMap();
    };
}
