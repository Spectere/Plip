/* PlipInput.cpp
 *
 * Provides an interface between the emulation core and the frontend.
 */

#include "PlipInput.h"

namespace Plip {
    void PlipInput::AddInput(int id, const PlipInputDefinition& input) {
        m_coreInput.insert(std::pair<int, PlipInputDefinition>(id, input));
    }

    void PlipInput::AddInput(std::map<int, PlipInputDefinition> inputList) {
        m_coreInput.insert(inputList.begin(), inputList.end());
    }

    void PlipInput::ClearInput() {
        m_coreInput.clear();
    }
}
