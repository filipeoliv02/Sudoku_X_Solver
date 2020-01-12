#include "clients_linked.h"
#include <stdlib.h>
#include <stdio.h>
#include "fileio_linked.h"
#include "../fase1/algorithms.h"
#include "../fase1/utils.h"
#include "utils_linked.h"

void client_linked_bruteforce() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("unsolved.txt");
    SudokuQueue *sudokuSolvedQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->total != 0) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        if(isConsistentLinked(sudoku)) {
            solveLinkedSudokuBruteForce(sudokuSolvedQueue, sudoku, sudoku->first);
            //solveLinkedSudokuOptimized(sudokuSolvedQueue, sudoku);
        }
        else {
            printf("Erro tabuleiro nao consistente\n");
        }

        freeSudoku(sudoku);
    }
    saveSudokuQueueToFile(sudokuSolvedQueue, "solved_linked.txt");

    freeSudokuQueue(sudokuUnsolvedQueue);
    freeSudokuQueue(sudokuSolvedQueue);
}

void client_linked_optimized() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("unsolved.txt");
    SudokuQueue *sudokuSolvedQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->total != 0) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        if(isConsistentLinked(sudoku)) {
            solveLinkedSudokuOptimized(sudokuSolvedQueue, sudoku);
        }
        else {
            printf("Erro tabuleiro nao consistente\n");
        }
        freeSudoku(sudoku);
    }
    saveSudokuQueueToFile(sudokuSolvedQueue, "solved_linked.txt");

    freeSudokuQueue(sudokuUnsolvedQueue);
    freeSudokuQueue(sudokuSolvedQueue);
}

void client_linked_read_txt() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("unsolved.txt");
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->total != 0) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        print_linked_board(sudoku);
        freeSudoku(sudoku);
    }
    freeSudokuQueue(sudokuUnsolvedQueue);
}

void client_linked_write_txt() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("unsolved.txt");
    saveSudokuQueueToFile(sudokuUnsolvedQueue, "unsolved_write.txt");
    freeSudokuQueue(sudokuUnsolvedQueue);
}

void client_linked_read_bin() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromBinFile("unsolved.bin");
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->total != 0) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        print_linked_board(sudoku);
        freeSudoku(sudoku);
    }
    freeSudokuQueue(sudokuUnsolvedQueue);
}

void client_linked_write_bin() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromBinFile("unsolved.bin");
    saveSudokuQueueToFile(sudokuUnsolvedQueue, "unsolved_write.bin");
    freeSudokuQueue(sudokuUnsolvedQueue);
}

void client_linked_check_consistency() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromBinFile("client_consistency.txt");
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->total != 0) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        if(isConsistentLinked(sudoku)) {
            printf("Sudoku consistente:\n");
        }
        else {
            printf("Sudoku consistente:\n");
        }
        print_linked_board(sudoku);
        freeSudoku(sudoku);
    }

    freeSudokuQueue(sudokuUnsolvedQueue);
}

void client_linked_random_generation() {
    SudokuQueueNode *sudoku = generateRandomSudokuLinked(9, 10);
    if(isConsistentLinked(sudoku)) {
        printf("Sudoku consistente, tentar resolver\n");
        solveLinkedSudokuOptimized(NULL, sudoku);
    }
    else {
        printf("Sudoku inconsistente:\n");
        print_linked_board(sudoku);
    }
}

void client_linked_solve_variable_size() {

}

void client_linked_search_solutions() {

}

void client_linked_compare_algorithms() {

}
