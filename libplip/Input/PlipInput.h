/* PlipInput.h
 *
 * Provides an interface between the emulation core and the frontend.
 */

#pragma once

#include <unordered_map>

#include "PlipInputDefinition.h"

namespace Plip {
    class PlipInput final {
    public:
        PlipInput() = default;

        void AddInput(int id, const PlipInputDefinition &input, const PlipInputData &initialData);
        void AddInput(std::unordered_map<int, PlipInputDefinition> inputList);
        void ClearInput();
        PlipInputData GetInput(int id);
        std::unordered_map<int, PlipInputDefinition> GetInputList() { return m_coreInput; }
        void UpdateInput(int id, PlipInputData data);

    private:
        std::unordered_map<int, PlipInputDefinition> m_coreInput;
    };
}
