cmake_minimum_required(VERSION 3.15)

project(bulk)

add_executable(bulk
    main.cpp
    source.cpp
    parser.cpp
    consolelogger.cpp
    filelogger.cpp
)

set_target_properties(bulk PROPERTIES CXX_STANDARD 17)