add_executable(fps ./fps.cpp)
target_link_libraries(fps birb)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/world.obj DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/world.mtl DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/tree.obj DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/tree.mtl DESTINATION ./)
