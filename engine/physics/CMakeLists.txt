add_library(physics STATIC)

file (GLOB PHYSICS_SRC src/*.cpp)
target_sources(physics PRIVATE ${PHYSICS_SRC})
target_include_directories(physics PUBLIC include)

if (NOT BIRB_DISTCC)
	target_precompile_headers(physics
		PUBLIC
			<Logger.hpp>
			<Profiling.hpp>
			<Types.hpp>
			<cassert>
	)
endif()
