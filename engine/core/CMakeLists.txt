add_library(birb
	./src/performance_widget.cpp
	./src/window.cpp
)

target_precompile_headers(birb PUBLIC
	./include/Logger.hpp
	./include/Vector.hpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(birb glfw glad imgui)
