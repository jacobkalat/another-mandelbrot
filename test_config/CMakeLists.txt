cmake_minimum_required(VERSION 3.17)
project(test_config C)

set(CMAKE_C_STANDARD 11)

add_executable(test_config main.c)
target_link_libraries(test_config config)