/* PlipInput.cpp
 *
 * Provides an interface between the emulation core and the frontend.
 */

#include "PlipInput.h"

using Plip::PlipInput;

void PlipInput::AddInput(int id, const PlipInputDefinition &input, const PlipInputData &initialData) {
    m_coreInput.insert(std::pair(id, input));
    UpdateInput(id, initialData);
}

void PlipInput::AddInput(std::unordered_map<int, PlipInputDefinition> inputList) {
    m_coreInput.insert(inputList.begin(), inputList.end());
}

void PlipInput::ClearInput() {
    m_coreInput.clear();
}

Plip::PlipInputData PlipInput::GetInput(const int id) {
    return m_coreInput.find(id)->second.GetData();
}

void PlipInput::UpdateInput(const int id, const PlipInputData data) {
    const auto it = m_coreInput.find(id);
    if(it == m_coreInput.end()) return;
    it->second.SetData(data);
}
