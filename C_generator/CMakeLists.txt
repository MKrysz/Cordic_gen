cmake_minimum_required(VERSION 3.24)
project(C C)

set(CMAKE_C_STANDARD 11)

add_executable(C main.c)

target_link_libraries(C m)