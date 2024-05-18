add_executable(suzanne_benchmark ./suzanne_benchmark.cpp)
target_link_libraries(suzanne_benchmark birb)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/suzanne.obj DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/suzanne.mtl DESTINATION ./)
file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/manaspace/manaspc.ttf DESTINATION ./)
