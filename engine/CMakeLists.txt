# Include shader source code
include_directories(rendering/shaders)

add_library(birb)

# Engine core
set(CORE_INC core/include)
file(GLOB CORE_SRC core/src/*.cpp)
target_sources(birb PRIVATE ${CORE_SRC})


# Rendering
file(GLOB SHADER_SRC rendering/shaders/*.glsl)
set(SHADER_SOURCE_HEADER
	"${CMAKE_SOURCE_DIR}/engine/rendering/shaders/ShaderSource.hpp")
add_custom_command(
	OUTPUT ${SHADER_SOURCE_HEADER}
	COMMAND ./generate_shader_header.sh
	DEPENDS ${SHADER_SRC} ${CMAKE_SOURCE_DIR}/engine/rendering/shaders/generate_shader_header.sh
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/engine/rendering/shaders
)

file(GLOB RENDER_SRC rendering/src/*.cpp)
target_sources(birb PRIVATE ${RENDER_SRC} ${SHADER_SOURCE_HEADER})


# Widgets
file(GLOB WIDGET_SRC widgets/src/*.cpp)
target_sources(birb PRIVATE ${WIDGET_SRC})


target_link_libraries(birb glfw glad imgui microprofile)

target_precompile_headers(birb PUBLIC
	${CORE_INC}/Input.hpp
	${CORE_INC}/Logger.hpp
	${CORE_INC}/Vector.hpp

	PRIVATE
		<glad/gl.h>
		<GLFW/glfw3.h>
		<algorithm>
		<array>
		<cassert>
		<chrono>
		<cmath>
		<cstddef>
		<cstdint>
		<cstdio>
		<ctime>
		<fstream>
		<future>
		<imgui.h>
		<imgui_impl_glfw.h>
		<imgui_impl_opengl3.h>
		<iostream>
		<numeric>
		<queue>
		<random>
		<stb_image.h>
		<string>
		<sys/cdefs.h>
		<sys/resource.h>
		<thread>
		<type_traits>
		<vector>
)
