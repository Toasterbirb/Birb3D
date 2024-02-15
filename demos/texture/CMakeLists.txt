add_executable(texture ./texture.cpp)
target_link_libraries(texture birb)

file(GLOB SHADER_SRC ${CMAKE_SOURCE_DIR}/engine/rendering/shaders/*.glsl)
file(COPY ${SHADER_SRC} DESTINATION ./)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/texture_512.png DESTINATION ./)
