/* PlipConfig.h
 *
 * A configuration system for Plip and its cores.
 */

#pragma once

#include <string>
#include <unordered_map>

namespace Plip {
    class PlipConfig {
    public:
        virtual const std::string& GetOption(const std::string &key) final;
        virtual void SetOption(const std::string &key, const std::string &value) final;

        const std::string empty = "\xff";

    private:
        std::unordered_map<std::string, std::string> m_config {};
    };
}
