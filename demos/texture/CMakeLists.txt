add_executable(texture ./texture.cpp)
target_link_libraries(texture birb)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/texture_512.png DESTINATION ./)
