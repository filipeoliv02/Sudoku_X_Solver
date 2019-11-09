#include <stdio.h>
#include <math.h>

#define L 9
#define C 9
#define GRID_LENGTH 9

void printSudoku(int sudoku[][C]);
void findSudokuBruteForce(int sudoku[][C], int row, int col, int side);
int isValidPlacement(int sudoku[][C], int num, int row, int col, int side);
int existsInCol(int sudoku[][C], int num, int col, int side);
int existsInRow(int sudoku[][C], int num, int row, int side);
int existsInRegion(int sudoku[][C], int num, int row, int col, int side);
int existsInPrincipalDiagonal(int sudoku[][C], int num, int row, int col, int side);
int existsInSecundaryDiagonal(int sudoku[][C], int num, int row, int col, int side);

int main() {
    int sudoku[L][C] = {{1, 0, 9, 7, 0, 0, 0, 3, 4},
                        {0, 0, 0, 0, 0, 4, 0, 0, 0},
                        {0, 0, 3, 0, 0, 0, 5, 0, 0},
                        {6, 0, 0, 0, 3, 2, 0, 0, 7},
                        {0, 0, 4, 0, 8, 0, 0, 2, 0},
                        {0, 0, 0, 0, 0, 7, 0, 0, 1},
                        {0, 0, 1, 0, 0, 0, 0, 5, 0},
                        {0, 0, 0, 0, 5, 0, 0, 0, 8},
                        {0, 0, 0, 0, 0, 8, 0, 0, 0}};

    printSudoku(sudoku);
    findSudokuBruteForce(sudoku, 0, 0, 9);
    printSudoku(sudoku);
    return 0;
}

void printSudoku(int sudoku[][C]) {
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int isValidPlacement(int sudoku[][C], int num, int row, int col, int side) {
    int s = (int) sqrt(side);
    return !(existsInCol(sudoku, num, col, side) ||
             existsInRow(sudoku, num, row, side) ||
             existsInRegion(sudoku, num, (row/s)*s, (col/s)*s, s) ||
             existsInPrincipalDiagonal(sudoku, num, row, col, side) ||
             existsInSecundaryDiagonal(sudoku, num, row, col, side));
}

int existsInCol(int sudoku[][C], int num, int col, int side) {
    for(int i = 0; i < side; i++) {
        if (sudoku[i][col] == num)
            return 1;
    }
    return 0;
}

int existsInRow(int sudoku[][C], int num, int row, int side) {
    for(int i = 0; i < side; i++) {
        if (sudoku[row][i] == num)
            return 1;
    }
    return 0;
}

int existsInRegion(int sudoku[][C], int num, int rowStart, int colStart, int regionSide) {
    for(int i = rowStart; i < rowStart + regionSide; i++) {
        for (int j = colStart; j < colStart + regionSide; j++) {
            if (sudoku[i][j] == num) {
                return 1;
            }
        }
    }
    return 0;
}

int existsInPrincipalDiagonal(int sudoku[][C], int num, int row, int col, int side) {
    if(row == col) {
        for (int i = 0; i < side; i++) {
            if (sudoku[i][i] == num)
                return 1;
        }
    }
    return 0;
}

int existsInSecundaryDiagonal(int sudoku[][C], int num, int row, int col, int side) {
    if(row == side-col-1) {
        for (int i = 0; i < side; i++) {
            if (sudoku[i][side-i-1] == num)
                return 1;
        }
    }
    return 0;
}

void findSudokuBruteForce(int sudoku[][C], int row, int col, int side) {
    int newRow = row+((col+1)/side), newCol = (col+1)%side;
    if (row * row == side * side) {
        printSudoku(sudoku);
    }
    else if (sudoku[row][col] > 0) {
        findSudokuBruteForce(sudoku, newRow, newCol, side);
    }
    else {
        for (int num = 1; num <= side; num++) {
            if (isValidPlacement(sudoku, num, row, col, side)) {
                sudoku[row][col] = num;
                findSudokuBruteForce(sudoku, newRow, newCol, side);
            }
        }
        sudoku[row][col] = 0;
    }
}
