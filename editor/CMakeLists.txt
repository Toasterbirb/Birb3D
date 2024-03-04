file(GLOB EDITOR_SRC src/*.cpp)

set(EDITOR_BIN_NAME "birb_editor")

include_directories(include)
add_executable(${EDITOR_BIN_NAME} ${EDITOR_SRC})
target_link_libraries(${EDITOR_BIN_NAME} birb)
