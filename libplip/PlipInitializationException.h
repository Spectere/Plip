/* PlipInitializationException.h
 *
 * An exception that may occur during emulation.
 */

#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace Plip {
    struct PlipInitializationException : std::runtime_error {
        explicit PlipInitializationException(const char *message)
                : std::runtime_error(message) {}
    };
}
