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

# add_executable(soft_heap main.cpp test/node_tests.cpp test/tree_tests.cpp test/soft_heap_tests.cpp)
# target_link_libraries(soft_heap PRIVATE gtest_main gmock gtest)
# target_include_directories(soft_heap PRIVATE "include" "src")

add_executable(soft_heap benchmark.cpp)
target_link_libraries(soft_heap PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(soft_heap PRIVATE "include" "src")

# Compiler Properties
target_compile_features(soft_heap PRIVATE cxx_std_20)
target_compile_options(soft_heap PRIVATE -O3 -Wall)

# Compiler Properties
# target_compile_features(soft_heap_bench PRIVATE cxx_std_20)
# target_compile_options(soft_heap_bench PRIVATE -g -Wall)

# Pass to clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(BENCHMARK_FORMAT json)
# set(BENCHMARK_OUT "../bench.json")

# gtest_discover_tests(soft_heap)

# enable_testing()