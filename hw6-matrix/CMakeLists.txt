cmake_minimum_required(VERSION 3.15)

project(matrix)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD 17)