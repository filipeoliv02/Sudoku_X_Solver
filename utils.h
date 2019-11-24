#ifndef SUDOKU_UTILS_H
#define SUDOKU_UTILS_H

#include <stdlib.h>
#include "fileio.h"
#include <stdio.h>
#include "algorithms.h"

void printSudoku(int **sudoku, int size);

int *resizeArray(int *ptr, int size, int newSize);

int ***resizeBoards(int ***ptr, int size, int newSize);

Sudoku *resizeSudokus(Sudoku *ptr, int size, int newSize);

int **createBoard(int size);

void printAllStoredBoards(ListSudoku s);

Sudoku gen_sudoku(int size, int n);

int isEqual(Sudoku s1, Sudoku s2);

ListSudoku merge_sudokus(ListSudoku target, ListSudoku source);

#endif