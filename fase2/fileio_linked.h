#ifndef SUDOKU_FILEIO_LINKED_H
#define SUDOKU_FILEIO_LINKED_H

#include "../fase2/algorithms_linked.h"

SudokuLinkedNode *load_sudokus_link(char *file);

void save_sudokus_linked(SudokuLinked sudokuLinked, char *file);

void save_binary_linked(SudokuLinked sudokuLinked, char *file);

#endif //SUDOKU_FILEIO_LINKED_H
