cmake_minimum_required(VERSION 3.16)
set(product_name "Plip")
set(gui_name "plip-sdl")
set(lib_name "plip")

project(${gui_name})
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

if(UNIX)
    add_compile_definitions(UNIX)
elseif(WIN32)
    add_compile_definitions(WIN32)
endif()

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
        libplip/PlipInstance.cpp
        libplip/PlipIo.cpp

        libplip/Core/PlipCore.cpp
        libplip/Core/Chip8/Chip8Instance.cpp

        libplip/Input/PlipInput.cpp
        libplip/Input/PlipInputDefinition.cpp

        libplip/Memory/PlipMemoryMap.cpp
        libplip/Memory/PlipMemoryRam.cpp
        libplip/Memory/PlipMemoryRom.cpp

        libplip/Video/PlipVideo.cpp
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
        plip-sdl/Config.cpp

        plip-sdl/SDL/SdlAudio.cpp
        plip-sdl/SDL/SdlEvent.cpp
        plip-sdl/SDL/SdlWindow.cpp
)

if(UNIX)
    target_sources(${gui_name} PRIVATE
            plip-sdl/Timer/TimerPosix.cpp
    )
else()
    target_sources(${gui_name} PRIVATE
            plip-sdl/Timer/TimerSdl.cpp
    )
endif()

target_include_directories(${gui_name}
        PRIVATE ${CMAKE_SOURCE_DIR}/libplip
        PUBLIC ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${gui_name}
        plip
        ${SDL2_LIBRARIES}
)
