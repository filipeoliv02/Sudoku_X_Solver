#ifndef SUDOKU_UTILS_H
#define SUDOKU_UTILS_H

#include <stdlib.h>
#include "fileio.h"
#include <stdio.h>

void printSudoku(int **sudoku, int size);

int *resizeArray(int *ptr, int size, int newSize);

int ***resizeBoards(int ***ptr, int size, int newSize);
Sudoku *resizeSudokus(Sudoku *ptr, int size, int newSize);
int **createBoard(int size);

void printAllStoredBoards(ListSudoku s);

#endif