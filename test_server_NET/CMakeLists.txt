cmake_minimum_required(VERSION 3.17)
project(test_server_NET C)

set(CMAKE_C_STANDARD 11)

add_executable(test_server_NET main.c UDP_Routines.c UDP_Routines.h)
target_link_libraries(test_server_NET pthread)