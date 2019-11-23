#ifndef SUDOKU_ALGORITHMS_H
#define SUDOKU_ALGORITHMS_H

#include "fileio.h"

void findSudokuBruteForce(int **sudoku, int row, int col, int side);

void findSudokuAdvanced(Sudoku s);

int isValidPlacement(int **sudoku, int num, int row, int col, int side);



#endif