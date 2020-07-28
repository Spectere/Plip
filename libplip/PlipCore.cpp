/* PlipCore.cpp
 *
 * Defines an emulation core.
 */

#include "PlipCore.h"

namespace Plip {
    PlipCore::PlipCore(PlipInput *input) {
        m_input = input;
    }

    std::vector<PlipCoreDescription> PlipCore::GetSupportedCores() {
        std::vector<PlipCoreDescription> coreList;

        for(auto core : m_supportedCores)
            coreList.push_back(core);

        return coreList;
    }
}
