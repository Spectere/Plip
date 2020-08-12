/* Console.h
 *
 * A Quake-style console implementation.
 */

#pragma once

#include "SDL/SdlEvent.h"
#include "SDL/SdlWindow.h"

namespace PlipSdl {
    class Console {
    public:
        explicit Console(SdlWindow *wnd);

        void Clear();
        [[nodiscard]] bool GetConsoleEnabled() const;
        SdlUiEvent ProcessEvents();
        void Resize();
        void Run();
        void SetConsoleEnabled(bool enabled);
        void SetConsoleKey(SDL_Scancode scancode);
        void SetConsoleKey(const std::string &binding);
        void ToggleConsole();

    private:
        bool m_consoleEnabled = false;
        SDL_Scancode m_consoleKey {};
        bool m_firstUse = true;
        SdlWindow *m_video;
        uint8_t *m_videoBuffer = nullptr;
        Plip::PlipVideoFormatInfo m_vidFmt {};
        int m_width = 0, m_height = 0;
    };
}
