cmake_minimum_required(VERSION 3.16)
set(product_name "Plip")
set(gui_name "plip-sdl")
set(lib_name "plip")

project(${gui_name})
set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)

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
        libplip/Plip.cpp
)

add_dependencies(${lib_name} GENERATE_LIB_VERSION_HEADER)

########
# plip #
########
add_executable(${gui_name}
        plip-sdl/main.cpp
        plip-sdl/SdlWindow.cpp
)

target_include_directories(${gui_name}
        PRIVATE ${CMAKE_SOURCE_DIR}/libplip
        PUBLIC ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${gui_name}
        plip
        ${SDL2_LIBRARIES}
)
