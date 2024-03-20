# Include shader source code
include_directories(rendering/shaders)

add_library(birb)

# Include stb libraries
target_include_directories(birb PUBLIC ${PROJECT_SOURCE_DIR}/vendor/stb)

# Include EnTT
target_include_directories(birb PUBLIC ${PROJECT_SOURCE_DIR}/vendor/entt/single_include/entt)

# Include cereal
target_include_directories(birb PUBLIC ${PROJECT_SOURCE_DIR}/vendor/cereal/include)

# Include nlohmann::json
target_include_directories(birb PUBLIC ${PROJECT_SOURCE_DIR}/vendor/json/include)

# Engine core
set(CORE_INC core/include)
file(GLOB CORE_SRC core/src/*.cpp)
target_sources(birb PRIVATE ${CORE_SRC})


# Asset management
file(GLOB ASSETS_SRC assets/src/*.cpp)
target_sources(birb PRIVATE ${ASSETS_SRC})


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

file(GLOB RENDER_SRC rendering/src/*.cpp)
target_sources(birb PRIVATE ${RENDER_SRC} ${SHADER_SOURCE_HEADER})


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


# Widgets
file(GLOB WIDGET_SRC widgets/src/*.cpp)
target_sources(birb PRIVATE ${WIDGET_SRC})


target_link_libraries(birb assimp glfw glad glm imgui microprofile)

target_precompile_headers(birb
	PUBLIC
		<Logger.hpp>

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
		<cassert>
		<cereal/archives/binary.hpp>
		<cereal/types/string.hpp>
		<chrono>
		<climits>
		<cmath>
		<cstddef>
		<cstdint>
		<ctime>
		<deque>
		<entt.hpp>
		<filesystem>
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
		<microprofile.h>
		<nlohmann/json.hpp>
		<nlohmann/json_fwd.hpp>
		<numeric>
		<queue>
		<random>
		<stb_image.h>
		<stb_sprintf.h>
		<string.h>
		<string>
		<sys/resource.h>
		<thread>
		<unordered_map>
		<unordered_set>
		<vector>
)


# Utils
add_subdirectory(utils)
