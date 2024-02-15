find_package(glfw3 REQUIRED)
add_library(birb)

# Engine core
set(CORE_INC core/include)
file(GLOB CORE_SRC core/src/*.cpp)
target_sources(birb PRIVATE ${CORE_SRC})


# Rendering
file(GLOB RENDER_SRC rendering/src/*.cpp)
target_sources(birb PRIVATE ${RENDER_SRC})

target_link_libraries(birb glfw glad imgui)

target_precompile_headers(birb PUBLIC
	${CORE_INC}/Input.hpp
	${CORE_INC}/Logger.hpp
	${CORE_INC}/Vector.hpp

	PRIVATE
		<glad/gl.h>
		<GLFW/glfw3.h>
)
