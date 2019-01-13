cmake_minimum_required(VERSION 3.13)
project(simpleFTP)


link_libraries(ws2_32)
set(CMAKE_CXX_STANDARD 17)
add_executable(simpleFTP main.cpp socket.h socket.cpp utils.h utils.cpp)