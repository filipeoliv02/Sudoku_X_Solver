#ifndef SUDOKU_FILEIO_H
#define SUDOKU_FILEIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct sudoku {
    int size;
    int **board;
} typedef Sudoku;

struct listsudoku {
    int total;
    Sudoku *sudokus;
} typedef ListSudoku;


ListSudoku load_sudokus(char *file);

#endif