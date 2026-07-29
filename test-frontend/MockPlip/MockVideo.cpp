// MockVideo.cpp
//
// Presents an RGBA8888 surface to libplip and allows the runner to capture the screen of the emulated device.

#include <cstdio>
#include <cstring>
#include <format>

#include <png.h>

#include "MockVideo.h"

// Quite a bit faster than the idiomatic approach on debug builds.
// Doesn't matter for release builds, but I'd rather not have distinct code paths.
#define BUFFER_SIZE (m_width * m_height * 4)

void MockVideo::Clear() {
    std::memset(m_buffer.get(), 0, BUFFER_SIZE);
}

void MockVideo::Draw(void* data) {
    std::memcpy(m_buffer.get(), data, BUFFER_SIZE);
}

void MockVideo::ResizeOutput(const int width, const int height, [[maybe_unused]] const double pixelAspectX, [[maybe_unused]] const double pixelAspectY) {
    m_width = width;
    m_height = height;
    m_buffer = std::make_unique<uint8_t[]>(BUFFER_SIZE);
}

void MockVideo::SaveScreenshot(const std::string& filename) const {
    png_image image = {
        .opaque = nullptr,
        .version = PNG_IMAGE_VERSION,
        .width = static_cast<png_uint_32>(m_width),
        .height = static_cast<png_uint_32>(m_height),
        .format = PNG_FORMAT_RGBA,
        .flags = PNG_FORMAT_FLAG_ALPHA,
    };

    png_image_write_to_file(&image, filename.c_str(), 0, m_buffer.get(), m_width * 4, nullptr);

    if(image.warning_or_error >= 2) {
        // Ignore warnings; throw on error.
        throw std::runtime_error(std::format("Error writing screenshot '{}': {}", filename, image.message));
    }
}
