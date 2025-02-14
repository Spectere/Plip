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
        explicit SdlWindow(const std::string &title = "", bool integerScaling = false, int lockScale = -1, int forceWidth = -1, int forceHeight = -1);
        ~SdlWindow();

        bool BeginDraw() override;
        void Clear() override;
        void Draw(void *data) override;
        bool EndDraw() override;
        Plip::PlipVideoFormat GetFormat() override;
        int GetHeight() override;
        int GetWidth() override;
        [[nodiscard]] bool IsScaleLocked() const;
        void Present() const;
        void Render();
        void ResizeOutput(int width, int height, double pixelAspectX, double pixelAspectY) override;
        void SetTitle(std::string title) override;

        void CalculateDestinationRectangle();
        [[nodiscard]] SDL_Renderer* GetRenderer() const;
        [[nodiscard]] SDL_Window* GetWindow() const;
        void SetScale(int scale);

    private:
        void CreateTexture(int textureWidth, int textureHeight, double pixelAspectX, double pixelAspectY);
        bool SelectFormat(uint32_t format);
        static SDL_PixelFormat SelectSdlFormat(Plip::PlipVideoFormat format);

        const int m_initWidth = 32;
        const int m_initHeight = 32;
        std::string m_baseTitle;

        int m_windowWidth = m_initWidth;
        int m_windowHeight = m_initHeight;
        bool m_integerScaling = false;
        int m_lockScale = -1;
        bool m_forceDimensions = false;

        int m_textureWidth = m_initWidth;
        int m_textureHeight = m_initHeight;
        int m_textureDisplayWidth = m_initWidth;
        int m_textureDisplayHeight = m_initHeight;
        double m_texturePixelAspectX = 1.0;
        double m_texturePixelAspectY = 1.0;
        SDL_FRect m_destRect = {};

        void* m_texData = nullptr;
        int m_pitch = -1;

        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
        SDL_Texture* m_texture = nullptr;

        Plip::PlipVideoFormat m_format = Plip::PlipVideoFormat::Unknown;
    };
}
