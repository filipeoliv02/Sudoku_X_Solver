#ifndef SUDOKU_FILEIO_LINKED_H
#define SUDOKU_FILEIO_LINKED_H

#include "../fase2/algorithms_linked.h"

SudokuQueue *loadSudokuQueueFromFile(char *file);

void saveSudokuQueueToFile(SudokuQueue *sudokuQueue, char *file);

SudokuQueue *loadSudokuQueueFromBinFile(char *file);

void saveSudokuQueueToBinFile(SudokuQueue *sudokuQueue, char *file);

#endif //SUDOKU_FILEIO_LINKED_H
