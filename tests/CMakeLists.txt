find_package(doctest REQUIRED)

add_executable(birb_test
	./doctest.cpp
	./logger.cpp
	./vector.cpp
)

add_custom_target(test DEPENDS birb_test COMMAND birb_test)
