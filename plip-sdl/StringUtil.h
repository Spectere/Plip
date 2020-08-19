/* StringUtil.h
 *
 * A bunch of common string manipulation function.
 */

#pragma once

#include <string>
#include <vector>

namespace PlipSdl {
    class StringUtil {
    public:
        static inline std::vector<std::string> Split(std::string str, unsigned char ch) {
            std::vector<std::string> results;

            str = Trim(str);
            while(true) {
                auto idx = str.find(ch, 0);
                auto val = Trim(str.substr(0, idx));
                results.push_back(val);

                if(idx == std::string::npos)
                    break;

                str = Trim(str.substr(idx + 1, std::string::npos));
            }

            return results;
        }

        static inline std::string ToLower(std::string str) {
            std::transform(str.cbegin(), str.cend(), str.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            return str;
        }

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
    };
}
