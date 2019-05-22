cmake_minimum_required(VERSION 2.6)
project(openglprojtexmap)

include_directories(/home/hongjun/hjwang/opengl/dist/include)

add_executable(openglprojtexmap main.cpp)
target_link_libraries(openglprojtexmap /usr/lib/x86_64-linux-gnu/libglut.so)
target_link_libraries(openglprojtexmap /usr/lib/x86_64-linux-gnu/libGL.so)
target_link_libraries(openglprojtexmap /usr/lib/x86_64-linux-gnu/libGLU.so)
target_link_libraries(openglprojtexmap /usr/lib/x86_64-linux-gnu/libGLEW.so)
target_link_libraries(openglprojtexmap /home/hongjun/hjwang/opengl/dist/lib/libSOIL.a)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

install(TARGETS openglprojtexmap RUNTIME DESTINATION bin)
