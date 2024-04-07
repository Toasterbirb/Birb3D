add_library(audio STATIC)
file(GLOB AUDIO_SRC src/*.cpp)
target_sources(audio PRIVATE ${AUDIO_SRC})

if (NOT BIRB_STATIC)
	target_link_libraries(audio openal sndfile birb)
else()
	target_link_libraries(audio birb)
endif()

target_precompile_headers(audio
	PUBLIC
		<Logger.hpp>
		<Profiling.hpp>
		<Types.hpp>
		<cassert>
)
