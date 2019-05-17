cmake_minimum_required(VERSION 3.4.1)
project(hwemulatorbase)

set(PREBUILD_SHARED "${PROJECT_SOURCE_DIR}/prebuild/shared/${ANDROID_ABI}")

include_directories(
        ${CMAKE_SOURCE_DIR}/src/main/cpp/common/include
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
endif ()

#------------------SHARED START----------------------
#libhwvcom
add_library(
        hwvcom
        SHARED
        IMPORTED)
set_target_properties(
        hwvcom
        PROPERTIES IMPORTED_LOCATION
        "${PREBUILD_SHARED}/libhwvcom.so")

#------------------SHARED END----------------------

file(
        GLOB
        SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/entity/*.cpp
)
add_library(
        hwemulatorbase
        SHARED
        ${SOURCES}
)

find_library(
        log-lib
        log)

target_link_libraries(
        hwemulatorbase
        hwvcom
        android
        ${log-lib}
)