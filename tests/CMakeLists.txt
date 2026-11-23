add_executable(test_sudoku test_sudoku.c)
target_link_libraries(test_sudoku PRIVATE sudoku_core)

# Robustness: malformed/hostile input files, file round trips, bounded random generation
add_test(NAME robustness COMMAND test_sudoku robustness)
set_tests_properties(robustness PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME bitmask COMMAND test_sudoku bitmask)

# Solve the sample puzzles with every algorithm and validate each solution independently.
# The last argument is the largest board size also solved with the (exhaustive) brute force solvers.
set(SUDOKU_SAMPLE_PUZZLES
        "unsolved_4x4|data/unsolved/unsolved_4x4.txt|9"
        "unsolved_9x9|data/unsolved/unsolved_9x9.txt|9"
        "unsolved_16x16|data/unsolved/unsolved_16x16.txt|0"
        "unsolved_25x25|data/unsolved/unsolved_25x25.txt|0"
        "client_puzzles|client_test_files/unsolved_client.txt|9"
        "client_read|client_test_files/client_read.txt|9")

foreach (entry IN LISTS SUDOKU_SAMPLE_PUZZLES)
    string(REPLACE "|" ";" parts "${entry}")
    list(GET parts 0 name)
    list(GET parts 1 file)
    list(GET parts 2 bruteforce_max)
    add_test(NAME solve_${name}
            COMMAND test_sudoku solve ${PROJECT_SOURCE_DIR}/${file} ${bruteforce_max})
    set_tests_properties(solve_${name} PROPERTIES TIMEOUT 600)
endforeach ()

# Smoke test of the command line interface
add_test(NAME cli_solve COMMAND sudoku --solve ${PROJECT_SOURCE_DIR}/data/unsolved/unsolved_4x4.txt)
add_test(NAME cli_bad_option COMMAND sudoku --no-such-option)
set_tests_properties(cli_bad_option PROPERTIES WILL_FAIL TRUE)
