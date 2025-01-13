/* PlipVideo.cpp
 *
 * Provides a toolkit-agnostic video interface.
 */

#include "PlipVideo.h"
#include "PlipVideoException.h"

namespace Plip {
    PlipVideoFormatInfo PlipVideo::GetFormatInfo(const PlipVideoFormat format) {
        switch(format) {
            case PlipVideoFormat::RGB888:
                return { .pixelWidth = 3, .plot = PlotXrgb888 };

            case PlipVideoFormat::BGR888:
                return { .pixelWidth = 3, .plot = PlotXbgr888 };

            case PlipVideoFormat::XRGB8888:
                return { .pixelWidth = 4, .plot = PlotArgb8888 };

            case PlipVideoFormat::XBGR8888:
                return { .pixelWidth = 4, .plot = PlotAbgr8888 };

            case PlipVideoFormat::ARGB8888:
                return { .pixelWidth = 4, .plot = PlotArgb8888 };

            case PlipVideoFormat::ABGR8888:
                return { .pixelWidth = 4, .plot = PlotAbgr8888 };

            case PlipVideoFormat::RGBX8888:
                return { .pixelWidth = 4, .plot = PlotRgba8888 };

            case PlipVideoFormat::BGRX8888:
                return { .pixelWidth = 4, .plot = PlotBgra8888 };

            case PlipVideoFormat::RGBA8888:
                return { .pixelWidth = 4, .plot = PlotRgba8888 };

            case PlipVideoFormat::BGRA8888:
                return { .pixelWidth = 4, .plot = PlotBgra8888 };

            default:
                throw PlipVideoException("Unsupported pixel format.");
        }
    }
}
