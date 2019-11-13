#include "utils.h"
#include <stdio.h>





void printSudoku(int **sudoku, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}