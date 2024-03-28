add_executable(sprite ./sprite.cpp)
target_link_libraries(sprite birb)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/texture_512.png DESTINATION ./)
