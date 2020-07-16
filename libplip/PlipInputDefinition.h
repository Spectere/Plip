/* PlipInputDefinition.h
 *
 * Allows the core to define an input for the frontend to handle.
 */

#pragma once

#include <string>

namespace Plip {
    union PlipInputData {
        bool digital = false;
    };

    enum class PlipInputType {
        Digital
    };

    class PlipInputDefinition {
    public:
        PlipInputDefinition(PlipInputType type, const std::string &description);

        std::string GetDescription() const;
        PlipInputType GetType() const;

        PlipInputData data;

    private:
        PlipInputType m_type;
        std::string m_description;
    };
}
