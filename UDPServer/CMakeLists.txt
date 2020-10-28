cmake_minimum_required(VERSION 3.17)
project(UDPServer C)

set(CMAKE_C_STANDARD 11)

add_executable(UDPServer main.c UDP_Routines.c UDP_Routines.h)
target_link_libraries(UDPServer pthread)