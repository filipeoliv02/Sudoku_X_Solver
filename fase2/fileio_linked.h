#ifndef SUDOKU_FILEIO_LINKED_H
#define SUDOKU_FILEIO_LINKED_H

#include "algorithms_linked.h"

SudokuQueue *loadSudokuQueueFromFile(const char *file);

int saveSudokuQueueToFile(SudokuQueue *sudokuQueue, const char *file);

SudokuQueue *loadSudokuQueueFromBinFile(const char *file);

int saveSudokuQueueToBinFile(SudokuQueue *sudokuQueue, const char *file);

#endif // SUDOKU_FILEIO_LINKED_H
