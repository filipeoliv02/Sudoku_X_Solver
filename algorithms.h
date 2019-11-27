#ifndef SUDOKU_ALGORITHMS_H
#define SUDOKU_ALGORITHMS_H

#include "fileio.h"

void findSudokuBruteForce(int **sudoku, int row, int col, int side, ListSudoku *solved, long long *cost);

void findSudokuAdvanced(Sudoku s, ListSudoku *solved, long long *cost);

int isValidPlacement(int **sudoku, int num, int row, int col, int side);

int searchSudokus(ListSudoku searchList, Sudoku sudoku);

int isPattern(Sudoku pattern, Sudoku unsolved);

int existsInCol(int **sudoku, int num, int col, int side);

int existsInRow(int **sudoku, int num, int row, int side);

int existsInRegion(int **sudoku, int num, int row, int col, int side);

#endif