/* PlipUtility.cpp
 *
 * Miscellaneous helper functions.
 */

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "PlipUtility.h"

using Plip::PlipUtility;

std::string PlipUtility::DumpValue(const std::string &label, const uintmax_t value, const int precision) {
    std::stringstream dump;
    dump << '\t' << label << ": " << FormatHex(value, precision);
    return dump.str();
}

std::string PlipUtility::FormatHex(const uintmax_t value, const int precision) {
    std::stringstream fmt;
    fmt << "0x" << std::uppercase << std::setfill('0') << std::setw(precision)
        << std::hex << value;

    return fmt.str();
}

std::string PlipUtility::StringToLower(const std::string &str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}
