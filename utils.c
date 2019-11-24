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
        printf("\n %dº", k + 1);
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
    Sudoku *pAux = (Sudoku *) malloc(newSize * sizeof(Sudoku));

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

Sudoku gen_sudoku(int size, int n) {
    int row, col, num;
    Sudoku s;
    s.board = NULL;
    s.size = size;
    s.board = createBoard(size);
    for (int i = 0; i < n; i++) {
        row = rand() % size;
        col = rand() % size;
        num = (rand() % size) + 1;
        if (isValidPlacement(s.board, num, row, col, size) && *(*(s.board + row) + col) == 0) {
            *(*(s.board + row) + col) = num;

        } else {
            i--;
        }

    }
    return s;


}

int isEqual(Sudoku s1, Sudoku s2) {
    if (s1.size != s2.size) {
        return 0;
    }
    for (int i = 0; i < s1.size; i++) {
        for (int j = 0; j < s2.size; j++) {
            if (s1.board[i][j] != s2.board[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}


ListSudoku merge_sudokus(ListSudoku target, ListSudoku source) {
    int alreadyExists;
    for (int i = 0; i < source.total; i++) {
        alreadyExists = 0;
        for (int j = 0; j < target.total; j++) {
            if (isEqual(target.sudokus[j], source.sudokus[i])) {             //Target->Source[j]
                alreadyExists = 1;
            }
        }
        if (alreadyExists == 0) {
            target.sudokus = resizeSudokus(target.sudokus, target.total, target.total + 1);
            target.sudokus[target.total].size = source.sudokus[i].size;
            target.sudokus[target.total].board = source.sudokus[i].board;
            target.total++;
        }
    }
    return target;
}
