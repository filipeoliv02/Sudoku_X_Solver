#ifndef SUDOKU_FILEIO_H
#define SUDOKU_FILEIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct boards {
    int *sizes;
    int num;
    int ***board;
} typedef Boards;


void load_boards(Boards *b);


#endif