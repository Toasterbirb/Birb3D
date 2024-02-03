find_package(doctest REQUIRED)

add_executable(birb_test
	./doctest.cpp
	./logger.cpp
	./math.cpp
	./vector.cpp
	./io.cpp
)

target_link_libraries(birb_test birb)

add_custom_target(test DEPENDS birb_test COMMAND birb_test)
