#ifndef SUDOKU_ALGORITHMS_H
#define SUDOKU_ALGORITHMS_H

#include "fileio.h"

void findSudokuBruteForce(int **sudoku, int row, int col, int side, ListSudoku *solved, int *cost);

void findSudokuAdvanced(Sudoku s, ListSudoku *solved, int *cost);

int isValidPlacement(int **sudoku, int num, int row, int col, int side);

int searchSudokus(ListSudoku searchList, Sudoku sudoku);

int isPattern(Sudoku pattern, Sudoku unsolved);

#endif