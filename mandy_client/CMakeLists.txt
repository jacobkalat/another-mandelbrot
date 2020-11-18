cmake_minimum_required(VERSION 3.18)
project(mandy_client C)

set(CMAKE_C_STANDARD 11)

add_executable(mandy_client main.c graphicslibrary.h graphicslibrary.c)
target_link_libraries(mandy_client X11 m config)