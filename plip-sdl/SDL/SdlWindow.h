/* SdlWindow.h
 *
 * Implements a SDL2 rendering window.
 */

#pragma once

#include <SDL3/SDL.h>

#include "Video/PlipVideo.h"

namespace PlipSdl {
    class SdlWindow final : public Plip::PlipVideo {
    public:
        explicit SdlWindow(int scale = 1, const std::string &title = "");
        ~SdlWindow();

        bool BeginDraw() override;
        void Clear() override;
        void Draw(void *data) override;
        bool EndDraw() override;
        Plip::PlipVideoFormat GetFormat() override;
        int GetHeight() override;
        int GetWidth() override;
        void Render() override;
        void Resize(int width, int height) override;
        void SetTitle(std::string title) override;

        void SetScale(int scale);

    private:
        void CreateTexture();
        bool SelectFormat(uint32_t format);
        static SDL_PixelFormat SelectSdlFormat(Plip::PlipVideoFormat format);

        const int m_initWidth = 64;
        const int m_initHeight = 64;

        int m_width = m_initWidth;
        int m_height = m_initHeight;
        int m_scale;

        void *m_texData = nullptr;
        int m_pitch = -1;

        SDL_Window *m_window = nullptr;
        SDL_Renderer *m_renderer = nullptr;
        SDL_Texture *m_texture = nullptr;

        Plip::PlipVideoFormat m_format = Plip::PlipVideoFormat::Unknown;
    };
}
