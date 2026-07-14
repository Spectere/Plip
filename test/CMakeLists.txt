include(CTest)
include(Catch)

add_executable(${test_name}
    # Common
    Cpu/MockCpu.h

    # Sharp SM83
    Cpu/SharpSm83/SharpSm83Common.h
    Cpu/SharpSm83/SharpSm83.cpp
)

target_include_directories(${test_name}
    PRIVATE ${CMAKE_SOURCE_DIR}/libplip
)

target_link_libraries(${test_name} PRIVATE
    plip
    Catch2::Catch2WithMain
)

catch_discover_tests(${test_name})
