#ifndef SUDOKU_ALGORITHMS_WITHOUT_DIAGONALS_H
#define SUDOKU_ALGORITHMS_WITHOUT_DIAGONALS_H

#include "fileio.h"

void findSudokuBruteForceNoDiags(int **sudoku, int row, int col, int side, ListSudoku *solved, long long *cost);

void findSudokuOptimizedNoDiags(Sudoku s, ListSudoku *solved, long long *cost);

#endif //SUDOKU_ALGORITHMS_WITHOUT_DIAGONALS_H
