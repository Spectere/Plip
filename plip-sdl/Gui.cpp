// Gui.cpp
//
// Handles initializing, populating, and rendering the ImGui overlay.

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "Gui.h"
#include "PlipUiEvent.h"

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
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void Gui::Render() const {
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
}

void Gui::SendEvent(const SDL_Event &event) const {
    if(!m_enabled) return;

    ImGui_ImplSDL3_ProcessEvent(&event);
}

void Gui::SetDebugInfo(std::map<std::string, std::map<std::string, Plip::DebugValue>> debugInfo) {
    m_debugInfo = std::move(debugInfo);
}

void Gui::SetEnabled(const bool enable) {
    m_enabled = enable;
}

PlipSdl::PlipUiEvent Gui::Update() {
    auto event = PlipUiEvent::None;

    if(!ImGui::Begin("Debug", &m_enabled, ImGuiWindowFlags_None)) {
        ImGui::End();
        return event;
    }

    if(ImGui::Button("Pause")) {
        event = PlipUiEvent::PauseEnable;
    }
    ImGui::SameLine();
    if(ImGui::Button("Step")) {
        event = PlipUiEvent::Step;
    }
    ImGui::SameLine();
    if(ImGui::Button("Play")) {
        event = PlipUiEvent::PauseDisable;
    }

    for(const auto &[section, values] : m_debugInfo) {
        if(!ImGui::CollapsingHeader(section.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) continue;

        if(ImGui::BeginTable(section.c_str(), 2)) {
            for(const auto &[key, value] : values) {
                ImGui::TableNextRow();

                ImGui::TableNextColumn(); ImGui::Text(key.c_str());
                ImGui::TableNextColumn(); if(value.Type == Plip::DebugValueType::Float32Be
                    || value.Type == Plip::DebugValueType::Float64Be
                    || value.Type == Plip::DebugValueType::Float32Le
                    || value.Type == Plip::DebugValueType::Float64Le) {
                    ImGui::Text("%f", value.ValueFloat);
                } else if(value.Type == Plip::DebugValueType::String) {
                    ImGui::Text("%s", value.ValueString.c_str());
                } else {
                    ImGui::Text("%d", value.ValueInt);
                }
            }

            ImGui::EndTable();
        }
    }

    ImGui::End();

    return event;
}
