cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

configure_file(version.h.in version.h)

add_executable(helloworld_cli main.cpp lib.cpp)

add_library(helloworld lib.cpp)

set_target_properties(helloworld_cli PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

include_directories(${CMAKE_BINARY_DIR}) 

target_link_libraries(helloworld_cli PRIVATE helloworld)

if(WITH_BOOST_TEST)
    set(Boost_ROOT "C:/boost_1_85_0/")
    set(Boost_USE_STATIC_LIBS        ON)
    set(Boost_USE_MULTITHREADED      ON)
    set(Boost_USE_STATIC_RUNTIME    OFF)
    
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)

    add_executable(test_version test_version.cpp)

    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
    )

    include_directories(${Boost_INCLUDE_DIRS}) 

    target_link_libraries(test_version ${Boost_LIBRARIES} helloworld)
endif()