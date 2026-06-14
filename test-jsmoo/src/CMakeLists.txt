add_executable(${jsmoo_test_name}
    JsmooMain.cpp
                                        IRunner.h
                                        Runner.h
                                        RunnerCpu.h
                                        RunnerCpuError.h
                                        RunnerMemoryError.h
                                        RunnerTest.h
                                        RunnerTestCycle.h
                                        RunnerTestResult.h
                                        RunnerTestMemory.h
                                        RunnerTestState.h
                                        TestableCpu.h

                                        Cpu/MockSharpSm83.h

    RunnerSharpSm83.cpp                 RunnerSharpSm83.h
)

target_include_directories(${jsmoo_test_name}
    PRIVATE
        ${CXXOPTS_DIR}/include
        ${CMAKE_SOURCE_DIR}/libplip
)

target_link_libraries(${jsmoo_test_name}
    plip
    nlohmann_json::nlohmann_json
)
