cmake_minimum_required(VERSION 3.16)
set(product_name "Plip")
set(gui_name "plip-sdl")
set(lib_name "plip")
set(test_name "plip-test")

project(${gui_name})
set(CMAKE_CXX_STANDARD 17)
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

# Catch2
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        914aeecfe23b1e16af6ea675a4fb5dbd5a5b8d0a  # v3.8.0
)
FetchContent_MakeAvailable(catch2)

# cxxopts
set(CXXOPTS_DIR ${CMAKE_BINARY_DIR}/lib/cxxopts)

FetchContent_Populate(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        4bf61f08697b110d9e3991864650a405b3dd515d  # v3.2.1
    SOURCE_DIR     ${CXXOPTS_DIR}
)

# Dear ImGui
# Modified from: https://gist.github.com/jeffamstutz/992723dfabac4e3ffff265eb71a24cd9
set(IMGUI_DIR ${CMAKE_BINARY_DIR}/lib/imgui)

FetchContent_Populate(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        dbb5eeaadffb6a3ba6a60de1290312e5802dba5a  # v1.91.8
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

# SDL3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        877399b2b2cf21e67554ed9046410f268ce1d1b2  # 3.2.10
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)


###########
# libplip #
###########
include(TestBigEndian)
TEST_BIG_ENDIAN(BIG_ENDIAN)

add_custom_target(
    GENERATE_LIB_VERSION_HEADER
    COMMAND ${CMAKE_COMMAND}
    -Dcmake_root="${CMAKE_CURRENT_SOURCE_DIR}"
    -Dsrc_root="${CMAKE_CURRENT_SOURCE_DIR}/libplip"
    -DPRODUCT_NAME="${product_name}"
    -DBIN_NAME="${lib_name}"
    -DVERSION_FILE="PlipVersion.h"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/version.cmake"
)

add_library(${lib_name}
                                                    libplip/DebugValue.h
                                                    libplip/PlipEmulationException.h
                                                    libplip/PlipError.h
                                                    libplip/PlipInitializationException.h
    libplip/PlipInstance.cpp                        libplip/PlipInstance.h
                                                    libplip/PlipInvalidOpcodeException.h
    libplip/PlipIo.cpp                              libplip/PlipIo.h
                                                    libplip/PlipKeyValuePairCollection.h
                                                    libplip/PlipSupport.h
    libplip/PlipUtility.cpp                         libplip/PlipUtility.h

                                                    libplip/Audio/PlipAudio.h

    libplip/Cpu/PlipCpu.cpp                         libplip/Cpu/PlipCpu.h

    libplip/Input/PlipInput.cpp                     libplip/Input/PlipInput.h
    libplip/Input/PlipInputDefinition.cpp           libplip/Input/PlipInputDefinition.h

                                                    libplip/Memory/PlipMemory.h
    libplip/Memory/PlipMemoryMap.cpp                libplip/Memory/PlipMemoryMap.h
    libplip/Memory/PlipMemoryRam.cpp                libplip/Memory/PlipMemoryRam.h
    libplip/Memory/PlipMemoryRom.cpp                libplip/Memory/PlipMemoryRom.h

    libplip/Video/PlipVideo.cpp                     libplip/Video/PlipVideo.h
                                                    libplip/Video/PlipVideoException.h

    ########
    # CPUs #
    ########

    # CHIP-8
    libplip/Cpu/Chip8/Chip8.cpp                     libplip/Cpu/Chip8/Chip8.h
    libplip/Cpu/Chip8/Chip8.Ops.cpp

    # Sharp LR35902
    libplip/Cpu/SharpLr35902/SharpLr35902.cpp       libplip/Cpu/SharpLr35902/SharpLr35902.h
                                                    libplip/Cpu/SharpLr35902/SharpLr35902Registers.h
    libplip/Cpu/SharpLr35902/SharpLr35902.Ops.cpp


    #########
    # Cores #
    #########

    # CHIP-8
    libplip/Core/PlipCore.cpp                       libplip/Core/PlipCore.h
    libplip/Core/Chip8/Chip8Instance.cpp            libplip/Core/Chip8/Chip8Instance.h

    # GameBoy
                                                    libplip/Core/GameBoy/DmaState.h
    libplip/Core/GameBoy/GameBoyInstance.cpp        libplip/Core/GameBoy/GameBoyInstance.h
    libplip/Core/GameBoy/GameBoyInstance.Video.cpp
    libplip/Core/GameBoy/GameBoyIoRegisters.cpp     libplip/Core/GameBoy/GameBoyIoRegisters.h
    libplip/Core/GameBoy/GameBoyMapper.cpp          libplip/Core/GameBoy/GameBoyMapper.h
                                                    libplip/Core/GameBoy/GameBoyModel.h
    libplip/Core/GameBoy/Mbc2Ram.cpp                libplip/Core/GameBoy/Mbc2Ram.h
                                                    libplip/Core/GameBoy/MBC_Type.h
                                                    libplip/Core/GameBoy/PPU_Mode.h
                                                    libplip/Core/GameBoy/PPU_OutputStage.h
)

if(BIG_ENDIAN)
    target_compile_definitions(${lib_name} PRIVATE CORE_BIG_ENDIAN)
endif()

add_dependencies(${lib_name} GENERATE_LIB_VERSION_HEADER)


########
# plip #
########
add_executable(${gui_name}
    plip-sdl/main.cpp
    plip-sdl/Config.cpp                 plip-sdl/Config.h
    plip-sdl/Game.cpp                   plip-sdl/Game.h
    plip-sdl/Gui.cpp                    plip-sdl/Gui.h
                                        plip-sdl/GuiState.h
                                        plip-sdl/NullAudio.h
                                        plip-sdl/PlipUiEvent.h

    plip-sdl/Sdl/SdlAudio.cpp           plip-sdl/Sdl/SdlAudio.h
    plip-sdl/Sdl/SdlEvent.cpp           plip-sdl/Sdl/SdlEvent.h
    plip-sdl/Sdl/SdlWindow.cpp          plip-sdl/Sdl/SdlWindow.h

    # Dear ImGui
    ${imgui_SOURCE_DIR}/imgui.cpp                              ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h

    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp           ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp   ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.h
)

target_include_directories(${gui_name}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/libplip
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


#############
# plip-test #
#############
include(CTest)
include(Catch)

add_executable(${test_name}
    # Common
    test/Cpu/MockCpu.h

    # CHIP-8
    test/Cpu/Chip8/Chip8Ops.cpp

    # Sharp LR35902
    test/Cpu/SharpLr35902/SharpLr35902Common.h
    test/Cpu/SharpLr35902/SharpLr35902.Arith8.cpp
    test/Cpu/SharpLr35902/SharpLr35902.Arith16.cpp
    test/Cpu/SharpLr35902/SharpLr35902.Bits.cpp
    test/Cpu/SharpLr35902/SharpLr35902.Interrupts.cpp
    test/Cpu/SharpLr35902/SharpLr35902.Invalid.cpp
    test/Cpu/SharpLr35902/SharpLr35902.Jumps.cpp
    test/Cpu/SharpLr35902/SharpLr35902.Ld8.cpp
    test/Cpu/SharpLr35902/SharpLr35902.Ld16.cpp
    test/Cpu/SharpLr35902/SharpLr35902.MiscControl.cpp
)

target_include_directories(${test_name}
    PRIVATE ${CMAKE_SOURCE_DIR}/libplip
)

target_link_libraries(${test_name} PRIVATE
    plip
    Catch2::Catch2WithMain
)

catch_discover_tests(${test_name})
