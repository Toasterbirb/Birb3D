add_executable(text_rendering ./text_rendering.cpp)
target_link_libraries(text_rendering birb)

file(COPY ${CMAKE_SOURCE_DIR}/demos/assets/manaspace/manaspc.ttf DESTINATION ./)
