set(PLAYGROUND_DIR "${CMAKE_SOURCE_DIR}/playground")
add_custom_command(
	OUTPUT "${PLAYGROUND_DIR}/playground.cpp"
	COMMAND cp "${PLAYGROUND_DIR}/template.cpp"
	"${PLAYGROUND_DIR}/playground.cpp"
	WORKING_DIRECTORY "${PLAYGROUND_DIR}"
)

add_executable(playground "${PLAYGROUND_DIR}/playground.cpp")
target_link_libraries(playground birb)
