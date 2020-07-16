/* PlipInput.h
 *
 * Provides an interface between the emulation core and the frontend.
 */

#pragma once

#include <map>
#include <string>

#include "PlipInputDefinition.h"

namespace Plip {
    class PlipInput {
    public:
        void AddInput(int id, const PlipInputDefinition &input);
        void AddInput(std::map<int, PlipInputDefinition> inputList);
        void ClearInput();

    protected:
        PlipInput() = default;

    private:
        std::map<int, PlipInputDefinition> m_coreInput;
    };
}
