cmake_minimum_required(VERSION 3.15)
project(sudoku C)

set(CMAKE_C_STANDARD 99)

add_executable(sudoku main.c algorithms.c algorithms.h fileio.c fileio.h utils.c utils.h menus.c menus.h clients.c clients.h algorithms_without_diagonals.c algorithms_without_diagonals.h)
target_link_libraries(sudoku m)