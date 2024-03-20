file(GLOB EDITOR_SRC src/*.cpp)

set(EDITOR_BIN_NAME "birb_editor")

add_executable(${EDITOR_BIN_NAME} ${EDITOR_SRC})
target_link_libraries(${EDITOR_BIN_NAME} birb)
target_include_directories(${EDITOR_BIN_NAME} PRIVATE include)

target_precompile_headers(${EDITOR_BIN_NAME} PRIVATE
	<algorithm>
	<entt.hpp>
	<glad/gl.h>
	<glm/glm.hpp>
	<glm/gtc/matrix_transform.hpp>
	<glm/gtc/type_ptr.hpp>
	<imgui.h>
	<imgui_internal.h>
	<imgui_stdlib.h>
	<limits>
	<stb_sprintf.h>
	<string>
	<vector>
)
