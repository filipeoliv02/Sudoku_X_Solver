#ifndef SUDOKU_UTILS_H
#define SUDOKU_UTILS_H

#include <stdlib.h>
void printSudoku(int **sudoku, int size);
int *resizeArray(int *ptr, int size, int newSize);
int ***resizeBoards(int ***ptr, int size, int newSize);
int **createBoard(int **ptr, int size);

#endif