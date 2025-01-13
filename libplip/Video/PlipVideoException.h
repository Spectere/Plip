/* plipVideoException.h
 *
 * An exception that may occur during video initialization.
 */

#pragma once

#include <stdexcept>
#include <utility>

namespace Plip {
    struct PlipVideoException final : std::runtime_error {
        explicit PlipVideoException(const char *message) : std::runtime_error(message) {}
    };
}
