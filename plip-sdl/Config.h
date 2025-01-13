/* Config.h
 *
 * Loads and stores configuration data in an INI-like format.
 */

#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

namespace PlipSdl {
    class Config {
    public:
        const std::string &GetValue(const std::string &key);
        const std::string &GetValue(const std::string &section, const std::string &key);
        bool LoadFile(const std::string &filename);
        void SetValue(const std::string &key, const std::string &value);
        void SetValue(const std::string &section, const std::string &key, const std::string &value);

        template<typename T>
        T GetValue(const std::string &key) {
            return GetValue<T>(global, key);
        }

        template<typename T>
        T GetValue(const std::string &section, const std::string &key) {
            const auto val = GetValue(section, key);
            T output;

            std::stringstream conversion(val);
            conversion >> output;
            return output;
        }

        const std::string global = "\x01";
        const std::string empty = "\xff";

    private:
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_section;
    };
}
