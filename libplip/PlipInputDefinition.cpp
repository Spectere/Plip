/* PlipInputDefinition.cpp
 *
 * Allows the core to define an input for the frontend to handle.
 */

#include "PlipInputDefinition.h"

namespace Plip {
    PlipInputDefinition::PlipInputDefinition(PlipInputType type, const std::string &description) {
        m_type = type;
        m_description = description;
    }

    std::string PlipInputDefinition::GetDescription() const {
        return m_description;
    }

    PlipInputType PlipInputDefinition::GetType() const {
        return m_type;
    }
}
