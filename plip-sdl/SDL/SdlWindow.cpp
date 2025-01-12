/* SdlWindow.cpp
 *
 * Implements a SDL2 rendering window.
 */

#include <SDL3/SDL_hints.h>
#include <sstream>

#include "Video/PlipVideoException.h"

#include "SdlWindow.h"

namespace PlipSdl {
    SdlWindow::SdlWindow(int scale, const std::string &title) {
        std::stringstream error;

        m_scale = scale;

        SDL_InitSubSystem(SDL_INIT_VIDEO);

        // Try to create a window.
        m_window = SDL_CreateWindow(title.c_str(),
                m_width * m_scale, m_height * m_scale, 0);

        if(m_window == nullptr) {
            error << "Unable to create SDL window: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }

        // Try to create a renderer.
        m_renderer = SDL_CreateRenderer(m_window, nullptr);

        if(m_renderer == nullptr) {
            error << "Unable to create SDL renderer: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }

        // Fetch supported texture formats
        auto textureFormats = static_cast<const SDL_PixelFormat *>(SDL_GetPointerProperty(
            SDL_GetRendererProperties(m_renderer),
            SDL_PROP_RENDERER_TEXTURE_FORMATS_POINTER,
            nullptr
        ));

        if(textureFormats == nullptr) {
            error << "Unable to retrieve supported texture formats: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }

        // Look for whichever 24/32-bit format is supported by this system.
        auto found = false;
        for(auto i = 0; textureFormats[i] != SDL_PIXELFORMAT_UNKNOWN; i++) {
            if(SelectFormat(textureFormats[i])) {
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

    bool SdlWindow::BeginDraw() {
        return SDL_LockTexture(m_texture, nullptr, &m_texData, &m_pitch) == 0;
    }

    void SdlWindow::Clear() {
        SDL_RenderClear(m_renderer);
    }

    void SdlWindow::CreateTexture() {
        if(m_texture != nullptr) SDL_DestroyTexture(m_texture);

        SDL_PixelFormat pixelFormat = SelectSdlFormat(m_format);

        m_texture = nullptr;
        m_texture = SDL_CreateTexture(m_renderer,
                pixelFormat, SDL_TEXTUREACCESS_STREAMING,
                m_width, m_height);

        if(m_texture == nullptr) {
            std::stringstream error;
            error << "Unable to create SDL texture: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }

        SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);
    }

    void SdlWindow::Draw(void *data) {
        memcpy(m_texData, data, m_pitch * m_height);
    }

    bool SdlWindow::EndDraw() {
        SDL_UnlockTexture(m_texture);
        return true;
    }

    Plip::PlipVideoFormat SdlWindow::GetFormat() {
        return m_format;
    }

    int SdlWindow::GetHeight() {
        return m_height;
    }

    int SdlWindow::GetWidth() {
        return m_width;
    }

    void SdlWindow::Render() {
        SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);
        SDL_RenderPresent(m_renderer);
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
            case SDL_PIXELFORMAT_RGB24:
                m_format = Plip::PlipVideoFormat::RGB888;
                return true;

            case SDL_PIXELFORMAT_BGR24:
                m_format = Plip::PlipVideoFormat::BGR888;
                return true;

            case SDL_PIXELFORMAT_XRGB8888:
                m_format = Plip::PlipVideoFormat::XRGB8888;
                return true;

            case SDL_PIXELFORMAT_XBGR8888:
                m_format = Plip::PlipVideoFormat::XBGR8888;
                return true;

            case SDL_PIXELFORMAT_ARGB8888:
                m_format = Plip::PlipVideoFormat::ARGB8888;
                return true;

            case SDL_PIXELFORMAT_ABGR8888:
                m_format = Plip::PlipVideoFormat::ABGR8888;
                return true;

            case SDL_PIXELFORMAT_RGBX8888:
                m_format = Plip::PlipVideoFormat::RGBX8888;
                return true;

            case SDL_PIXELFORMAT_BGRX8888:
                m_format = Plip::PlipVideoFormat::BGRX8888;
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

    SDL_PixelFormat SdlWindow::SelectSdlFormat(Plip::PlipVideoFormat format) {
        switch(format) {
            case Plip::PlipVideoFormat::RGB888:
                return SDL_PIXELFORMAT_RGB24;

            case Plip::PlipVideoFormat::BGR888:
                return SDL_PIXELFORMAT_BGR24;

            case Plip::PlipVideoFormat::XRGB8888:
            case Plip::PlipVideoFormat::ARGB8888:
                return SDL_PIXELFORMAT_ARGB8888;

            case Plip::PlipVideoFormat::XBGR8888:
            case Plip::PlipVideoFormat::ABGR8888:
                return SDL_PIXELFORMAT_ABGR8888;

            case Plip::PlipVideoFormat::RGBX8888:
            case Plip::PlipVideoFormat::RGBA8888:
                return SDL_PIXELFORMAT_RGBA8888;

            case Plip::PlipVideoFormat::BGRX8888:
            case Plip::PlipVideoFormat::BGRA8888:
                return SDL_PIXELFORMAT_BGRA8888;

            default:
                throw Plip::PlipVideoException("Unsupported pixel format.");
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
