cmake_minimum_required(VERSION 3.12)

project(Birb3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BIRB_RELEASE "Build a release build with all vendored dependencies" OFF)
option(BIRB_WINDOWS "Disable Linux specific things" OFF)
option(BIRB_PROFILER "Enable the profiling with microprofile" ON)
option(BIRB_STATIC "Use static linking" OFF)
option(BIRB_EDITOR "Build the editor project" ON)
option(BIRB_DEMOS "Build demo projects" ON)
option(BIRB_TESTS "Build unit tests" ON)
option(BIRB_PLAYGROUND "Build the playground project" OFF)
option(BIRB_DISTCC "Use distcc and ccache (if available) for compiling. Also
disables precompiled headers" OFF)

if (NOT CMAKE_BUILD_TYPE)
	message(WARNING "CMAKE_BUILD_TYPE not set. Defaulting to Debug")
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Default build type: Debug" FORCE)
endif()


include(vendor/assimp/cmake-modules/Findassimp.cmake)

# Check if birb3d is being built as part of a game project
set(GAME_PROJECT ON)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(GAME_PROJECT OFF)
endif()

if (BIRB_RELEASE)
	add_definitions(-DBIRB_RELEASE)
endif()

if (BIRB_WINDOWS)
	message("> Windows compatibility mode enabled")
	add_definitions(-DBIRB_PLATFORM_WINDOWS)

	# Disable the profiler on windows
	set(BIRB_PROFILER OFF CACHE BOOL "Enable the profiling with microprofile")

	# Disable linux specific stuff
	set(ALSOFT_BACKEND_PIPEWIRE OFF CACHE BOOL "Enable PipeWire backend")
	set(ALSOFT_BACKEND_PULSEAUDIO OFF CACHE BOOL "Enable PulseAudio backend")
	set(ALSOFT_BACKEND_ALSA OFF CACHE BOOL "Enable ALSA backend")
	set(ALSOFT_BACKEND_OSS OFF CACHE BOOL "Enable OSS backend")
	set(ALSOFT_BACKEND_SOLARIS OFF CACHE BOOL "Enable Solaris backend")
	set(ALSOFT_RTKIT OFF CACHE BOOL "Enable rtkit")

	set(WIN32 ON CACHE BOOL "Enable Win32 backend")

	set(GLFW_BUILD_WIN32 ON CACHE BOOL "Build support for Win32")
	set(GLFW_BUILD_X11 OFF CACHE BOOL "Build support for X11")
	set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Build support for wayland")

	# Use the glfw toolchain file
	set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/CMake/x86_64-w64-mingw32.cmake)
else()
	add_definitions(-DBIRB_PLATFORM_LINUX)
endif()

if (BIRB_STATIC)
	set(LIBTYPE STATIC CACHE BOOL "Enable static linking for OpenAL-soft")
	set(GLFW_LIBRARY_TYPE STATIC CACHE BOOL "Use static linking with glfw")
	add_definitions(-DAL_LIBTYPE_STATIC)
endif()

find_program(CCACHE_FOUND ccache)
find_program(DISTCC_FOUND distcc)

if (DISTCC_FOUND AND NOT BIRB_DISTCC)
	message("distcc was found, but it won't be used since BIRB_DISTCC is set to OFF")
endif()

if(CCACHE_FOUND AND NOT BIRB_DISTCC)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	message("> ccache was found, using it to speed up compiling")
elseif (CCACHE_FOUND AND DISTCC_FOUND AND BIRB_DISTCC)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "ccache distcc")
	message("> ccache and distcc were found, using them to speed up compiling")
endif()

include_directories(
	./engine/assets/include
	./engine/audio/include
	./engine/core/include
	./engine/physics/include
	./engine/rendering/include
	./engine/scenes/components/include
	./engine/scenes/include
	./engine/widgets/include
)

# Include doctest
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/doctest)

find_package(assimp)
# Build assimp
if (NOT assimp_FOUND OR BIRB_RELEASE)
# Disable some features that aren't needed
	set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build the assimp test suite")
	set(ASSIMP_INSTALL OFF CACHE BOOL "Disable this if you want to use assimp as a submodule.")
	set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Build assimp with all importers enabled.")
	set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Build assimp with all exporters enabled.")
	set(ASSIMP_NO_EXPORT ON CACHE BOOL "Disable Assimp's export functionality.")
	set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "Build Assimp view tool")

	# Enable the needed file formats
	set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Build support for .obj files")
	set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "Build support for .fbx files")
	set(ASSIMP_BUILD_BLEND_IMPORTER ON CACHE BOOL "Build support for .blend files")

	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include)

	if (GAME_PROJECT)
		include_directories(${CMAKE_BINARY_DIR}/birb3d/vendor/assimp/include)
	else()
		include_directories(${CMAKE_BINARY_DIR}/vendor/assimp/include)
	endif()

	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp)
endif()

# Build cpptrace if we are creating a debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vendor/cpptrace/include")
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/cpptrace")
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/freetype")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vendor/freetype/include")

