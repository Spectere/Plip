/* SdlWindow.cpp
 *
 * Implements a SDL2 rendering window.
 */

#include <sstream>

#include "PlipVideoException.h"

#include "SdlWindow.h"

namespace PlipSdl {
    SdlWindow::SdlWindow(int scale, const std::string &title) {
        std::stringstream error;

        m_scale = scale;

        // Try to create a window.
        m_window = SDL_CreateWindow(title.c_str(),
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                m_width * m_scale, m_height * m_scale, 0);

        if(m_window == nullptr) {
            error << "Unable to create SDL window: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }

        // Try to create a renderer.
        m_renderer = SDL_CreateRenderer(m_window, -1, 0);

        if(m_renderer == nullptr) {
            error << "Unable to create SDL renderer: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }

        SDL_RendererInfo rendererInfo;
        if(SDL_GetRendererInfo(m_renderer, &rendererInfo)) {
            error << "Unable to retrieve SDL renderer info: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }

        auto found = false;
        for(auto i = 0; i < rendererInfo.num_texture_formats; i++) {
            if(SelectFormat(rendererInfo.texture_formats[i])) {
                found = true;
                break;
            }
        }

        if(!found) {
            error << "No supported texture formats!";
            throw Plip::PlipVideoException(error.str().c_str());
        }

        // Try to create a small starting texture. This will be removed and
        // recreated later, but we want to ensure that we can make one in the
        // first place.
        CreateTexture();
    }

    SdlWindow::~SdlWindow() {
        if(m_texture != nullptr) SDL_DestroyTexture(m_texture);
        if(m_renderer != nullptr) SDL_DestroyRenderer(m_renderer);
        if(m_window != nullptr) SDL_DestroyWindow(m_window);
    }

    void SdlWindow::CreateTexture() {
        if(m_texture != nullptr) SDL_DestroyTexture(m_texture);

        uint32_t pixelFormat;
        switch(m_format) {
            case Plip::PlipVideoFormat::RGB888:
                pixelFormat = SDL_PIXELFORMAT_RGB888;
                break;
            case Plip::PlipVideoFormat::BGR888:
                pixelFormat = SDL_PIXELFORMAT_BGR888;
                break;
            case Plip::PlipVideoFormat::ARGB8888:
                pixelFormat = SDL_PIXELFORMAT_ARGB8888;
                break;
            case Plip::PlipVideoFormat::ABGR8888:
                pixelFormat = SDL_PIXELFORMAT_ABGR8888;
                break;
            case Plip::PlipVideoFormat::RGBA8888:
                pixelFormat = SDL_PIXELFORMAT_RGBA8888;
                break;
            case Plip::PlipVideoFormat::BGRA8888:
                pixelFormat = SDL_PIXELFORMAT_BGRA8888;
                break;
        }

        m_texture = nullptr;
        m_texture = SDL_CreateTexture(m_renderer,
                pixelFormat, SDL_TEXTUREACCESS_STREAMING,
                m_width, m_height);

        if(m_texture == nullptr) {
            std::stringstream error;
            error << "Unable to create SDL texture: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }
    }

    Plip::PlipVideoFormat SdlWindow::GetFormat() {
        return m_format;
    }

    void SdlWindow::Resize(int width, int height) {
        m_width = width;
        m_height = height;

        // Resize the window.
        SDL_SetWindowSize(m_window, m_width * m_scale, m_height * m_scale);

        // Destroy and recreate the texture.
        CreateTexture();
    }

    bool SdlWindow::SelectFormat(uint32_t format) {
        switch(format) {
            case SDL_PIXELFORMAT_RGB888:
                m_format = Plip::PlipVideoFormat::RGB888;
                return true;
            case SDL_PIXELFORMAT_BGR888:
                m_format = Plip::PlipVideoFormat::BGR888;
                return true;
            case SDL_PIXELFORMAT_ARGB8888:
                m_format = Plip::PlipVideoFormat::ARGB8888;
                return true;
            case SDL_PIXELFORMAT_ABGR8888:
                m_format = Plip::PlipVideoFormat::ABGR8888;
                return true;
            case SDL_PIXELFORMAT_RGBA8888:
                m_format = Plip::PlipVideoFormat::RGBA8888;
                return true;
            case SDL_PIXELFORMAT_BGRA8888:
                m_format = Plip::PlipVideoFormat::BGRA8888;
                return true;
            default:
                return false;
        }
    }

    void SdlWindow::SetScale(int scale) {
        m_scale = scale;
        SDL_SetWindowSize(m_window, m_width * m_scale, m_height * m_scale);
    }

    void SdlWindow::SetTitle(std::string title) {
        SDL_SetWindowTitle(m_window, title.c_str());
    }
}
