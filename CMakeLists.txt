cmake_minimum_required(VERSION 3.17)
project(dice C)

set(CMAKE_C_STANDARD 99)

set(SOURCES
        calc_coba.h
        structs.c
        calc_odin.c
        calc_coba.c
        main.c
)

add_executable(dice ${SOURCES})
