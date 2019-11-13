#ifndef SUDOKU_ALGORITHMS_H
#define SUDOKU_ALGORITHMS_H

#define L 9
#define C 9
#define GRID_LENGTH 9

void findSudokuBruteForce(int **sudoku, int row, int col, int side);

void findSudokuAdvanced(int ***sudoku, int row, int col, int side);


#endif