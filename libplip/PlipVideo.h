/* PlipVideo.h
 *
 * Provides a toolkit-agnostic video interface.
 */

#pragma once

#include <string>

namespace Plip {
    enum class PlipVideoFormat {
        Unknown,
        RGB888,
        BGR888,
        ARGB8888,
        ABGR8888,
        RGBA8888,
        BGRA8888
    };

    class PlipVideo {
    public:
        virtual bool BeginDraw() = 0;
        virtual void Draw(void *data) = 0;
        virtual void Clear() = 0;
        virtual bool EndDraw() = 0;
        virtual PlipVideoFormat GetFormat() = 0;
        virtual void Resize(int width, int height) = 0;
        virtual void SetTitle(std::string title) = 0;
        virtual void Render() = 0;

    protected:
        PlipVideo() = default;
        ~PlipVideo() = default;
    };
}
