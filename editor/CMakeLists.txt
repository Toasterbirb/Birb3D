file(GLOB EDITOR_SRC src/*.cpp)

set(EDITOR_BIN_NAME "birb_editor")

include_directories(include)
add_executable(${EDITOR_BIN_NAME} ${EDITOR_SRC})
target_link_libraries(${EDITOR_BIN_NAME} birb)

target_precompile_headers(${EDITOR_BIN_NAME} PUBLIC
	<array>
	<assimp/Importer.hpp>
	<assimp/postprocess.h>
	<assimp/scene.h>
	<entt.hpp>
	<filesystem>
	<glm/glm.hpp>
	<glm/gtc/matrix_transform.hpp>
	<glm/gtc/type_ptr.hpp>
	<imgui.h>
	<imgui_internal.h>
	<imgui_stdlib.h>
	<iostream>
	<vector>
)
