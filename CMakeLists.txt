cmake_minimum_required(VERSION 3.12)

project(Birb3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

include_directories(./engine/include ./vendor)
# add_executable(${PROJECT_NAME} ./src/main.cpp)
# install(TARGETS ${PROJECT_NAME})

# Build the engine library
add_subdirectory(engine)

# Build demos
add_subdirectory(demos)

# Build tests
add_subdirectory(tests)
