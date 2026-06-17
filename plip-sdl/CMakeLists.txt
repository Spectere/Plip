add_executable(${gui_name}
    main.cpp
    Config.cpp                          Config.h
    Game.cpp                            Game.h
    Gui.cpp                             Gui.h
                                        GuiState.h
                                        NullAudio.h
                                        PlipUiEvent.h

    Sdl/SdlAudio.cpp                    Sdl/SdlAudio.h
    Sdl/SdlEvent.cpp                    Sdl/SdlEvent.h
    Sdl/SdlWindow.cpp                   Sdl/SdlWindow.h

    # Dear ImGui
    ${imgui_SOURCE_DIR}/imgui.cpp                               ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
                                                                ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
                                                                ${imgui_SOURCE_DIR}/imstb_rectpack.h
                                                                ${imgui_SOURCE_DIR}/imstb_textedit.h
                                                                ${imgui_SOURCE_DIR}/imstb_truetype.h

    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.h
)

target_include_directories(${gui_name}
    PRIVATE
        ${CXXOPTS_DIR}/include

    PUBLIC
        SDL3::SDL3

        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(${gui_name}
    plip
    SDL3::SDL3
)
