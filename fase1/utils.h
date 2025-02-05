#ifndef SUDOKU_UTILS_H
#define SUDOKU_UTILS_H

#include "fileio.h"
#include "../fase2/algorithms_linked.h"

void printSudoku(int **sudoku, int size);

Sudoku *resizeSudokus(Sudoku *ptr, int size, int newSize);

int **createBoard(int size);

void printAllStoredBoards(SudokuList s, int flagOrdered);

Sudoku gen_sudoku(int size, int n);

int isEqual(Sudoku s1, Sudoku s2);

SudokuList merge_sudokus(SudokuList target, SudokuList source);

int *resizeList(int *pList, int size, int newSize);

int isConsistent(Sudoku sudoku);

int gettimeuseconds(long long *time_usec);

void free_list_sudoku(SudokuList l);

#endif