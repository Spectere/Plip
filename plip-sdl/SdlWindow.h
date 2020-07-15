/* SdlWindow.h
 *
 * Implements a SDL2 rendering window.
 */

#pragma once

#include <SDL.h>

#include "PlipVideo.h"

namespace PlipSdl {
    class SdlWindow : Plip::PlipVideo {
    public:
        explicit SdlWindow(int scale = 1, const std::string &title = "");
        ~SdlWindow();

        Plip::PlipVideoFormat GetFormat() override;
        void Resize(int width, int height) override;
        void SetScale(int scale);
        void SetTitle(std::string title) override;

    private:
        void CreateTexture();
        bool SelectFormat(uint32_t format);

        const int m_init_width = 64;
        const int m_init_height = 64;

        int m_width = m_init_width;
        int m_height = m_init_height;
        int m_scale;

        SDL_Window *m_window = nullptr;
        SDL_Renderer *m_renderer = nullptr;
        SDL_Texture *m_texture = nullptr;

        Plip::PlipVideoFormat m_format = Plip::PlipVideoFormat::Unknown;

    };
}
