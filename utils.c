#include "utils.h"


void printSudoku(int **sudoku, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printAllStoredBoards(ListSudoku s) {
    for (int k = 0; k < s.total; k++) {
        printf("\n %dº", k+1);
        for (int i = 0; i < s.sudokus[k].size; i++) {
            printf("\n");
            for (int j = 0; j < s.sudokus[k].size; j++) {
                if (s.sudokus[k].board[i][j] > 9) {
                    printf("%d ", s.sudokus[k].board[i][j]);

                } else {
                    printf(" %d ", s.sudokus[k].board[i][j]);

                }
            }
        }
        printf("\n");
    }
};


int *resizeArray(int *ptr, int size, int newSize) {
    int *pAux = (int *) calloc(newSize, sizeof(int));

    for (int i = 0; i < size; i++) {
        *(pAux + i) = *(ptr + i);

    }
    free(ptr);

    return pAux;
}

Sudoku *resizeSudokus(Sudoku *ptr, int size, int newSize) {
    Sudoku *pAux = (Sudoku *) malloc(newSize* sizeof(Sudoku));

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

int **createBoard(int size) {
    int **pAux = (int **) malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++) {
        *(pAux + i) = (int *) calloc(size, sizeof(int));
    }
    return pAux;
}
