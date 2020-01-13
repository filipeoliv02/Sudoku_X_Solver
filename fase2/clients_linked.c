#include "clients_linked.h"
#include <stdlib.h>
#include <stdio.h>
#include "fileio_linked.h"
#include "../fase1/algorithms.h"
#include "../fase1/utils.h"
#include "utils_linked.h"

void client_linked_bruteforce() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("./data/unsolved/unsolved.txt");
    SudokuQueue *sudokuSolvedQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->first != NULL) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        if(isConsistentLinked(sudoku)) {
            solveLinkedSudokuBruteForce(sudokuSolvedQueue, sudoku, sudoku->first);
        }
        else {
            printf("Erro, tabuleiro nao consistente\n");
        }

        freeSudoku(sudoku);
    }
    saveSudokuQueueToFile(sudokuSolvedQueue, "./data/solved/solved.txt");

    freeSudokuQueue(sudokuUnsolvedQueue);
    freeSudokuQueue(sudokuSolvedQueue);
}

void client_linked_optimized() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("./data/unsolved/unsolved.txt");
    SudokuQueue *sudokuSolvedQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->first != NULL) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        if(isConsistentLinked(sudoku)) {
            solveLinkedSudokuOptimized(sudokuSolvedQueue, sudoku);
        }
        else {
            printf("Erro, tabuleiro nao consistente\n");
        }
        freeSudoku(sudoku);
    }
    saveSudokuQueueToFile(sudokuSolvedQueue, "./data/unsolved/solved.txt");

    freeSudokuQueue(sudokuUnsolvedQueue);
    freeSudokuQueue(sudokuSolvedQueue);
}

void client_linked_read_write_txt() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("./data/unsolved/unsolved_read.txt");
    //saveSudokuQueueToBinFile(sudokuUnsolvedQueue, "./data/unsolved/unsolved_read.bin");
    SudokuQueue *sudokuSolvedQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->first != NULL) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        solveLinkedSudokuOptimized(sudokuSolvedQueue, sudoku);
        freeSudoku(sudoku);
    }
    saveSudokuQueueToFile(sudokuSolvedQueue, "./data/solved/solved_write.txt");
    freeSudokuQueue(sudokuUnsolvedQueue);
}

void client_linked_read_write_bin() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromBinFile("./data/unsolved/unsolved_read.bin");
    SudokuQueue *sudokuSolvedQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->first != NULL) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        solveLinkedSudokuOptimized(sudokuSolvedQueue, sudoku);
        freeSudoku(sudoku);
    }
    saveSudokuQueueToBinFile(sudokuSolvedQueue, "./data/solved/solved_write.bin");
    freeSudokuQueue(sudokuUnsolvedQueue);
}

void client_linked_check_consistency() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("./data/unsolved/unsolved_consistency.txt");
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->first != NULL) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        if(isConsistentLinked(sudoku)) {
            printf("Sudoku consistente:\n");
            solveLinkedSudokuOptimized(NULL, sudoku);
        }
        else {
            printf("Sudoku inconsistente:\n");
        }
        printSudokuLinked(sudoku);

        freeSudoku(sudoku);
    }

    freeSudokuQueue(sudokuUnsolvedQueue);
}

void client_linked_random_generation() {
    SudokuQueueNode *sudoku = generateRandomSudokuLinked(9, 16);
    if(isConsistentLinked(sudoku)) {
        printf("Sudoku consistente, tentar resolver\n");
        solveLinkedSudokuOptimized(NULL, sudoku);
    }
    else {
        printf("Sudoku inconsistente\n");
        printSudokuLinked(sudoku);
    }

    freeSudoku(sudoku);
}

void client_linked_solve_variable_size() {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("./data/unsolved/unsolved_all_sizes.txt");
    SudokuQueue *sudokuSolvedQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->first != NULL) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        if(isConsistentLinked(sudoku)) {
            solveLinkedSudokuOptimized(sudokuSolvedQueue, sudoku);
        }
        else {
            printf("Erro, tabuleiro nao consistente\n");
        }
        freeSudoku(sudoku);
    }
    saveSudokuQueueToFile(sudokuSolvedQueue, "./data/solved/solved_all_sizes.txt");

    freeSudokuQueue(sudokuUnsolvedQueue);
    freeSudokuQueue(sudokuSolvedQueue);
}