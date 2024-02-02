cmake_minimum_required(VERSION 3.12)

project(Birb3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

include_directories(./engine/core/include)
# add_executable(${PROJECT_NAME} ./src/main.cpp)
# install(TARGETS ${PROJECT_NAME})

# Build with all kinds of warnings enabled because programmers usually don't
# know what they are doing
add_compile_options(-pedantic -Wall -Wextra -Wcast-align
	-Wdisabled-optimization -Wformat=2
	-Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs
	-Wnoexcept -Woverloaded-virtual -Wstrict-null-sentinel -Werror -Wno-unused)

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


# Build the engine library
add_subdirectory(engine)

# Build demos
add_subdirectory(demos)

# Build tests
add_subdirectory(tests)
