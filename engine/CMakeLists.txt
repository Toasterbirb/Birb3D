add_library(birb
	./src/window.cpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(birb glfw OpenGL)
