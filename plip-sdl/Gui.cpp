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

    if(!ImGui::Begin("Debug", &m_enabled, ImGuiWindowFlags_None) || !m_enabled) {
        State.PerformRead = false;
        ImGui::End();
        return event;
    }

    // Emulator controls.
    if(ImGui::Checkbox("Pause", &State.PauseCore)) {
        event = State.PauseCore ? PlipUiEvent::PauseEnable : PlipUiEvent::PauseDisable;
    }

    if(State.PauseCore) {
        ImGui::SameLine();
        if(ImGui::Button("Step")) {
            event = PlipUiEvent::Step;
        }
    }

    // Memory viewer/editor.
    if(ImGui::CollapsingHeader("Memory")) {
        static int address = 0;
        static int value = 0;

        static int valueType = 1;

        State.PerformRead = true;

        ImGui::InputInt("Addr", &address, 1, 16, ImGuiInputTextFlags_CharsHexadecimal);

        if(valueType == 0) {
            ImGui::InputInt("Val", &value, 1, 10, ImGuiInputTextFlags_CharsDecimal);
        } else {
            ImGui::InputInt("Val", &value, 1, 16, ImGuiInputTextFlags_CharsHexadecimal);
        }

        ImGui::Text("Value in");
        ImGui::SameLine();
        ImGui::RadioButton("dec", &valueType, 0);
        ImGui::SameLine();
        ImGui::RadioButton("hex", &valueType, 1);

        if(ImGui::Button("Read")) {
            State.ReadAddress = address;
        }
        ImGui::SameLine();
        if(ImGui::Button("Write")) {
            State.WriteAddress = address;
            State.WriteValue = static_cast<uint8_t>(value & 0xFF);
            State.PerformWrite = true;
        }

        constexpr auto memoryAddress = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        constexpr auto memoryNormal = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        constexpr auto memoryHighlight = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        for(auto y = 0; y < State.MemoryDisplayRows; y++) {
            ImGui::TextColored(
                memoryAddress,
                "%.8X", State.MemoryDisplayBase + (State.MemoryDisplayColumns * y)
            );

            for(auto x = 0; x < State.MemoryDisplayColumns; x++) {
                const auto currentAddress = State.MemoryDisplayBase + (y * State.MemoryDisplayColumns) + x;

                ImGui::SameLine();
                ImGui::TextColored(
                    currentAddress == State.ReadAddress ? memoryHighlight : memoryNormal,
                    "%.2X", State.MemoryContents[(y * State.MemoryDisplayColumns) + x]
                );
            }
        }
    } else {
        State.PerformRead = false;
    }

    // Draw core debug information.
    for(const auto &[section, values] : m_debugInfo) {
        if(!ImGui::CollapsingHeader(section.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) continue;

        if(ImGui::BeginTable(section.c_str(), 2)) {
            for(const auto &[key, value] : values) {
                ImGui::TableNextRow();

                ImGui::TableNextColumn(); ImGui::Text(key.c_str());
                ImGui::TableNextColumn();
                switch(value.Type) {
                    case Plip::DebugValueType::Float32Le:
                    case Plip::DebugValueType::Float32Be:
                    case Plip::DebugValueType::Float64Le:
                    case Plip::DebugValueType::Float64Be:
                        ImGui::Text("%f", value.ValueFloat);
                        break;

                    case Plip::DebugValueType::Int8:
                        ImGui::Text("0x%.2X (%d)", value.ValueInt, value.ValueInt);
                        break;

                    case Plip::DebugValueType::Int16Le:
                    case Plip::DebugValueType::Int16Be:
                        ImGui::Text("0x%.4X (%d)", value.ValueInt, value.ValueInt);
                        break;

                    case Plip::DebugValueType::Int32Le:
                    case Plip::DebugValueType::Int32Be:
                        ImGui::Text("0x%.8X (%d)", value.ValueInt, value.ValueInt);
                        break;

                    case Plip::DebugValueType::Int64Le:
                    case Plip::DebugValueType::Int64Be:
                        ImGui::Text("0x%.16X (%d)", value.ValueInt, value.ValueInt);
                        break;

                    case Plip::DebugValueType::String:
                        ImGui::Text("%s", value.ValueString.c_str());
                        break;

                    case Plip::DebugValueType::Unknown:
                    default:
                        ImGui::Text("???");
                        break;
                }
            }

            ImGui::EndTable();
        }
    }

    ImGui::End();

    return event;
}
