cmake_minimum_required(VERSION 3.5)

project(soft_heap
    VERSION 1.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
include(GoogleTest)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)

FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
)

FetchContent_MakeAvailable(googletest googlebenchmark)

# Unit Testing Executable
add_executable(soft_heap_test main.cpp test/node_tests.cpp test/tree_tests.cpp test/soft_heap_tests.cpp)
target_link_libraries(soft_heap_test PRIVATE gtest_main gmock gtest)
target_include_directories(soft_heap_test PRIVATE "include" "src")

# Compiler Properties for Unit Testing
target_compile_features(soft_heap_test PRIVATE cxx_std_20)
target_compile_options(soft_heap_test PRIVATE -O3 -Wall -Wextra)

# Benchmark Executable
add_executable(soft_heap_bench benchmark.cpp)
target_link_libraries(soft_heap_bench PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(soft_heap_bench PRIVATE "include" "src")
set_property(TARGET soft_heap_bench PROPERTY CMAKE_BUILD_TYPE Release)

# Compiler Properties for Benchmarking
target_compile_features(soft_heap_bench PRIVATE cxx_std_20)
target_compile_options(soft_heap_bench PRIVATE -O3 -Wall)

# Pass to clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

gtest_discover_tests(soft_heap_test)

enable_testing()
