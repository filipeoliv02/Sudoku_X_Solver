#ifndef SUDOKU_ALGORITHMS_H
#define SUDOKU_ALGORITHMS_H

#include "fileio.h"

void solveSudokuBruteForce(ListSudoku *solved, Sudoku sudoku, int row, int col, long long *cost);

void solveSudokuOptimized(Sudoku s, ListSudoku *solved, long long *cost);

int isValidPlacement(Sudoku sudoku, int num, int row, int col);

int searchSudokus(ListSudoku searchList, Sudoku sudoku);

int isPattern(Sudoku pattern, Sudoku unsolved);

void computeOrderBySize(ListSudoku *a);

#endif