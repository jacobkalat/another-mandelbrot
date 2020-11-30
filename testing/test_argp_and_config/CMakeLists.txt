cmake_minimum_required(VERSION 3.18)
project(test_argp_and_config C)

set(CMAKE_C_STANDARD 11)

add_executable(test_argp_and_config main.c)
target_link_libraries(test_argp_and_config m config)