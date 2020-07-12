/* SdlWindow.cpp
 *
 * Describe this file here.
 */

#include <sstream>

#include "PlipVideoException.h"

#include "SdlWindow.h"

namespace PlipSdl {
    SdlWindow::SdlWindow(int scale, std::string title) {
        std::stringstream error;

        m_scale = scale;

        // Try to create a window.
        m_window = SDL_CreateWindow(title.c_str(),
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                m_width * m_scale, m_height * m_scale, 0);

        if(m_window == nullptr) {
            error << "Unable to create SDL window: " << SDL_GetError() << "\n";
            throw Plip::PlipVideoException(error.str().c_str());
        }

        // Try to create a renderer.
        m_renderer = SDL_CreateRenderer(m_window, -1, 0);

        if(m_renderer == nullptr) {
            error << "Unable to create SDL renderer: " << SDL_GetError() << "\n";
            throw Plip::PlipVideoException(error.str().c_str());
        }

        // Try to create a small starting texture. This will be removed and
        // recreated later, but we want to ensure that we can make one in the
        // first place.
        m_texture = SDL_CreateTexture(m_renderer, 0, 0, m_width, m_height);

        if(m_texture == nullptr) {
            error << "Unable to create SDL texture: " << SDL_GetError() << "\n";
            throw Plip::PlipVideoException(error.str().c_str());
        }
    }

    SdlWindow::~SdlWindow() {
        if(m_texture != nullptr) SDL_DestroyTexture(m_texture);
        if(m_renderer != nullptr) SDL_DestroyRenderer(m_renderer);
        if(m_window != nullptr) SDL_DestroyWindow(m_window);
    }

    void SdlWindow::Resize(int width, int height) {
        m_width = width;
        m_height = height;

        // Resize the window.
        SDL_SetWindowSize(m_window, m_width * m_scale, m_height * m_scale);

        // Destroy and recreate the texture.
        SDL_DestroyTexture(m_texture);
        SDL_CreateTexture(m_renderer, 0, 0, m_width, m_height);
    }

    void SdlWindow::SetScale(int scale) {
        m_scale = scale;
        SDL_SetWindowSize(m_window, m_width * m_scale, m_height * m_scale);
    }

    void SdlWindow::SetTitle(std::string title) {
        SDL_SetWindowTitle(m_window, title.c_str());
    }
}
