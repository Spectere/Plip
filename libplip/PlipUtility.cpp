/* PlipUtility.cpp
 *
 * Miscellaneous helper functions.
 */

#include <iomanip>
#include <sstream>

#include "PlipUtility.h"

namespace Plip {
    std::string PlipUtility::DumpValue(const std::string &label, uintmax_t value, int precision) {
        std::stringstream dump;
        dump << '\t' << label << ": " << FormatHex(value, precision);
        return dump.str();
    }

    std::string PlipUtility::FormatHex(uintmax_t value, int precision) {
        std::stringstream fmt;
        fmt << "0x" << std::uppercase << std::setfill('0') << std::setw(precision)
            << std::hex << value;

        return fmt.str();
    }
}
