cmake_minimum_required(VERSION 3.12)

project(Birb3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BIRB_WINDOWS "Disable Linux specific things")
option(BIRB_PROFILER "Enable the profiling with microprofile" ON)

if (BIRB_WINDOWS)
	add_definitions(-DBIRB_PLATFORM_WINDOWS)
else()
	add_definitions(-DBIRB_PLATFORM_LINUX)
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

include_directories(./engine/core/include
	./engine/rendering/include
	./engine/widgets/include)

# Include stb libraries
include_directories(./vendor/stb)

# Include doctest
include_directories(./vendor/doctest)

# Include EnTT
include_directories(${CMAKE_SOURCE_DIR}/vendor/entt/single_include/entt)


# Build GLFW
set(GLFW_PATH "${CMAKE_SOURCE_DIR}/vendor/glfw")
include_directories("${GLFW_PATH}/include")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate GLFW installation target")
add_subdirectory("${GLFW_PATH}")

# Build ImGui
set(IMGUI_PATH "${CMAKE_SOURCE_DIR}/vendor/imgui")
include_directories(
	${IMGUI_PATH}
	${IMGUI_PATH}/backends
	${IMGUI_PATH}/misc/cpp)

add_library(imgui
	${IMGUI_PATH}/imgui.cpp
	${IMGUI_PATH}/imgui_demo.cpp
	${IMGUI_PATH}/imgui_draw.cpp
	${IMGUI_PATH}/imgui_tables.cpp
	${IMGUI_PATH}/imgui_widgets.cpp
	${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
	${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
	${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp)

# Build glad
set(GLAD_PATH "${CMAKE_SOURCE_DIR}/vendor/glad")
include_directories(${GLAD_PATH}/include)
add_library(glad ${GLAD_PATH}/src/gl.c)

# Build glm
include_directories("${CMAKE_SOURCE_DIR}/vendor/glm")
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/glm")

# Build microprofile
add_subdirectory(vendor/microprofile)
include_directories(vendor/microprofile)
if (BIRB_PROFILER)
	add_compile_definitions(MICROPROFILE_ENABLED=1)
else()
	add_compile_definitions(MICROPROFILE_ENABLED=0)
endif()

# Build with all kinds of warnings enabled because programmers usually don't
# know what they are doing
add_compile_options(-pedantic -Wall -Wextra -Wcast-align
	-Wdisabled-optimization -Wformat=2
	-Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs
	-Werror -Wno-unused)

# Build the engine library
add_subdirectory(engine)

# Build demos
add_subdirectory(demos)

# Build tests
add_subdirectory(tests)

# Build the playground project
add_subdirectory(playground)

# Build documentation
find_package(Doxygen)
if (Doxygen_FOUND)
	set(DOXYGEN_HTML_EXTRA_STYLESHEET "${PROJECT_SOURCE_DIR}/vendor/doxygen-awesome-css/doxygen-awesome.css")
	set(DOXYGEN_DISABLE_INDEX NO)
	set(DOXYGEN_GENERATE_TREEVIEW YES)
	set(DOXYGEN_FULL_SIDEBAR NO)
	set(DOXYGEN_HTML_COLORSTYLE LIGHT)
	set(DOXYGEN_CALL_GRAPH YES)
	set(DOXYGEN_CALLER_GRAPH YES)
	set(DOXYGEN_EXTRACT_PRIVATE YES)
	doxygen_add_docs(docs "${PROJECT_SOURCE_DIR}/engine" COMMENT "Generate
	documentation from code comments")
endif()
