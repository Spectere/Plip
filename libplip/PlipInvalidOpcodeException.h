/* PlipInvalidOpcodeException.h
 *
 * Thrown if the CPU core encounters an invalid opcode that is either unimplemented or
 * would cause real hardware to consistently hard lock.
 */

#pragma once

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace Plip {
    struct PlipInvalidOpcodeException final : std::runtime_error {
        template<typename T>
        std::string integerToHexString(T value) {
            std::stringstream sStream;
            sStream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << value;
            return sStream.str();
        }

        template<typename T>
        explicit PlipInvalidOpcodeException(const T opcode)
            : std::runtime_error("Invalid opcode: " + integerToHexString(opcode)) {}
    };
}
