cmake_minimum_required(VERSION 3.5)

project(soft_heap
    VERSION 1.0.0
    LANGUAGES CXX)


# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL
    https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
include(GoogleTest)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(soft_heap src/test.cpp src/tree.hpp)

target_link_libraries(soft_heap PRIVATE gtest_main)
target_include_directories(soft_heap PRIVATE "/usr/local/include/" . "include")

# Compiler Properties
target_compile_features(soft_heap PRIVATE cxx_std_20)
target_compile_options(soft_heap PRIVATE -g -Wall)

# Pass to clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

gtest_discover_tests(soft_heap)

enable_testing()
