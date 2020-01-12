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
}