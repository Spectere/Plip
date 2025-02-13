// Gui.cpp
//
// Handles initializing, populating, and rendering the ImGui overlay.

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "Gui.h"

using PlipSdl::Gui;

Gui::Gui(const SdlWindow* sdlWindow) : m_renderer(sdlWindow->GetRenderer()) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    const auto imguiIo = &ImGui::GetIO();
    imguiIo->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
                         |  ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplSDL3_InitForSDLRenderer(sdlWindow->GetWindow(), m_renderer);
    ImGui_ImplSDLRenderer3_Init(m_renderer);
}

Gui::~Gui() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

bool Gui::GetEnabled() const {
    return m_enabled;
}

void Gui::NewFrame() const {
    if(!m_enabled) return;

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
}

void Gui::Render() const {
    if(!m_enabled) return;

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
}

void Gui::SendEvent(const SDL_Event &event) const {
    if(!m_enabled) return;

    ImGui_ImplSDL3_ProcessEvent(&event);
}

void Gui::SetEnabled(const bool enable) {
    m_enabled = enable;
}
