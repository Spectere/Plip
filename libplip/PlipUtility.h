/* PlipUtility.h
 *
 * Miscellaneous helper functions.
 */

#include <string>

namespace Plip {
    class PlipUtility {
    public:
        static std::string DumpValue(const std::string &label, uintmax_t value, int precision);
        static std::string FormatHex(uintmax_t value, int precision);
    };
}
