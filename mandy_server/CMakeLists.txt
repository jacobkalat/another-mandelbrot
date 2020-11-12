cmake_minimum_required(VERSION 3.17)
project(mandy_server C)

set(CMAKE_C_STANDARD 11)

add_executable(mandy_server main.c UDP_Routines.c UDP_Routines.h graphicslibrary.c graphicslibrary.h mandelbrot.c mandelbrot.h)
target_link_libraries(mandy_server pthread m)