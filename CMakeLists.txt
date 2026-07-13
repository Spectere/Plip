cmake_minimum_required(VERSION 4.0)

set(product_name "Plip")
set(gui_name "plip-sdl")
set(lib_name "plip")
set(test_name "plip-test")
set(jsmoo_test_name "plip-test-jsmoo")
set(frontend_test_name "plip-test-frontend")

project(${gui_name})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

if(UNIX)
    add_compile_definitions(UNIX)
elseif(WIN32)
    add_compile_definitions(WIN32)
endif()

#
# Dependencies
#

# cxxopts
set(CXXOPTS_DIR ${CMAKE_BINARY_DIR}/lib/cxxopts)

FetchContent_Populate(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        44380e5a44706ab7347f400698c703eb2a196202  # v3.3.1
    SOURCE_DIR     ${CXXOPTS_DIR}
)

# Dear ImGui
# Modified from: https://gist.github.com/jeffamstutz/992723dfabac4e3ffff265eb71a24cd9
set(IMGUI_DIR ${CMAKE_BINARY_DIR}/lib/imgui)

FetchContent_Populate(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        bf75bfec48fc00f532af8926130b70c0e26eb099  # v1.92.3
    SOURCE_DIR     ${IMGUI_DIR}
)

add_library(imgui STATIC
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui.h

    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_link_libraries(imgui PUBLIC
    SDL3::SDL3
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_compile_definitions(imgui PUBLIC
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS
)

# json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        55f93686c01528224f448c19128836e7df245f72  # v3.12.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(json)

# LodePNG
FetchContent_Declare(
    lodepng
    GIT_REPOSITORY https://github.com/lvandeve/lodepng.git
    GIT_TAG        ed6fe5825c6a4fbb7f58ab35a4231c7543cd452a
)
FetchContent_MakeAvailable(lodepng)

# SDL3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        a96677bdf6b4acb84af4ec294e5f60a4e8cbbe03  # 3.2.22
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)


#
# Targets
#

# libplip
add_subdirectory(libplip)

# plip-sdl
add_subdirectory(plip-sdl)

# plip-test-jsmoo
add_subdirectory(test-jsmoo/src)

# plip-test-frontend
add_subdirectory(test-frontend)
