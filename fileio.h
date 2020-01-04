#ifndef SUDOKU_FILEIO_H
#define SUDOKU_FILEIO_H

#include "algorithms.h"

SudokuList load_sudokus(char *file);

void save_sudokus(SudokuList newlist, char *file);

void save_binary(SudokuList solved, char *file);

SudokuLinkedNode *load_sudokus_link(char *file);

void save_sudokus_linked(SudokuLinked sudokuLinked, char *file);

void save_binary_linked(SudokuLinked sudokuLinked, char *file);


#endif