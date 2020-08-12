/* SdlWindow.h
 *
 * Implements a SDL2 rendering window.
 */

#pragma once

#include <SDL.h>

#include "Video/PlipVideo.h"

namespace PlipSdl {
    class SdlWindow : public Plip::PlipVideo {
    public:
        explicit SdlWindow(int gameScale = 1, const std::string &title = "");
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

        bool BeginDrawConsole();
        void DrawConsole(void *data);
        bool EndDrawConsole();
        void SetConsoleEnabled(bool enabled);
        void SetGameScale(int scale);

    private:
        void CreateTexture(SDL_Texture **texture, int width, int height, SDL_BlendMode blendMode = SDL_BLENDMODE_NONE);
        void CreateConsoleTexture();
        void CreateGameTexture();
        bool SelectFormat(uint32_t format);
        static uint32_t SelectSdlFormat(Plip::PlipVideoFormat format);

        const int m_initWidth = 64;
        const int m_initHeight = 64;

        int m_width = m_initWidth;
        int m_height = m_initHeight;
        int m_gameScale;

        void *m_texData = nullptr;
        int m_pitch = -1;

        bool m_drawConsole = false;

        SDL_Window *m_window = nullptr;
        SDL_Renderer *m_renderer = nullptr;
        SDL_Texture *m_conTex = nullptr;
        SDL_Texture *m_gameTex = nullptr;

        Plip::PlipVideoFormat m_format = Plip::PlipVideoFormat::Unknown;
    };
}
