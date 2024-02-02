find_package(doctest REQUIRED)

add_executable(birb_test
	./doctest.cpp
	./logger.cpp
	./math.cpp
	./vector.cpp
)

add_custom_target(test DEPENDS birb_test COMMAND birb_test)
