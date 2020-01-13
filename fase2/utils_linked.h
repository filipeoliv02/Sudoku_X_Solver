#include "algorithms_linked.h"

#ifndef SUDOKU_UTILS_LINKED_H
#define SUDOKU_UTILS_LINKED_H
void printSudokuLinked(SudokuQueueNode *sudoku);
void printCandidates(Node *first, int size);
int isSudokuLinkedEqual(SudokuQueueNode *sudoku1, SudokuQueueNode *sudoku2);
SudokuQueueNode *createEmptySudokuLinked(int size);
int isConsistentLinked(SudokuQueueNode *sudoku);
SudokuQueueNode *cloneSudoku(SudokuQueueNode *sudoku);
SudokuQueueNode *generateRandomSudokuLinked(int size, int n);
void enqueueSudoku(SudokuQueue *sudokuQueue, SudokuQueueNode *sudoku);
SudokuQueueNode *dequeueSudoku(SudokuQueue *sudokuQueue);
void freeSudokuQueue(SudokuQueue *sudokuQueue);
void freeSudoku(SudokuQueueNode *sudokuQueueNode);
void freeCandidates(Node *candidatesOrigin);
void freeCandidateUnit(Node *candidatesOrigin, Node *(*nextNode)(Node *));
#endif //SUDOKU_UTILS_LINKED_H
