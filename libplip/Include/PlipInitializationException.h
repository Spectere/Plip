/* PlipInitializationException.h
 *
 * An exception that may occur during emulation.
 */

#pragma once

#include <stdexcept>

namespace Plip {
    struct PlipInitializationException final : std::runtime_error {
        explicit PlipInitializationException(const char *message) : std::runtime_error(message) {}
    };
}
