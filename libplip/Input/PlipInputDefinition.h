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

    class PlipInputDefinition final {
    public:
        PlipInputDefinition(PlipInputType type, const std::string &description);

        [[nodiscard]] PlipInputData GetData() const;
        [[nodiscard]] std::string GetDescription() const;
        [[nodiscard]] PlipInputType GetType() const;
        void SetData(PlipInputData data);

    private:
        PlipInputData m_data;
        std::string m_description;
        PlipInputType m_type;
    };
}
