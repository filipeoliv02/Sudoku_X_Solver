#ifndef SUDOKU_ALGORITHMS_H
#define SUDOKU_ALGORITHMS_H

typedef struct sudoku {
    int size;
    int **board;
} Sudoku;

typedef struct sudoku_list {
    int total;
    int *orderedList;
    Sudoku *sudokus;
} SudokuList;

void solveSudokuBruteForce(SudokuList *solved, Sudoku sudoku, int row, int col, long long *cost);

void solveSudokuOptimized(Sudoku unsolved, SudokuList *solved, long long *cost);

int isValidPlacement(Sudoku sudoku, int num, int row, int col);

int searchSudokus(SudokuList searchList, Sudoku sudoku);

int isPattern(Sudoku pattern, Sudoku unsolved);

void computeOrderBySize(SudokuList *a);

#endif