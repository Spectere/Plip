/* Console.cpp
 *
 * A Quake-style console implementation.
 */

#include <list>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>

#include "Console.h"
#include "StringUtil.h"

namespace PlipSdl {
    Console::Console(SdlWindow *wnd) {
        m_video = wnd;
        m_renderer = wnd->GetRenderer();
        m_vidFmt = Plip::PlipVideo::GetFormatInfo(m_video->GetFormat());
        IMG_Init(0);

        RegisterInternalCommands();
    }

    Console::~Console() {
        IMG_Quit();
    }

    void Console::Clear() {
        for(auto i = 0; i < m_conWidth * m_conHeight; i++)
            m_conBuffer[i] = 0x20;

        m_cursor = 0;
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

    void Console::EnterPressed() {
        Write("\n");

        if(m_input.empty()) {
            NewCommand();
            return;
        }

        std::string str(m_input.cbegin(), m_input.cend());
        if(StringUtil::Trim(str).empty()) {
            NewCommand();
            return;
        }

        std::vector<std::string> cmdLine(StringUtil::Split(str, ' '));
        auto cmd = StringUtil::ToLower(cmdLine[0]);

        // Build a list of candidates.
        std::list<Command> candidates;
        auto len = cmd.length();
        for(const auto &it : m_commandList) {
            if(it.name.substr(0, len) != cmd) continue;
            candidates.push_back(it);
        }

        if(candidates.empty()) {
            // No candidates.
            WriteError("command not found");
        } else if(candidates.size() == 1) {
            // Only one candidate. Execute the attached function.
            candidates.cbegin()->func(this, cmdLine);
        } else {
            std::stringstream ss;
            bool first = true;
            ss << "Ambiguous command. Possible candidates: ";

            // Check for an exact match.
            for(const auto &it : candidates) {
                if(it.name != cmd) {
                    if(first) first = false;
                    else ss << ", ";
                    ss << it.name;
                    continue;
                }

                it.func(this, cmdLine);
                goto done;
            }

            // Print the list of candidates.
            WriteLine(ss.str());
        }

done:
        NewCommand();
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
            int idx;
            char ch;
            switch(ev.type) {
                case SDL_KEYDOWN:
                    if(ev.key.keysym.scancode == m_consoleKey)
                        ToggleConsole();

                    if(ev.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                        if(m_input.empty()) break;

                        m_input.pop_back();
                        m_conBuffer[--m_cursor] = 0x20;
                    }

                    if(ev.key.keysym.scancode == SDL_SCANCODE_KP_ENTER ||
                       ev.key.keysym.scancode == SDL_SCANCODE_RETURN ||
                       ev.key.keysym.scancode == SDL_SCANCODE_RETURN2) {
                            EnterPressed();
                    }
                    break;

                case SDL_TEXTINPUT:
                    idx = 0;
                    while((ch = ev.text.text[idx++]) != 0) {
                        m_input.push_back(ch);
                        Write(ch);
                    }
                    break;

                case SDL_QUIT:
                    uiEvent = SdlUiEvent::Quit;
                    break;
            }
        }

        return uiEvent;
    }

    void Console::RegisterCommand(const std::string &commandName,
                                  void (*func)(Console*, const std::vector<std::string> &args)) {
        auto lowerName = StringUtil::ToLower(commandName);

        m_commandList.push_back({ lowerName, func });
        m_commandList.sort([](const Command &first, const Command &second) {
            return first.name < second.name;
        });
    }

    void Console::RegisterInternalCommands() {
        RegisterCommand("help", [](Console *console, const std::vector<std::string> &args) {
            console->Write("valid commands: ");

            bool first = true;
            for(auto &it : console->m_commandList) {
                if(first) first = false;
                else console->Write(", ");

                console->Write(it.name);
            }
            console->WriteLine();
        });
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

    void Console::Scroll(int lines) {
        if(lines <= 0) return;
        if(lines >= m_conHeight) {
            Clear();
            m_cursor = m_conWidth * (m_conHeight - 1);
        }

        memcpy(m_conBuffer,
               m_conBuffer + (m_conWidth * lines),
               m_conWidth * m_conHeight - (m_conWidth * lines));
        memset(m_conBuffer + (m_conWidth * (m_conHeight - lines)),
               0x20, lines * m_conWidth);

        m_cursor -= m_conWidth;
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
        Initialize();
        SetConsoleEnabled(!m_consoleEnabled);

        if(m_consoleEnabled)
            SDL_StartTextInput();
        else
            SDL_StopTextInput();
    }

    void Console::Write(const char ch) {
        Initialize();
        if(ch == '\n') {
            auto row = m_cursor / m_conWidth;
            m_cursor = (row + 1) * m_conWidth;
        } else {
            m_conBuffer[m_cursor++] = ch;
        }

        if(m_cursor >= m_conWidth * m_conHeight)
            Scroll();
    }

    void Console::Write(const std::string &str) {
        for(auto ch : str)
            Write(ch);
    }
}
