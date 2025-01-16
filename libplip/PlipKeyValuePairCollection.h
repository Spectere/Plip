/* PlipKeyValuePairCollection.h
 *
 * A collection of key-value pairs.
 */

#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

namespace Plip {
    class PlipKeyValuePairCollection {
    public:
        PlipKeyValuePairCollection() = default;
        explicit PlipKeyValuePairCollection(std::unordered_map<std::string, std::string> collection) : m_collection(std::move(collection)) {}

        bool Contains(const std::string &key) const {
            return m_collection.find(key) != m_collection.end();
        }

        const std::string& GetValue(const std::string &key) const {
            static const std::string empty = {};
            try {
                return m_collection.at(key);
            } catch([[maybe_unused]] std::out_of_range &ex) {
                return empty;
            }
        }

        template<typename T>
        T GetValue(const std::string &key) {
            const auto val = GetValue(key);
            T output;

            std::stringstream conversion(val);
            conversion >> output;
            return output;
        }

        template<typename T>
        T GetValue(const std::string &key, T defaultValue) {
            if(!Contains(key)) {
                return defaultValue;
            }

            return GetValue<T>(key);
        }

    private:
        std::unordered_map<std::string, std::string> m_collection = {};
    };
}
