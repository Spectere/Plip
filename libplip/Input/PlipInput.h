/* PlipInput.h
 *
 * Provides an interface between the emulation core and the frontend.
 */

#pragma once

#include <unordered_map>
#include <string>

#include "PlipInputDefinition.h"

namespace Plip {
    class PlipInput final {
    public:
        PlipInput() = default;

        virtual void AddInput(int id, const PlipInputDefinition &input, const PlipInputData &initialData);
        virtual void AddInput(std::unordered_map<int, PlipInputDefinition> inputList);
        virtual void ClearInput();
        virtual PlipInputData GetInput(int id);
        virtual void UpdateInput(int id, PlipInputData data);

    private:
        std::unordered_map<int, PlipInputDefinition> m_coreInput;
    };
}
