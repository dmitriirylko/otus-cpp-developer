cmake_minimum_required(VERSION 3.15)

project(bayan)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS filesystem program_options REQUIRED) 
    
add_executable(bayan
    main.cpp
    config.cpp
)
    
set_target_properties(bayan PROPERTIES CXX_STANDARD 17)
target_link_libraries(bayan Boost::filesystem Boost::program_options)