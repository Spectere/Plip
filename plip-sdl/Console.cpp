/* Console.cpp
 *
 * A Quake-style console implementation.
 */

#include "Console.h"

namespace PlipSdl {
    Console::Console(SdlWindow *wnd) {
        m_video = wnd;
        m_vidFmt = Plip::PlipVideo::GetFormatInfo(m_video->GetFormat());
    }

    void Console::Clear() {
        for(auto i = 0; i < m_width * m_height; i++)
            m_vidFmt.plot(m_videoBuffer, i, 0, 0, 0, 0xC0);
    }

    bool Console::GetConsoleEnabled() const {
        return m_consoleEnabled;
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
        delete m_videoBuffer;

        m_width = m_video->GetWidth() * m_video->GetGameScale();
        m_height = m_video->GetHeight() * m_video->GetGameScale();
        m_videoBuffer = new uint8_t[m_width * m_height * m_vidFmt.pixelWidth];
        Clear();
    }

    void Console::Run() {
        m_video->BeginDrawConsole();
        m_video->DrawConsole(m_videoBuffer);
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
