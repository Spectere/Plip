/* SdlWindow.cpp
 *
 * Implements a SDL2 rendering window.
 */

#include <SDL3/SDL_hints.h>
#include <sstream>

#include "Video/PlipVideoException.h"

#include "SdlWindow.h"

#include <cmath>

namespace PlipSdl {
    SdlWindow::SdlWindow(const std::string &title, const bool integerScaling)
    : m_baseTitle(title), m_integerScaling(integerScaling) {
        std::stringstream error;

        SDL_InitSubSystem(SDL_INIT_VIDEO);

        // Try to create a window.
        m_window = SDL_CreateWindow(m_baseTitle.c_str(),
                m_windowWidth, m_windowHeight,
                SDL_WINDOW_RESIZABLE);

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
        const auto textureFormats = static_cast<const SDL_PixelFormat *>(SDL_GetPointerProperty(
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
        CreateTexture(m_windowWidth, m_windowHeight, m_texturePixelAspectX, m_texturePixelAspectY);
        CalculateDestinationRectangle();
    }

    SdlWindow::~SdlWindow() {
        if(m_texture != nullptr) SDL_DestroyTexture(m_texture);
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    bool SdlWindow::BeginDraw() {
        return SDL_LockTexture(m_texture, nullptr, &m_texData, &m_pitch) == 0;
    }

    void SdlWindow::CalculateDestinationRectangle() {
        SDL_GetWindowSize(m_window, &m_windowWidth, &m_windowHeight);

        // Update output size/position, based on the various aspect ratios and scaling options.
        const auto xMaxScale = static_cast<double>(m_windowWidth) / m_textureDisplayWidth;
        const auto yMaxScale = static_cast<double>(m_windowHeight) / m_textureDisplayHeight;
        auto scale = xMaxScale < yMaxScale ? xMaxScale : yMaxScale;

        if(m_integerScaling && scale >= 1.0f) {
            scale = std::floor(scale);
        }

        const int targetWidth = static_cast<int>(m_textureDisplayWidth * scale);
        const int targetHeight = static_cast<int>(m_textureDisplayHeight * scale);
        const int xOffset = (m_windowWidth - targetWidth) / 2;
        const int yOffset = (m_windowHeight - targetHeight) / 2;

        m_destRect.x = static_cast<float>(xOffset);
        m_destRect.y = static_cast<float>(yOffset);
        m_destRect.w = static_cast<float>(targetWidth);
        m_destRect.h = static_cast<float>(targetHeight);
    }

    void SdlWindow::Clear() {
        SDL_RenderClear(m_renderer);
    }

    void SdlWindow::CreateTexture(const int textureWidth, const int textureHeight, const double pixelAspectX, const double pixelAspectY) {
        m_textureWidth = textureWidth;
        m_textureHeight = textureHeight;
        m_texturePixelAspectX = pixelAspectX;
        m_texturePixelAspectY = pixelAspectY;

        m_textureDisplayWidth = static_cast<int>(m_textureWidth * m_texturePixelAspectX);
        m_textureDisplayHeight = static_cast<int>(m_textureHeight * m_texturePixelAspectY);

        if(m_texture != nullptr) SDL_DestroyTexture(m_texture);

        const SDL_PixelFormat pixelFormat = SelectSdlFormat(m_format);

        m_texture = nullptr;
        m_texture = SDL_CreateTexture(m_renderer,
                pixelFormat, SDL_TEXTUREACCESS_STREAMING,
                m_textureWidth, m_textureHeight);

        if(m_texture == nullptr) {
            std::stringstream error;
            error << "Unable to create SDL texture: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }

        SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);
    }

    void SdlWindow::Draw(void *data) {
        memcpy(m_texData, data, m_pitch * m_textureHeight);
    }

    bool SdlWindow::EndDraw() {
        SDL_UnlockTexture(m_texture);
        return true;
    }

    Plip::PlipVideoFormat SdlWindow::GetFormat() {
        return m_format;
    }

    int SdlWindow::GetHeight() {
        return m_textureHeight;
    }

    int SdlWindow::GetWidth() {
        return m_textureWidth;
    }

    void SdlWindow::Render() {
        Clear();
        SDL_RenderTexture(m_renderer, m_texture, nullptr, &m_destRect);
        SDL_RenderPresent(m_renderer);
    }

    void SdlWindow::ResizeOutput(const int width, const int height, const double pixelAspectX, const double pixelAspectY) {
        // Destroy and recreate the texture.
        CreateTexture(width, height, pixelAspectX, pixelAspectY);

        // Resize the window if it's too small to contain the new texture.
        auto doResize = false;
        if(m_windowWidth < m_textureDisplayWidth) {
            m_windowWidth = m_textureDisplayWidth;
            doResize = true;
        }

        if(m_windowHeight < m_textureDisplayHeight) {
            m_windowHeight = m_textureDisplayHeight;
            doResize = true;
        }

        if(doResize) {
            SDL_SetWindowSize(m_window, m_windowWidth, m_windowHeight);
        }

        // Recalculate the destination rectangle.
        CalculateDestinationRectangle();
    }

    bool SdlWindow::SelectFormat(const uint32_t format) {
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

    SDL_PixelFormat SdlWindow::SelectSdlFormat(const Plip::PlipVideoFormat format) {
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

    void SdlWindow::SetScale(const int scale) {
        const auto newWidth = m_textureDisplayWidth * scale;
        const auto newHeight = m_textureDisplayHeight * scale;

        SDL_SetWindowSize(m_window, newWidth, newHeight);
        CalculateDestinationRectangle();
    }

    void SdlWindow::SetTitle(const std::string title) {
        const auto newTitle = m_baseTitle + " - " + title;
        SDL_SetWindowTitle(m_window, newTitle.c_str());
    }
}
