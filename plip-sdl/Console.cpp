/* Console.cpp
 *
 * A Quake-style console implementation.
 */

#include <SDL.h>
#include <SDL_image.h>

#include "Console.h"

namespace PlipSdl {
    Console::Console(SdlWindow *wnd) {
        m_video = wnd;
        m_renderer = wnd->GetRenderer();
        m_vidFmt = Plip::PlipVideo::GetFormatInfo(m_video->GetFormat());

        IMG_Init(0);
    }

    Console::~Console() {
        IMG_Quit();
    }

    void Console::Clear() {
        for(auto i = 0; i < m_conWidth * m_conHeight; i++)
            m_conBuffer[i] = 0x20;

        // Test code. TODO: Remove me :)
        const std::string str = "Hello world!";
        auto i = 0;
        for(auto ch : str)
            m_conBuffer[i++] = ch;
        m_conBuffer[m_conWidth * m_conHeight - 1] = 0x7F;
    }

    void Console::Draw() {
        if(m_renderer == nullptr) return;

        // Background
        SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xC0);
        SDL_RenderClear(m_renderer);

        // Foreground
        SDL_Rect src, dest;
        src.w = dest.w = m_charWidth;
        src.h = dest.h = m_charHeight;
        for(auto i = 0; i < m_conWidth * m_conHeight; i++) {
            src.x = (m_conBuffer[i] % m_charCountX) * m_charWidth;
            src.y = (m_conBuffer[i] / m_charCountY) * m_charHeight;
            dest.x = (i % m_conWidth) * m_charWidth;
            dest.y = (i / m_conWidth) * m_charHeight;

            SDL_RenderCopy(m_renderer, m_fontTex, &src, &dest);
        }
    }

    bool Console::GetConsoleEnabled() const {
        return m_consoleEnabled;
    }

    bool Console::LoadFont(const std::string &filename) {
        auto newFont = IMG_Load(filename.c_str());
        if(!newFont) return false;

        if(m_fontSurf != nullptr) SDL_FreeSurface(m_fontSurf);
        m_fontSurf = newFont;

        if(m_fontTex != nullptr) SDL_DestroyTexture(m_fontTex);
        m_fontTex = SDL_CreateTextureFromSurface(m_renderer, m_fontSurf);
        SDL_SetTextureBlendMode(m_fontTex, SDL_BLENDMODE_ADD);

        m_charWidth = m_fontSurf->w / m_charCountX;
        m_charHeight = m_fontSurf->h / m_charCountY;
        Resize();

        return true;
    }

    SdlUiEvent Console::ProcessEvents() {
        SDL_Event ev;
        auto uiEvent = SdlUiEvent::None;

        while(SDL_PollEvent(&ev)) {
            switch(ev.type) {
                case SDL_KEYDOWN:
                    if(ev.key.keysym.scancode == m_consoleKey)
                        uiEvent = SdlUiEvent::ToggleConsole;
                    break;

                case SDL_QUIT:
                    uiEvent = SdlUiEvent::Quit;
                    break;
            }
        }

        return uiEvent;
    }

    void Console::Resize() {
        if(m_charWidth == 0 || m_charHeight == 0) return;

        delete m_conBuffer;
        delete m_videoBuffer;

        m_wndWidth = m_video->GetWidth() * m_video->GetGameScale();
        m_wndHeight = m_video->GetHeight() * m_video->GetGameScale();
        m_conWidth = m_wndWidth / m_charWidth;
        m_conHeight = m_wndHeight / m_charHeight;

        m_conBuffer = new uint8_t[m_conWidth * m_conHeight];
        m_videoBuffer = new uint8_t[m_wndWidth * m_wndHeight * m_vidFmt.pixelWidth];

        Clear();
    }

    void Console::Run() {
        m_video->BeginDrawConsole();
        Draw();
        m_video->EndDrawConsole();
        m_video->Render();
    }

    void Console::SetConsoleEnabled(bool enabled) {
        m_consoleEnabled = enabled;
        m_video->SetConsoleEnabled(m_consoleEnabled);
    }

    void Console::SetConsoleKey(SDL_Scancode scancode) {
        m_consoleKey = scancode;
    }

    void Console::SetConsoleKey(const std::string &binding) {
        auto scancode = SDL_GetScancodeFromName(binding.c_str());
        SetConsoleKey(scancode);
    }

    void Console::ToggleConsole() {
        if(m_firstUse) {
            Resize();
            m_firstUse = false;
        }
        SetConsoleEnabled(!m_consoleEnabled);
    }
}
