/* PlipKeyValuePairCollection.h
 *
 * A collection of key-value pairs.
 */

#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Plip {
    class PlipKeyValuePairCollection {
    public:
        PlipKeyValuePairCollection() = default;
        explicit PlipKeyValuePairCollection(std::unordered_map<std::string, std::string> collection) : m_collection(std::move(collection)) {}

        bool Contains(const std::string &key) const {
            return m_collection.find(ToLower(key)) != m_collection.end();
        }

        const std::string& GetValue(const std::string &key) const {
            static const std::string empty = {};
            try {
                return m_collection.at(ToLower(key));
            } catch([[maybe_unused]] std::out_of_range &ex) {
                return empty;
            }
        }

        template<typename T>
        T GetValue(const std::string &key) {
            const auto val = GetValue(ToLower(key));
            T output;

            std::stringstream conversion(val);
            conversion >> output;
            return output;
        }

        template<typename T>
        T GetValue(const std::string &key, T defaultValue) {
            if(!Contains(ToLower(key))) {
                return defaultValue;
            }

            return GetValue<T>(ToLower(key));
        }

    private:
        static std::string ToLower(std::string str) {
            std::transform(str.cbegin(), str.cend(), str.begin(), [](const unsigned char c) {
                return std::tolower(c);
            });

            return str;
        }

        std::unordered_map<std::string, std::string> m_collection = {};
    };
}
