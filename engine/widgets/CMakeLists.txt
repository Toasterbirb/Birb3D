add_library(widgets STATIC)


file(GLOB WIDGET_SRC src/*.cpp)
target_sources(widgets PRIVATE ${WIDGET_SRC})
target_include_directories(widgets PRIVATE "${PROJECT_SOURCE_DIR}/engine/assets/primitive")

if (NOT BIRB_DISTCC)
	target_precompile_headers(widgets
		PUBLIC
			<Logger.hpp>
			<Profiling.hpp>
			<Types.hpp>
	)
endif()
