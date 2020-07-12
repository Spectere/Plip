/* SdlWindow.h
 *
 * Describe this file here.
 */

#pragma once

#include <SDL.h>

#include "PlipVideo.h"

namespace PlipSdl {
    class SdlWindow : Plip::PlipVideo {
    public:
        SdlWindow(int scale = 1, std::string title = "");
        ~SdlWindow();

        void Resize(int width, int height);
        void SetScale(int scale);
        void SetTitle(std::string title);

    private:
        const int m_init_width = 64;
        const int m_init_height = 64;

        int m_width = m_init_width;
        int m_height = m_init_height;

        SDL_Window *m_window = nullptr;
        SDL_Renderer *m_renderer = nullptr;
        SDL_Texture *m_texture = nullptr;

        int m_scale;
    };
}
