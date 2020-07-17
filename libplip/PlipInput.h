/* PlipInput.h
 *
 * Provides an interface between the emulation core and the frontend.
 */

#pragma once

#include <unordered_map>
#include <string>

#include "PlipInputDefinition.h"

namespace Plip {
    class PlipInput {
    public:
        virtual void AddInput(int id, const PlipInputDefinition &input, const PlipInputData &initialData) final;
        virtual void AddInput(std::unordered_map<int, PlipInputDefinition> inputList) final;
        virtual void ClearInput() final;
        virtual PlipInputData GetInput(int id) final;
        virtual void UpdateInput(int id, PlipInputData data) final;

    protected:
        PlipInput() = default;

    private:
        std::unordered_map<int, PlipInputDefinition> m_coreInput;
    };
}
