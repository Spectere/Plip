add_executable(${frontend_test_name}
                                        MockPlip/MockAudio.h
    MockPlip/MockVideo.cpp              MockPlip/MockVideo.h

                                        Models/FrontendConfig.h
                                        Models/MemoryValue.h
                                        Models/Results.h
                                        Models/ResultType.h
                                        Models/StopEvent.h
                                        Models/Subtest.h
                                        Models/TestResults.h
                                        Models/UnitTest.h
                                        Models/ValueType.h

    FrontendMain.cpp
                                        PlipRunner.h
    ResultComparator.cpp                ResultComparator.h
    ResultWriter.cpp                    ResultWriter.h

    ${lodepng_SOURCE_DIR}/lodepng.cpp
)

target_include_directories(${frontend_test_name}
    PUBLIC
        ${lodepng_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/libplip
)

target_link_libraries(${frontend_test_name}
    plip
    nlohmann_json::nlohmann_json
)
