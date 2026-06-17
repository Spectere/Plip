/* PlipEmulationException.h
 *
 * An exception that may occur during emulation.
 */

#pragma once

#include <stdexcept>

namespace Plip {
    struct PlipEmulationException final : std::runtime_error {
        explicit PlipEmulationException(const char *message, const bool skipsUnitTest = false)
            : std::runtime_error(message), SkipsUnitTest(skipsUnitTest) {}

        bool SkipsUnitTest;
    };
}
