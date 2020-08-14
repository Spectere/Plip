/* SdlWindow.cpp
 *
 * Implements a SDL2 rendering window.
 */

#include <sstream>

#include "Video/PlipVideoException.h"

#include "SdlWindow.h"

namespace PlipSdl {
    SdlWindow::SdlWindow(int gameScale, const std::string &title) {
        std::stringstream error;

        m_gameScale = gameScale;

        SDL_InitSubSystem(SDL_INIT_VIDEO);

        // Try to create a window.
        m_window = SDL_CreateWindow(title.c_str(),
                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    m_width * m_gameScale, m_height * m_gameScale, 0);

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
        CreateConsoleTexture();
        CreateGameTexture();
    }

    SdlWindow::~SdlWindow() {
        if(m_gameTex != nullptr) SDL_DestroyTexture(m_gameTex);
        if(m_renderer != nullptr) SDL_DestroyRenderer(m_renderer);
        if(m_window != nullptr) SDL_DestroyWindow(m_window);
    }

    bool SdlWindow::BeginDraw() {
        return SDL_LockTexture(m_gameTex, nullptr, &m_texData, &m_pitch) == 0;
    }

    bool SdlWindow::BeginDrawConsole() {
        if(!SDL_RenderTargetSupported(m_renderer)) return false;
        SDL_SetRenderTarget(m_renderer, m_conTex);
        return true;
    }

    void SdlWindow::Clear() {
        SDL_RenderClear(m_renderer);
    }

    void SdlWindow::CreateTexture(SDL_Texture **texture, int width, int height,
                                  SDL_BlendMode blendMode, SDL_TextureAccess access) {
        if(*texture != nullptr) SDL_DestroyTexture(*texture);

        uint32_t pixelFormat = SelectSdlFormat(m_format);

        *texture = SDL_CreateTexture(m_renderer,
                                      pixelFormat, access,
                                      width, height);
        SDL_SetTextureBlendMode(*texture, blendMode);

        if(*texture == nullptr) {
            std::stringstream error;
            error << "Unable to create SDL texture: " << SDL_GetError();
            throw Plip::PlipVideoException(error.str().c_str());
        }
    }

    void SdlWindow::CreateConsoleTexture() {
        CreateTexture(&m_conTex, m_width * m_gameScale, m_height * m_gameScale,
                      SDL_BLENDMODE_BLEND, SDL_TEXTUREACCESS_TARGET);
    }

    void SdlWindow::CreateGameTexture() {
        CreateTexture(&m_gameTex, m_width, m_height);
    }

    void SdlWindow::Draw(void *data) {
        memcpy(m_texData, data, m_pitch * m_height);
    }

    bool SdlWindow::EndDraw() {
        SDL_UnlockTexture(m_gameTex);
        return true;
    }

    bool SdlWindow::EndDrawConsole() {
        SDL_SetRenderTarget(m_renderer, nullptr);
        return true;
    }

    Plip::PlipVideoFormat SdlWindow::GetFormat() {
        return m_format;
    }

    int SdlWindow::GetGameScale() const {
        return m_gameScale;
    }

    int SdlWindow::GetHeight() {
        return m_height;
    }

    SDL_Renderer *SdlWindow::GetRenderer() const {
        return m_renderer;
    }

    int SdlWindow::GetWidth() {
        return m_width;
    }

    void SdlWindow::Render() {
        SDL_RenderCopy(m_renderer, m_gameTex, nullptr, nullptr);
        if(m_drawConsole) SDL_RenderCopy(m_renderer, m_conTex, nullptr, nullptr);
        SDL_RenderPresent(m_renderer);
    }

    void SdlWindow::Resize(int width, int height) {
        m_width = width;
        m_height = height;

        // Resize the window.
        SDL_SetWindowSize(m_window, m_width * m_gameScale, m_height * m_gameScale);

        // Destroy and recreate the texture.
        CreateConsoleTexture();
        CreateGameTexture();
    }

    bool SdlWindow::SelectFormat(uint32_t format) {
        switch(format) {
            case SDL_PIXELFORMAT_RGB24:
                m_format = Plip::PlipVideoFormat::RGB888;
                return true;

            case SDL_PIXELFORMAT_BGR24:
                m_format = Plip::PlipVideoFormat::BGR888;
                return true;

            case SDL_PIXELFORMAT_RGB888:
                m_format = Plip::PlipVideoFormat::XRGB8888;
                return true;

            case SDL_PIXELFORMAT_BGR888:
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

    uint32_t SdlWindow::SelectSdlFormat(Plip::PlipVideoFormat format) {
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

    void SdlWindow::SetConsoleEnabled(bool enabled) {
        m_drawConsole = enabled;
    }

    void SdlWindow::SetGameScale(int scale) {
        m_gameScale = scale;
        SDL_SetWindowSize(m_window, m_width * m_gameScale, m_height * m_gameScale);
    }

    void SdlWindow::SetTitle(std::string title) {
        SDL_SetWindowTitle(m_window, title.c_str());
    }
}
