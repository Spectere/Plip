/* plipVideoException.h
 *
 * An exception that may occur during video initialization.
 */

#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace Plip {
    struct PlipVideoException : std::runtime_error {
        explicit PlipVideoException(const char *message)
            : std::runtime_error(message) {}
    };
}
