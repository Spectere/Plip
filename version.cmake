cmake_minimum_required(VERSION 3.15)

find_package(Git)
if(GIT_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY "${cmake_root}"
            OUTPUT_VARIABLE GIT_BRANCH
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY "${cmake_root}"
            OUTPUT_VARIABLE GIT_REVISION
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags
            WORKING_DIRECTORY "${cmake_root}"
            OUTPUT_VARIABLE GIT_TAG
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "${APP_TITLE}: ${GIT_BRANCH}@${GIT_REVISION}; tag: ${GIT_TAG}")
else()
    message(STATUS "Git package not found. Unable to fetch version information.")
endif()

configure_file(${cmake_root}/version.h.in ${src_root}/${VERSION_FILE})
