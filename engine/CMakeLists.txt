# Include shader source code
include_directories(rendering/shaders)

add_library(birb STATIC)

set(IMGUI_PATH "${PROJECT_SOURCE_DIR}/vendor/imgui")
set(DEPENDENCY_HEADERS
	# stb libraries
	${PROJECT_SOURCE_DIR}/vendor/stb

	# EnTT
	${PROJECT_SOURCE_DIR}/vendor/entt/single_include/entt

	# cereal
	${PROJECT_SOURCE_DIR}/vendor/cereal/include

	# nlohmann::json
	${PROJECT_SOURCE_DIR}/vendor/json/include

	# dear imgui
	${IMGUI_PATH}
	${IMGUI_PATH}/backends
	${IMGUI_PATH}/misc/cpp
)

target_include_directories(birb PUBLIC ${DEPENDENCY_HEADERS})

# Engine core
set(CORE_INC core/include)
file(GLOB CORE_SRC core/src/*.cpp)
target_sources(birb PRIVATE ${CORE_SRC})


# Asset management
file(GLOB ASSETS_SRC assets/src/*.cpp)
target_sources(birb PRIVATE ${ASSETS_SRC})

# Audio
file(GLOB AUDIO_SRC audio/src/*.cpp)
target_sources(birb PRIVATE ${AUDIO_SRC})


# Rendering
file(GLOB SHADER_SRC rendering/shaders/*.glsl)
set(SHADER_SOURCE_HEADER
	"${PROJECT_SOURCE_DIR}/engine/rendering/shaders/ShaderSource.hpp")
add_custom_command(
	OUTPUT ${SHADER_SOURCE_HEADER}
	COMMAND ./generate_shader_header.sh
	DEPENDS ${SHADER_SRC} ${PROJECT_SOURCE_DIR}/engine/rendering/shaders/generate_shader_header.sh
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/engine/rendering/shaders
)

# Build the primitive meshes header
file(GLOB OBJ_SRC assets/primitive/*.obj)
set(OBJ_SRC_HEADER
	"${PROJECT_SOURCE_DIR}/engine/assets/primitive/PrimitiveMeshes.hpp")
add_custom_command(
	OUTPUT ${OBJ_SRC_HEADER}
	COMMAND ./gen_primitive_mesh_header.sh
	DEPENDS ${OBJ_SRC} ${PROJECT_SOURCE_DIR}/engine/assets/primitive/gen_primitive_mesh_header.sh
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/engine/assets/primitive
)

file(GLOB RENDER_SRC rendering/src/*.cpp rendering/src/ui/*)
target_sources(birb PRIVATE ${RENDER_SRC})
target_include_directories(birb PUBLIC assets/primitive)


# Physics
file (GLOB PHYSICS_SRC physics/src/*.cpp)
target_sources(birb PRIVATE ${PHYSICS_SRC})


# Scenes and ECS
file(GLOB COMPONENT_HEADERS "${PROJECT_SOURCE_DIR}/engine/scenes/components/include/*.hpp")
set(COMPONENT_COMBINATION_HEADER "${PROJECT_SOURCE_DIR}/engine/scenes/include/Components.hpp")
add_custom_command(
	OUTPUT ${COMPONENT_COMBINATION_HEADER}
	COMMAND ./gen_component_header.sh > ${COMPONENT_COMBINATION_HEADER}
	DEPENDS ${COMPONENT_HEADERS} ${PROJECT_SOURCE_DIR}/engine/scenes/components/gen_component_header.sh
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/engine/scenes/components
)
file(GLOB SCENES_SRC scenes/src/*.cpp scenes/components/src/*.cpp)
target_sources(birb PRIVATE ${SCENES_SRC} ${COMPONENT_COMBINATION_HEADER})
target_include_directories(birb PUBLIC
	"${PROJECT_SOURCE_DIR}/engine/scenes/components/include")


# Widgets
file(GLOB WIDGET_SRC widgets/src/*.cpp)
target_sources(birb PRIVATE ${WIDGET_SRC})


add_custom_target(headers)
target_sources(headers PRIVATE ${OBJ_SRC_HEADER} ${SHADER_SOURCE_HEADER} ${COMPONENT_COMBINATION_HEADER})

add_dependencies(birb headers)

target_link_libraries(birb
	assimp
	glfw
	glad
	glm
	imgui
	freetype
	sndfile
)

if (NOT BIRB_WINDOWS)
	target_link_libraries(birb openal)
else()
	target_link_libraries(birb OpenAL32)
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_link_libraries(birb cpptrace::cpptrace)
endif()

if (BIRB_PROFILER)
	target_link_libraries(birb microprofile)
endif()

if (NOT BIRB_DISTCC)
	target_precompile_headers(birb
		PUBLIC
			<Logger.hpp>
			<Profiling.hpp>
			<Types.hpp>

		PRIVATE
			# Headers that probably won't be changed that often
			<Color.hpp>
			<IO.hpp>
			<Input.hpp>
			<Random.hpp>
			<Rect.hpp>
			<Vector.hpp>
			<Timestep.hpp>

			# The order of these two headers is important
			<glad/gl.h>
			<GLFW/glfw3.h>

			# Other misc. library headers
			<algorithm>
			<array>
			<assimp/Importer.hpp>
			<assimp/material.h>
			<assimp/mesh.h>
			<assimp/postprocess.h>
			<assimp/scene.h>
			<cereal/archives/binary.hpp>
			<cereal/types/string.hpp>
			<climits>
			<cmath>
			<cstddef>
			<cstdint>
			<deque>
			<entt.hpp>
			<fstream>
			<future>
			<glm/fwd.hpp>
			<glm/glm.hpp>
			<glm/gtc/matrix_transform.hpp>
			<glm/gtc/type_ptr.hpp>
			<glm/vec2.hpp>
			<glm/vec3.hpp>
			<imgui.h>
			<imgui_impl_glfw.h>
			<imgui_impl_opengl3.h>
			<imgui_stdlib.h>
			<iostream>
			<nlohmann/json.hpp>
			<nlohmann/json_fwd.hpp>
			<numeric>
			<queue>
			<stb_image.h>
			<stb_sprintf.h>
			<string>
			<unordered_map>
			<unordered_set>
			<vector>
	)
endif()


# Utils
add_subdirectory(utils)
