add_executable(sprite ./sprite.cpp)
target_link_libraries(sprite birb)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/texture_512.png DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/wide_texture_512x256.png DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/shaders DESTINATION ./)
