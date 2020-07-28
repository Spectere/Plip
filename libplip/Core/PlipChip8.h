/* PlipChip8.h
 *
 * A CHIP-8 implementation for Plip.
 *
 * (hahaha, more like PLIP-8 amirite)
 */

#pragma once

#include <string>

#include "../PlipCore.h"
#include "../PlipError.h"
#include "../Input/PlipInput.h"

namespace Plip::Core {
    class PlipChip8 : public PlipCore {
    public:
        explicit PlipChip8(PlipInput *input);

        PlipError Load(const std::string &path) override;
    };
}
