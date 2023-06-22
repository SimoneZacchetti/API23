cmake_minimum_required(VERSION 3.22)
project(API23 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=gnu11 -O2 -g3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

add_executable(API23 main.c)
