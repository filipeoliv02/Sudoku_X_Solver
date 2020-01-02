#ifndef SUDOKU_ALGORITHMS_H
#define SUDOKU_ALGORITHMS_H


typedef struct node {
    int info, row, col;
    struct node *pn, *pne, *pe, *pse, *ps, *pso, *po, *pno;
    int *poss;
} NODE;

typedef struct sudoku_queue {
    NODE *pfirst;
    struct sudoku_queue *pnext;
    int size;
} SUDOKU_QUEUE;

struct sudoku {
    int size;
    int **board;
} typedef Sudoku;

struct listsudoku {
    int total;
    int *orderedList;
    Sudoku *sudokus;
} typedef ListSudoku;

void solveSudokuBruteForce(ListSudoku *solved, Sudoku sudoku, int row, int col, long long *cost);

void solveSudokuOptimized(Sudoku unsolved, ListSudoku *solved, long long *cost);

int isValidPlacement(Sudoku sudoku, int num, int row, int col);

int searchSudokus(ListSudoku searchList, Sudoku sudoku);

int isPattern(Sudoku pattern, Sudoku unsolved);

void computeOrderBySize(ListSudoku *a);

void solveSudokuBruteForceLink(SUDOKU_QUEUE queue, NODE *node);

int isValidPlacementLink(SUDOKU_QUEUE sudoku, NODE *node, int num);

int checkNakedSingle(NODE *node, int size);

void writePossibilities(SUDOKU_QUEUE sudoku);

void solveSudokuOptimizedLink(SUDOKU_QUEUE sudoku);

#endif