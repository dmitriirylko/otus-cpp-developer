cmake_minimum_required(VERSION 3.15)

project(matrix)

add_executable(test_matrix
    test_matrix.cpp
)

set_target_properties(test_matrix PROPERTIES CXX_STANDARD 17)

add_executable(matrix
    main.cpp
)

set_target_properties(matrix PROPERTIES CXX_STANDARD 17)