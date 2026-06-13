add_executable(${jsmoo_test_name}
    JsmooMain.cpp
)

target_include_directories(${jsmoo_test_name}
    PRIVATE
        ${CXXOPTS_DIR}/include
        ${CMAKE_SOURCE_DIR}/libplip
)

target_link_libraries(${jsmoo_test_name}
    plip
)
