cmake_minimum_required(VERSION 3.10)

# Set the project name
project(GameProject)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add compile options
add_compile_options(-Wall -Werror)

# Add an executable
add_executable(game main.c calc_odin.c structs.c)

# Include directories
target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR})
