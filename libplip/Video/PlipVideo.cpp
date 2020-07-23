/* PlipVideo.cpp
 *
 * Provides a toolkit-agnostic video interface.
 */

#include "PlipVideo.h"
#include "PlipVideoException.h"

namespace Plip {
    PlipVideoFormatInfo PlipVideo::GetFormatInfo(PlipVideoFormat format) {
        switch(format) {
            case PlipVideoFormat::RGB888:
                return { .pixelWidth = 3, .plot = PlipVideo::PlotRgb888 };

            case PlipVideoFormat::BGR888:
                return { .pixelWidth = 3, .plot = PlipVideo::PlotBgr888 };

            case PlipVideoFormat::XRGB8888:
                return { .pixelWidth = 4, .plot = PlipVideo::PlotArgb8888 };

            case PlipVideoFormat::XBGR8888:
                return { .pixelWidth = 4, .plot = PlipVideo::PlotAbgr8888 };

            case PlipVideoFormat::ARGB8888:
                return { .pixelWidth = 4, .plot = PlipVideo::PlotArgb8888 };

            case PlipVideoFormat::ABGR8888:
                return { .pixelWidth = 4, .plot = PlipVideo::PlotAbgr8888 };

            case PlipVideoFormat::RGBX8888:
                return { .pixelWidth = 4, .plot = PlipVideo::PlotRgba8888 };

            case PlipVideoFormat::BGRX8888:
                return { .pixelWidth = 4, .plot = PlipVideo::PlotBgra8888 };

            case PlipVideoFormat::RGBA8888:
                return { .pixelWidth = 4, .plot = PlipVideo::PlotRgba8888 };

            case PlipVideoFormat::BGRA8888:
                return { .pixelWidth = 4, .plot = PlipVideo::PlotBgra8888 };

            default:
                throw PlipVideoException("Unsupported pixel format.");
        }
    }
}
