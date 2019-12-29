#ifndef SUDOKU_FILEIO_H
#define SUDOKU_FILEIO_H

#include "algorithms.h"

ListSudoku load_sudokus(char *file);

void save_sudokus(ListSudoku newlist, char *file);

void save2binary(ListSudoku solved, char *file);

SUDOKU_QUEUE *load_sudokus_link(char *file);
#endif