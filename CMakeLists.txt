cmake_minimum_required(VERSION 3.20)
project(sudoku_x_solver
        VERSION 2.0.0
        DESCRIPTION "Sudoku X generator and solver (array and linked-list implementations)"
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

option(SUDOKU_ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(SUDOKU_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

# Common warnings/sanitizer settings, applied to every target through an interface library
add_library(sudoku_options INTERFACE)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sudoku_options INTERFACE -Wall -Wextra -Wpedantic -Wshadow -Wformat=2)
    if (SUDOKU_WARNINGS_AS_ERRORS)
        target_compile_options(sudoku_options INTERFACE -Werror)
    endif ()
    if (SUDOKU_ENABLE_SANITIZERS)
        target_compile_options(sudoku_options INTERFACE
                -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=undefined)
        target_link_options(sudoku_options INTERFACE -fsanitize=address,undefined)
    endif ()
endif ()

# Solver code shared by the program and the tests
add_library(sudoku_core STATIC
        common/sudoku_common.c
        fase1/algorithms.c
        fase1/clients.c
        fase1/fileio.c
        fase1/menus.c
        fase1/utils.c
        fase2/algorithms_linked.c
        fase2/clients_linked.c
        fase2/fileio_linked.c
        fase2/utils_linked.c
        solvers/bitmask.c)
target_include_directories(sudoku_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sudoku_core PUBLIC sudoku_options)
if (NOT WIN32)
    target_link_libraries(sudoku_core PUBLIC m)
endif ()

add_executable(sudoku main.c)
target_link_libraries(sudoku PRIVATE sudoku_core)

include(CTest)
if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()
