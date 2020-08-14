/* Console.h
 *
 * A Quake-style console implementation.
 */

#pragma once

#include <vector>

#include "SDL/SdlEvent.h"
#include "SDL/SdlWindow.h"

namespace PlipSdl {
    class Console {
    public:
        explicit Console(SdlWindow *wnd);
        ~Console();

        void Clear();
        void Draw();
        [[nodiscard]] bool GetConsoleEnabled() const;
        bool LoadFont(const std::string &filename);
        SdlUiEvent ProcessEvents();
        void Resize();
        void Run();
        void SetConsoleEnabled(bool enabled);
        void SetConsoleKey(SDL_Scancode scancode);
        void SetConsoleKey(const std::string &binding);
        void ToggleConsole();

    private:
        const int m_charCountX = 16;
        const int m_charCountY = 16;

        bool m_consoleEnabled = false;
        SDL_Scancode m_consoleKey {};
        bool m_firstUse = true;

        SDL_Renderer *m_renderer = nullptr;
        SdlWindow *m_video;
        uint8_t *m_videoBuffer = nullptr;
        SDL_Surface *m_fontSurf = nullptr;
        SDL_Texture *m_fontTex = nullptr;
        Plip::PlipVideoFormatInfo m_vidFmt {};

        int m_wndWidth = 0, m_wndHeight = 0;
        int m_charWidth = 0, m_charHeight = 0;
        int m_conWidth = 0, m_conHeight = 0;

        uint8_t *m_conBuffer {};
        std::vector<char> m_input {};
    };
}
