add_library(widgets STATIC)


file(GLOB WIDGET_SRC src/*.cpp)
target_sources(widgets PRIVATE ${WIDGET_SRC})

target_precompile_headers(widgets
	PUBLIC
		<Logger.hpp>
		<Profiling.hpp>
		<Types.hpp>
		<cassert>
)
