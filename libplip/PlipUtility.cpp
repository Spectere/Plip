/* PlipUtility.cpp
 *
 * Miscellaneous helper functions.
 */

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
