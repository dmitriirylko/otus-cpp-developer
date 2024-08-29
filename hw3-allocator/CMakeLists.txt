cmake_minimum_required(VERSION 3.15)

project(allocator)

add_executable(allocator main.cpp)

set_target_properties(allocator PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

add_executable(allocatorBenchmark benchmark.cpp)

set_target_properties(allocatorBenchmark PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)