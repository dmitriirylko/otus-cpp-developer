cmake_minimum_required(VERSION 3.15)

find_package(Git)

if(GIT_EXECUTABLE)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE PROJECT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

if(PROJECT_VERSION STREQUAL "")
  set(PROJECT_VERSION 0.0.0)
  message(WARNING "Failed to determine version from Git tags. Using default version \"${PROJECT_VERSION}\".")
endif()

message("Version is \"${PROJECT_VERSION}\".")

project(allocator VERSION ${PROJECT_VERSION})

add_executable(allocator main.cpp)

set_target_properties(allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

add_executable(allocatorBenchmark benchmark.cpp)

set_target_properties(allocatorBenchmark PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(LINUX)
# -- install command was introduced in 3.15 version of cmake
# -- we can use sudo make install from build folder
    install(TARGETS allocator RUNTIME DESTINATION bin)
    set(CPACK_GENERATOR DEB)
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    set(CPACK_PACKAGE_CONTACT dmitriirylko@gmail.com)
    include(CPack)
endif()