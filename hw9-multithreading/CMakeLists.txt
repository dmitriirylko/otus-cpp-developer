cmake_minimum_required(VERSION 3.15)

project(async)

set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 17)

add_library(async SHARED
    async.cpp
    async.h
    parser.cpp
    parser.h
    consolelogger.cpp
    consolelogger.h
    waitingqueue.h
    asyncdefs.h
    filelogger.cpp
    filelogger.h
)

add_executable(async_cli main.cpp)

target_link_libraries(async_cli async)