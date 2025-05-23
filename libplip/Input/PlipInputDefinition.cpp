/* PlipInputDefinition.cpp
 *
 * Allows the core to define an input for the frontend to handle.
 */

#include "PlipInputDefinition.h"

using Plip::PlipInputDefinition;

PlipInputDefinition::PlipInputDefinition(const PlipInputType type, const std::string &description) {
    m_type = type;
    m_description = description;
}

Plip::PlipInputData PlipInputDefinition::GetData() const {
    return m_data;
}

std::string PlipInputDefinition::GetDescription() const {
    return m_description;
}

Plip::PlipInputType PlipInputDefinition::GetType() const {
    return m_type;
}

void PlipInputDefinition::SetData(const PlipInputData data) {
    m_data = data;
}
