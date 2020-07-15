/* Plip.cpp
 *
 * The main class for controlling the Plip emulation suite.
 */

#include "Plip.h"
#include "PlipVersion.h"

namespace Plip {
    std::string Plip::GetVersion() {
        #ifndef GIT_FOUND
            return PRODUCT_NAME;
        #else
            #ifndef GIT_TAG
                return PRODUCT_NAME " (" GIT_BRANCH "@" GIT_REVISION ")";
            #else
                return PRODUCT_NAME " " GIT_TAG;
            #endif // GIT_TAG
        #endif // GIT_FOUND
    }
}
