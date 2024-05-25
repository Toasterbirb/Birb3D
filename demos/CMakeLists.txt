option(BIRB_DEMO_ALL "Enable all Birb3D demos" OFF)

option(BIRB_DEMO_AUDIOPLAYER "Enable the audio player demo" OFF)
option(BIRB_DEMO_BENCHMARK "Enable the benchmark demo" OFF)
option(BIRB_DEMO_EMPTYWINDOW "Enable the empty window demo" OFF)
option(BIRB_DEMO_FPS "Enable the FPS demo" OFF)
option(BIRB_DEMO_IMGUI "Enable the dear ImGui demo" OFF)
option(BIRB_DEMO_LIGHTING "Enable the lighting demo" OFF)
option(BIRB_DEMO_LINES "Enable the line rendering demo" OFF)
option(BIRB_DEMO_RIGIDBODY "Enable the rigidbody physics demo" OFF)
option(BIRB_DEMO_SPRITE "Enable the sprite rendering demo" OFF)
option(BIRB_DEMO_SUZANNE "Enable the suzanne demo" OFF)
option(BIRB_DEMO_SUZANNE_BENCHMARK "Enable the suzanne benchmark demo" OFF)
option(BIRB_DEMO_TEXT "Enable the text rendering program" OFF)
option(BIRB_DEMO_TEXTURE "Enable the texture rendering demo" OFF)
option(BIRB_DEMO_TRIANGLE "Enable the hello world triangle demo" OFF)

if (BIRB_DEMO_ALL OR BIRB_DEMO_AUDIOPLAYER)
	add_subdirectory(audio_player)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_BENCHMARK)
	add_subdirectory(benchmark)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_EMPTYWINDOW)
	add_subdirectory(empty_window)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_FPS)
	add_subdirectory(fps)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_IMGUI)
	add_subdirectory(imgui)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_LIGHTING)
	add_subdirectory(lighting)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_LINES)
	add_subdirectory(lines)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_RIGIDBODY)
	add_subdirectory(rigidbody)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_SPRITE)
	add_subdirectory(sprite)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_SUZANNE)
	add_subdirectory(suzanne)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_SUZANNE_BENCHMARK)
	add_subdirectory(suzanne_benchmark)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_TEXT)
	add_subdirectory(text_rendering)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_TEXTURE)
	add_subdirectory(texture)
endif()

if (BIRB_DEMO_ALL OR BIRB_DEMO_TRIANGLE)
	add_subdirectory(triangle)
endif()
