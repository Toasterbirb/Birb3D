add_executable(suzanne ./suzanne.cpp)
target_link_libraries(suzanne birb)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/suzanne.obj DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/suzanne.mtl DESTINATION ./)

