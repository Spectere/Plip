// MockVideo.h
//
// Presents an ARGB8888 surface to libplip and allows the runner to capture the screen of the emulated device.

#pragma once

#include <memory>

#include "Video/PlipVideo.h"

class MockVideo final : public Plip::PlipVideo {
public:
    bool BeginDraw() override { return true; }
    void Draw(void* data) override;
    void Clear() override;
    bool EndDraw() override { return true; }
    Plip::PlipVideoFormat GetFormat() override {
#ifdef SYS_BIG_ENDIAN
        return Plip::PlipVideoFormat::RGBA8888;
#else
        // PNG data is big endian. Request reversed byte order from libplip.
        return Plip::PlipVideoFormat::ABGR8888;
#endif // SYS_BIG_ENDIAN
    }
    int GetHeight() override { return m_height; }
    int GetWidth() override { return m_width; }
    void ResizeOutput(int width, int height, double pixelAspectX, double pixelAspectY) override;
    void SetTitle(std::string title) override { /* no-op */ }

    void SaveScreenshot(const std::string& filename) const;

private:
    int m_width {};
    int m_height {};
    std::unique_ptr<uint8_t[]> m_buffer;
};
