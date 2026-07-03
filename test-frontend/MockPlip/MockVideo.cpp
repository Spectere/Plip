// MockVideo.cpp
//
// Presents an RGBA8888 surface to libplip and allows the runner to capture the screen of the emulated device.

#include <cstring>
#include <format>

#include "lodepng.h"

#include "MockVideo.h"

// Quite a bit faster than the idiomatic approach on debug builds.
// Doesn't matter for release builds, but I'd rather not have distinct code paths.
#define BUFFER_SIZE (m_width * m_height * 3)

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
    if(const auto error = lodepng::encode(filename, m_buffer.get(), m_width, m_height, LCT_RGB, 8)) {
        throw std::runtime_error(std::format("Error '{}' writing screenshot '{}'", lodepng_error_text(error), filename));
    }
}
