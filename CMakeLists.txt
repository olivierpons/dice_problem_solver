cmake_minimum_required(VERSION 3.17)
project(dice C)

set(CMAKE_C_STANDARD 99)

# Trouver le package OpenMP
find_package(OpenMP)

set(SOURCES
        calc_coba.h
        structs.c
        calc_odin.c
        calc_coba.c
        main.c
)

if(OpenMP_C_FOUND)
    add_executable(dice ${SOURCES})
    target_link_libraries(dice PUBLIC OpenMP::OpenMP_C)
else()
    message(FATAL_ERROR "OpenMP not found. Please install it.")
endif()
