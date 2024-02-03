find_package(glfw3 REQUIRED)
add_library(birb)

# Engine core
set(CORE core/src)
set(CORE_INC core/include)
target_sources(birb PRIVATE
	${CORE}/io.cpp
	${CORE}/performance_widget.cpp
	${CORE}/window.cpp)


# Rendering
set(RENDER rendering/src)
target_sources(birb PRIVATE
	${RENDER}/renderer.cpp)

target_link_libraries(birb glfw glad imgui)

target_precompile_headers(birb PUBLIC
	${CORE_INC}/Logger.hpp
	${CORE_INC}/Vector.hpp

	PRIVATE
		<glad/gl.h>
		<GLFW/glfw3.h>
)
