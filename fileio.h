#ifndef SUDOKU_FILEIO_H
#define SUDOKU_FILEIO_H
#include <stdlib.h>
struct sudoku {
    int size;
    int **board;
} typedef Sudoku;

struct listsudoku {
    int total;
    int *orderedList;
    Sudoku *sudokus;
} typedef ListSudoku;


ListSudoku load_sudokus(char *file);

void save_sudokus(ListSudoku newlist, char *file);

void save2binary(ListSudoku solved, char *file);


#endif