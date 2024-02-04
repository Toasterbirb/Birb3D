find_package(doctest REQUIRED)

add_executable(birb_test
	./doctest.cpp
	./color.cpp
	./io.cpp
	./logger.cpp
	./math.cpp
	./rect.cpp
	./vector.cpp
)

target_link_libraries(birb_test birb)

add_custom_target(test DEPENDS birb_test COMMAND birb_test)
