cmake_minimum_required(VERSION 3.4.1)
project(hwemulator_native)
include_directories(
        ${CMAKE_SOURCE_DIR}/src/main/cpp/nes/include
        ${CMAKE_SOURCE_DIR}/src/main/cpp/hwemulatorbase/include
)

file(
        GLOB
        SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
)
add_library( # Sets the name of the library.
        hwemulator_native

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${SOURCES}
        )

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries( # Specifies the target library.
        hwemulator_native
        hwemulator_nes
        hwemulatorbase
        android
        ${log-lib})