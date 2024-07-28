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

# project(ip_filter VERSION ${PROJECT_VERSION})

project(ip_filter)

option(WITH_GTEST "Whether to build Boost test" ON)

# configure_file(version.h.in version.h)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    reader.cpp
    ipaddrpool.cpp
)

# add_library(helloworld lib.cpp)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

# include_directories(${CMAKE_BINARY_DIR}) 

# target_link_libraries(helloworld_cli PRIVATE helloworld)

if(WITH_GTEST)
    find_package(GTest)

    add_executable(test_ip_filter test_ip_filter.cpp)

    set_target_properties(test_ip_filter PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
    )

    include_directories(${GTEST_INCLUDE_DIRS}) 

    target_link_libraries(test_ip_filter PRIVATE ${GTEST_BOTH_LIBRARIES})
endif()

# if(LINUX)
# # -- install command was introduced in 3.15 version of cmake
# # -- we can use sudo make install from build folder
#     install(TARGETS helloworld_cli RUNTIME DESTINATION bin)
#     set(CPACK_GENERATOR DEB)
#     set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
#     set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
#     set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
#     set(CPACK_PACKAGE_CONTACT dmitriirylko@gmail.com)
#     include(CPack)
# endif()

# if(WITH_BOOST_TEST)
#     enable_testing()
#     add_test(test_version test_version)
# endif()