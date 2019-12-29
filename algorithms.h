#ifndef SUDOKU_ALGORITHMS_H
#define SUDOKU_ALGORITHMS_H

#include "fileio.h"

typedef struct node {
    int info;
    struct node *pn, *pne, *pe, *pse, *ps, *pso, *po, *pno;
} NODE;

typedef struct sudoku_queue {
    NODE *pfirst;
    struct sudoku_queue *pnext;
    int size;
} SUDOKU_QUEUE;


void solveSudokuBruteForce(ListSudoku *solved, Sudoku sudoku, int row, int col, long long *cost);

void solveSudokuOptimized(Sudoku unsolved, ListSudoku *solved, long long *cost);

int isValidPlacement(Sudoku sudoku, int num, int row, int col);

int searchSudokus(ListSudoku searchList, Sudoku sudoku);

int isPattern(Sudoku pattern, Sudoku unsolved);

void computeOrderBySize(ListSudoku *a);

#endif