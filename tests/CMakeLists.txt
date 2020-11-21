cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic-errors")

add_subdirectory("${EXTERN_DIR}/googletest" "${BUILD_DIR}/external/googletest")

set(TEST_FILES
    Core/Vector/vector.test.cpp
    Core/Matrix/matrix.test.cpp
    Core/Quaternion/quaternion.test.cpp
    util/type_traits.test.cpp
    util/util.test.cpp
)

# link test files against gtest_main
add_executable(tests ${TEST_FILES})
target_link_libraries(tests gtest gmock gtest_main)
add_test(NAME example_test COMMAND tests)

target_include_directories(tests PUBLIC
    "${SRC_DIR}/mathlib"
)