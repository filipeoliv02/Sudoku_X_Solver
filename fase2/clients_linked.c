#include "clients_linked.h"
#include <stdio.h>
#include "fileio_linked.h"
#include "utils_linked.h"

void client_linked_bruteforce(void) {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("./data/unsolved/unsolved.txt");
    SudokuQueue *sudokuSolvedQueue = createSudokuQueue();
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

void client_linked_optimized(void) {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("./data/unsolved/unsolved.txt");
    SudokuQueue *sudokuSolvedQueue = createSudokuQueue();
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
    saveSudokuQueueToFile(sudokuSolvedQueue, "./data/solved/solved_optimized.txt");

    freeSudokuQueue(sudokuUnsolvedQueue);
    freeSudokuQueue(sudokuSolvedQueue);
}

void client_linked_read_write_txt(void) {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("./data/unsolved/unsolved_read.txt");
    SudokuQueue *sudokuSolvedQueue = createSudokuQueue();
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->first != NULL) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        solveLinkedSudokuOptimized(sudokuSolvedQueue, sudoku);
        freeSudoku(sudoku);
    }
    saveSudokuQueueToFile(sudokuSolvedQueue, "./data/solved/solved_write.txt");
    freeSudokuQueue(sudokuUnsolvedQueue);
    freeSudokuQueue(sudokuSolvedQueue);
}

void client_linked_read_write_bin(void) {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromBinFile("./data/unsolved/unsolved_read.bin");
    SudokuQueue *sudokuSolvedQueue = createSudokuQueue();
    SudokuQueueNode *sudoku;

    while(sudokuUnsolvedQueue->first != NULL) {
        sudoku = dequeueSudoku(sudokuUnsolvedQueue);
        solveLinkedSudokuOptimized(sudokuSolvedQueue, sudoku);
        freeSudoku(sudoku);
    }
    saveSudokuQueueToBinFile(sudokuSolvedQueue, "./data/solved/solved_write.bin");
    freeSudokuQueue(sudokuUnsolvedQueue);
    freeSudokuQueue(sudokuSolvedQueue);
}

void client_linked_check_consistency(void) {
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

void client_linked_random_generation(void) {
    SudokuQueueNode *sudoku = generateRandomSudokuLinked(9, 16);
    if (sudoku == NULL) {
        return;
    }
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

void client_linked_solve_variable_size(void) {
    SudokuQueue *sudokuUnsolvedQueue = loadSudokuQueueFromFile("./data/unsolved/unsolved_all_sizes.txt");
    SudokuQueue *sudokuSolvedQueue = createSudokuQueue();
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