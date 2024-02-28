add_executable(lighting ./lighting.cpp)
target_link_libraries(lighting birb)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/texture_512.png DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/specular_512.png DESTINATION ./)