# Build GLFW
set(GLFW_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw")
include_directories("${GLFW_PATH}/include")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate GLFW installation target")
add_subdirectory("${GLFW_PATH}")

# Build ImGui
set(IMGUI_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui")
add_library(imgui SHARED
	${IMGUI_PATH}/imgui.cpp
	${IMGUI_PATH}/imgui_demo.cpp
	${IMGUI_PATH}/imgui_draw.cpp
	${IMGUI_PATH}/imgui_tables.cpp
	${IMGUI_PATH}/imgui_widgets.cpp
	${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
	${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
	${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp)

target_include_directories(imgui PUBLIC
	${IMGUI_PATH}
	${IMGUI_PATH}/backends
	${IMGUI_PATH}/misc/cpp
)
target_link_libraries(imgui glfw)


# Build glad
set(GLAD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad")
include_directories(${GLAD_PATH}/include)
add_library(glad ${GLAD_PATH}/src/gl.c)

# Build glm
set(GLM_ENABLE_CXX_20 ON CACHE BOOL "Enable C++ 20")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm")

# Build microprofile
if (BIRB_PROFILER)
	add_subdirectory(vendor/microprofile)
	include_directories(vendor/microprofile)
	add_compile_definitions(MICROPROFILE_ENABLED=1)
else()
	add_compile_definitions(MICROPROFILE_ENABLED=0)
endif()

# Build minizip
add_subdirectory(vendor/minizip-ng)

# Build OpenAL
find_package(OpenAL)
if (NOT OpenAL_FOUND OR BIRB_RELEASE)
	set(ALSOFT_UTILS OFF CACHE BOOL "Build utility programs")
	set(ALSOFT_NO_CONFIG_UTIL ON CACHE BOOL "Disable building the alsoft-config utility")
	set(ALSOFT_EXAMPLES OFF CACHE BOOL "Build example programs")
	set(ALSOFT_INSTALL OFF CACHE BOOL "Install main library")
	add_subdirectory(vendor/openal-soft)
	include_directories(vendor/openal-soft/include)
endif()

# Build libsndfile
set(ENABLE_EXTERNAL_LIBS OFF CACHE BOOL "Enable FLAC, Vorbis, and Opus codecs")
set(ENABLE_MPEG OFF CACHE BOOL "Enable MPEG codecs")
set(BUILD_PROGRAMS OFF CACHE BOOL "Build programs")
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(ENABLE_CPACK OFF CACHE BOOL "Enable CPack support")
set(ENABLE_PACKAGE_CONFIG OFF CACHE BOOL "Generate and install package config file")
set(INSTALL_PKGCONFIG_MODULE OFF CACHE BOOL "Generate and install pkg-config module")
set(BUILD_TESTING OFF CACHE BOOL "Enable sndfile build testing")
add_subdirectory(vendor/libsndfile)
include_directories(vendor/libsndfile/include)

# Build with all kinds of warnings enabled because programmers usually don't
# know what they are doing
add_compile_options(-pedantic -Wall -Wextra -Wcast-align
	-Wdisabled-optimization -Wformat=2 -Wdisabled-optimization
	-Winit-self -Wlogical-op -Wmissing-declarations -Woverloaded-virtual
	-Werror -Wno-unused -Wredundant-decls -Wsign-promo -Wstrict-null-sentinel
	-Wstrict-overflow -Wmissing-format-attribute

	# These warnings are disabled due to them causing issues with stb libs
	# -Winline
)

# Build the engine library
add_subdirectory(engine)

# Skip building demos, editor, tests and the playground project if Birb3D is being
# built as part of a game project
if (NOT GAME_PROJECT)
	if (BIRB_EDITOR)
		# Build the editor
		add_subdirectory(editor)
	endif()

	if (BIRB_DEMOS)
		# Build demos
		add_subdirectory(demos)
	endif()

	if (BIRB_TESTS)
		# Build tests
		add_subdirectory(tests)
	endif()

	if (BIRB_PLAYGROUND)
		# Build the playground project
		add_subdirectory(playground)
	endif()
endif()

# Build documentation
find_package(Doxygen)
if (Doxygen_FOUND)
	set(DOXYGEN_HTML_EXTRA_STYLESHEET "${PROJECT_SOURCE_DIR}/vendor/doxygen-awesome-css/doxygen-awesome.css")
	set(DOXYGEN_DISABLE_INDEX NO)
	set(DOXYGEN_GENERATE_TREEVIEW YES)
	set(DOXYGEN_FULL_SIDEBAR NO)
	set(DOXYGEN_HTML_COLORSTYLE LIGHT)
	set(DOXYGEN_CALL_GRAPH YES)
	set(DOXYGEN_CALLER_GRAPH YES)
	set(DOXYGEN_EXTRACT_PRIVATE YES)
	doxygen_add_docs(docs "${PROJECT_SOURCE_DIR}/engine" COMMENT "Generate
	documentation from code comments")
endif()
