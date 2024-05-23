add_executable(benchmark ./benchmark.cpp)
target_link_libraries(benchmark birb)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/suzanne.obj DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/suzanne.mtl DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/manaspace/manaspc.ttf DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/texture_512.png DESTINATION ./)
