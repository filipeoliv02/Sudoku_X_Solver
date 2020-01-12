cmake_minimum_required(VERSION 3.15)
project(sudoku C)

set(CMAKE_C_STANDARD 99)

add_executable(sudoku main.c fase1/algorithms.c fase1/algorithms.h fase1/fileio.c fase1/fileio.h fase1/utils.c
        fase1/utils.h fase1/menus.c fase1/menus.h fase1/clients.c fase1/clients.h fase2/algorithms_linked.c
        fase2/algorithms_linked.h fase2/fileio_linked.c fase2/fileio_linked.h fase2/clients_linked.c
        fase2/clients_linked.h fase2/utils_linked.c fase2/utils_linked.h)

target_link_libraries(sudoku m)