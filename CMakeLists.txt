cmake_minimum_required(VERSION 3.11)

project(MathLib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic-errors")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(EXTERN_DIR "${PROJECT_SOURCE_DIR}/external")
set(BUILD_DIR "${PROJECT_SOURCE_DIR}/build")

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_library(mathlib INTERFACE)

target_include_directories(mathlib INTERFACE src/)