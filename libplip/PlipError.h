/* PlipError.h
 *
 * Defines a set of errors that can be returned to the frontend.
 */

#pragma once

namespace Plip {
    enum class PlipError {
        Success,
        InvalidCore,
        FileNotFound,
        RomFileTruncated,
        UnrecognizedMedia,
    };
}
