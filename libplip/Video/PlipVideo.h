/* PlipVideo.h
 *
 * Provides a toolkit-agnostic video interface.
 */

#pragma once

#include <cstdint>
#include <string>

namespace Plip {
    enum class PlipVideoFormat {
        Unknown,
        RGB888,
        BGR888,
        XRGB8888,
        XBGR8888,
        ARGB8888,
        ABGR8888,
        RGBX8888,
        BGRX8888,
        RGBA8888,
        BGRA8888
    };

    struct PlipVideoFormatInfo {
        uint8_t pixelWidth;
        void (*plot)(void* data, int offset, uint8_t r, uint8_t g, uint8_t b);
    };

    class PlipVideo {
    public:
        virtual bool BeginDraw() = 0;
        virtual void Draw(void *data) = 0;
        virtual void Clear() = 0;
        virtual bool EndDraw() = 0;
        virtual PlipVideoFormat GetFormat() = 0;
        virtual int GetHeight() = 0;
        virtual int GetWidth() = 0;
        virtual void ResizeOutput(int width, int height, double pixelAspectX, double pixelAspectY) = 0;
        virtual void SetTitle(std::string title) = 0;

        static PlipVideoFormatInfo GetFormatInfo(PlipVideoFormat format);

        // Pixel format functions.
        static void PlotXrgb888(void *data, const int offset, const uint8_t r, const uint8_t g, const uint8_t b) {
            static_cast<uint8_t*>(data)[offset * 3] = r;
            static_cast<uint8_t*>(data)[offset * 3 + 1] = g;
            static_cast<uint8_t*>(data)[offset * 3 + 2] = b;
        }

        static void PlotXbgr888(void *data, const int offset, const uint8_t r, const uint8_t g, const uint8_t b) {
            static_cast<uint8_t*>(data)[offset * 3] = b;
            static_cast<uint8_t*>(data)[offset * 3 + 1] = g;
            static_cast<uint8_t*>(data)[offset * 3 + 2] = r;
        }

        static void PlotArgb8888(void *data, const int offset, const uint8_t r, const uint8_t g, const uint8_t b) {
            static_cast<uint32_t*>(data)[offset] = (0xFF << 24) + (r << 16) + (g << 8) + b;
        }

        static void PlotAbgr8888(void *data, const int offset, const uint8_t r, const uint8_t g, const uint8_t b) {
            static_cast<uint32_t*>(data)[offset] = (0xFF << 24) + (b << 16) + (g << 8) + r;
        }

        static void PlotRgba8888(void *data, const int offset, const uint8_t r, const uint8_t g, const uint8_t b) {
            static_cast<uint32_t*>(data)[offset] = (r << 24) + (g << 16) + (b << 8) + 0xFF;
        }

        static void PlotBgra8888(void *data, const int offset, const uint8_t r, const uint8_t g, const uint8_t b) {
            static_cast<uint32_t*>(data)[offset] = (b << 24) + (g << 16) + (r << 8) + 0xFF;
        }

    protected:
        PlipVideo() = default;
        ~PlipVideo() = default;
    };
}
