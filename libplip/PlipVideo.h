/* PlipVideo.h
 *
 * Provides a toolkit-agnostic video interface.
 */

#pragma once

#include <string>

namespace Plip {
    class PlipVideo {
    public:
        virtual void Resize(int width, int height) = 0;
        virtual void SetTitle(std::string title) = 0;

    protected:
        PlipVideo() = default;
        ~PlipVideo() = default;
    };
}
