/* PlipConfig.cpp
 *
 * A configuration interface for Plip and its cores.
 */

#include "PlipConfig.h"

#include <algorithm>

namespace Plip {
    static inline std::string ToLower(std::string str) {
        std::transform(str.cbegin(), str.cend(), str.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return str;
    }

    const std::string& PlipConfig::GetOption(const std::string &key) {
        auto itConfig = m_config.find(ToLower(key));
        if(itConfig == m_config.cend()) return empty;

        return itConfig->second;
    }

    void PlipConfig::SetOption(const std::string &key, const std::string &value) {
        m_config.insert(std::pair<std::string, std::string>(ToLower(key), value));
    }
}
