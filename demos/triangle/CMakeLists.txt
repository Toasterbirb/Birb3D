add_executable(triangle ./triangle.cpp)
target_link_libraries(triangle birb)

file(GLOB SHADER_SRC ${CMAKE_SOURCE_DIR}/engine/rendering/shaders/*.glsl)
file(COPY ${SHADER_SRC} DESTINATION ./)
