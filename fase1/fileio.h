#ifndef SUDOKU_FILEIO_H
#define SUDOKU_FILEIO_H

#include "algorithms.h"

SudokuList load_sudokus(const char *file);

int save_sudokus(SudokuList newlist, const char *file);

int save_binary(SudokuList solved, const char *file);

#endif // SUDOKU_FILEIO_H
