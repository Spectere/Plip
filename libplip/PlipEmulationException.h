/* PlipEmulationException.h
 *
 * An exception that may occur during emulation.
 */

#pragma once

#include <stdexcept>
#include <utility>

namespace Plip {
    struct PlipEmulationException final : std::runtime_error {
        explicit PlipEmulationException(const char *message) : std::runtime_error(message) {}
    };
}
