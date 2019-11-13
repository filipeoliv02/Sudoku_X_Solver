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


int *resizeArray(int *ptr, int size, int newSize) {
    int *pAux = (int *) calloc(newSize, sizeof(int));

    for (int i = 0; i < size; i++) {
        *(pAux + i) = *(ptr + i);

    }
    free(ptr);

    return pAux;
}

int ***resizeBoards(int ***ptr, int size, int newSize) {
    int ***pAux = (int ***) calloc(newSize, sizeof(int **));

    for (int i = 0; i < size; i++) {
        *(pAux + i) = *(ptr + i);

    }
    free(ptr);

    return pAux;
}

int **createBoard(int **ptr, int size) {
    int **pAux = (int **) malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++) {
        *(pAux + i) = (int *) calloc(size, sizeof(int));
    }
    return pAux;
}