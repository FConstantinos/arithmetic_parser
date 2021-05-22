cmake_minimum_required(VERSION 3.16)
include(GNUInstallDirs)

project(AM_exercise)

set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

########################################################################
## Targets
########################################################################

add_executable(am_parser
            src/am_parser.cc
            src/AMParser.cc
            src/AMScanner.cc
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        )

install(TARGETS am_parser
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

set_target_properties(am_parser PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

########################################################################
## Sub-directory Targets
########################################################################
option(ENABLE_TEST "Build tests" OFF)

if (ENABLE_TEST)
    add_subdirectory(unittests)
endif(ENABLE_TEST)

