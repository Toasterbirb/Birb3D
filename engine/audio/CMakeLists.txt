add_library(audio STATIC)
file(GLOB AUDIO_SRC src/*.cpp)
target_sources(audio PRIVATE ${AUDIO_SRC})
target_link_libraries(audio openal sndfile birb)

target_precompile_headers(audio
	PUBLIC
		<Logger.hpp>
		<Profiling.hpp>
		<Types.hpp>
		<cassert>
)
