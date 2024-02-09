find_package(doctest REQUIRED)

file(GLOB TEST_SRC *.cpp)
add_executable(birb_test ${TEST_SRC})

target_link_libraries(birb_test birb)

add_custom_target(test DEPENDS birb_test COMMAND birb_test)
