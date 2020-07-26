/* Config.cpp
 *
 * Loads and stores configuration data in an INI-like format.
 */

#include <algorithm>
#include <fstream>
#include <iostream>

#include "Config.h"

namespace PlipSdl {
    static inline std::string Trim(std::string str) {
        // Trim front.
        str.erase(str.cbegin(), std::find_if(str.cbegin(), str.cend(), [](unsigned char c) {
            return !std::isspace(c);
        }));

        // Trim back.
        str.erase(std::find_if(str.crbegin(), str.crend(), [](unsigned char c) {
            return !std::isspace(c);
        }).base(), str.cend());

        return str;
    }

    static inline std::string ToLower(std::string str) {
        std::transform(str.cbegin(), str.cend(), str.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return str;
    }

    const std::string &Config::GetValue(const std::string &key) {
        return GetValue(global, key);
    }

    const std::string &Config::GetValue(const std::string &section, const std::string &key) {
        auto secLower = ToLower(section);
        auto keyLower = ToLower(key);

        auto itSection = m_section.find(secLower);
        if(itSection == m_section.end()) return empty;

        auto itKey = itSection->second.find(keyLower);
        if(itKey == itSection->second.end()) return empty;

        return itKey->second;
    }

    bool Config::LoadFile(const std::string &filename) {
        std::string section = global;

        std::ifstream file;
        file.open(filename);

        if(!file.is_open()) return false;

        std::string line;
        int lineNum = 0;
        while(std::getline(file, line)) {
            line = Trim(line);
            ++lineNum;

            if(line.empty()) continue;

            // Comment.
            if(line.front() == '#') continue;

            if(line.front() == '[' && line.back() == ']') {
                // Section
                line.erase(line.begin());
                line.erase(line.end() - 1);
                section = Trim(line);
                continue;
            }

            // Everything else. :)
            auto equals = line.find('=', 0);
            if(equals == std::string::npos) {
                std::cerr << filename << ": no value specified on line " << lineNum << std::endl;
                continue;
            }

            auto key = Trim(line.substr(0, equals));
            auto value = Trim(line.substr(equals + 1, std::string::npos));
            SetValue(section, key, value);
        }

        file.close();
        return true;
    }

    void Config::SetValue(const std::string &key, const std::string &value) {
        SetValue(global, key, value);
    }

    void Config::SetValue(const std::string &section, const std::string &key, const std::string &value) {
        auto secLower = ToLower(section);
        auto keyLower = ToLower(key);

        auto itSection = m_section.find(secLower);
        if(itSection == m_section.end()) {
            // O.o
            auto newSection = std::pair<std::string, std::unordered_map<std::string, std::string>>
                    (secLower, std::unordered_map<std::string, std::string>());
            newSection.second.insert(std::pair<std::string, std::string>(keyLower, value));
            m_section.insert(newSection);
            return;
        }

        auto itKey = itSection->second.find(keyLower);
        if(itKey == itSection->second.end()) {
            itSection->second.insert(std::pair<std::string, std::string>(keyLower, value));
            return;
        }

        itKey->second = value;
    }
}
