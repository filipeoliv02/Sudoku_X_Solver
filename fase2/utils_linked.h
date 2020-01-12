#include "algorithms_linked.h"

#ifndef SUDOKU_UTILS_LINKED_H
#define SUDOKU_UTILS_LINKED_H
int isConsistentLinked(SudokuQueueNode *sudoku);
SudokuQueueNode *cloneSudoku(SudokuQueueNode *sudoku);
void enqueueSudoku(SudokuQueue *sudokuQueue, SudokuQueueNode *sudoku);
SudokuQueueNode *dequeueSudoku(SudokuQueue *sudokuQueue);
void freeSudoku(SudokuQueueNode *sudokuQueueNode);
void freeCandidates(Node *candidatesOrigin);
void freeCandidateUnit(Node *candidatesOrigin, Node *(*nextNode)(Node *));
#endif //SUDOKU_UTILS_LINKED_H
