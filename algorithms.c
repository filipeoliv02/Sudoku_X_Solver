#include "algorithms.h"
#include "utils.h"

#include <math.h>



int existsInCol(int **sudoku, int num, int col, int side);

int existsInRow(int **sudoku, int num, int row, int side);

int existsInRegion(int **sudoku, int num, int row, int col, int side);

int existsInPrincipalDiagonal(int **sudoku, int num, int row, int col, int side);

int existsInSecundaryDiagonal(int **sudoku, int num, int row, int col, int side);

int checkSudokuCell(ListSudoku list, int k, int row, int col);

void sudokuFillCell(ListSudoku list, int k, int row, int col);


int isValidPlacement(int **sudoku, int num, int row, int col, int side) {
    int s = (int) sqrt(side);
    return !(existsInCol(sudoku, num, col, side) ||
             existsInRow(sudoku, num, row, side) ||
             existsInRegion(sudoku, num, (row / s) * s, (col / s) * s, s) ||
             existsInPrincipalDiagonal(sudoku, num, row, col, side) ||
             existsInSecundaryDiagonal(sudoku, num, row, col, side));
}

int existsInCol(int **sudoku, int num, int col, int side) {
    for (int i = 0; i < side; i++) {
        if (sudoku[i][col] == num)
            return 1;
    }
    return 0;
}

int existsInRow(int **sudoku, int num, int row, int side) {
    for (int i = 0; i < side; i++) {
        if (sudoku[row][i] == num)
            return 1;
    }
    return 0;
}

int existsInRegion(int **sudoku, int num, int rowStart, int colStart, int regionSide) {
    for (int i = rowStart; i < rowStart + regionSide; i++) {
        for (int j = colStart; j < colStart + regionSide; j++) {
            if (sudoku[i][j] == num) {
                return 1;
            }
        }
    }
    return 0;
}

int existsInPrincipalDiagonal(int **sudoku, int num, int row, int col, int side) {
    if (row == col) {
        for (int i = 0; i < side; i++) {
            if (sudoku[i][i] == num)
                return 1;
        }
    }
    return 0;
}

int existsInSecundaryDiagonal(int **sudoku, int num, int row, int col, int side) {
    if (row == side - col - 1) {
        for (int i = 0; i < side; i++) {
            if (sudoku[i][side - i - 1] == num)
                return 1;
        }
    }
    return 0;
}

void findSudokuBruteForce(int **sudoku, int row, int col, int side) {
    int newRow = row + ((col + 1) / side), newCol = (col + 1) % side;
    if (row == side) {
        printSudoku(sudoku, side);
    } else if (sudoku[row][col] > 0) {
        findSudokuBruteForce(sudoku, newRow, newCol, side);
    } else {
        for (int num = 1; num <= side; num++) {
            if (isValidPlacement(sudoku, num, row, col, side)) {
                sudoku[row][col] = num;
                findSudokuBruteForce(sudoku, newRow, newCol, side);
            }
        }
        sudoku[row][col] = 0;
    }
}

void sudokuFillCell(ListSudoku list, int k, int row, int col) {
    for (int i = 0; i < list.total; i++) {
        list.sudokus[k - 1].board[row][i] = 1;
        list.sudokus[k - 1].board[i][col] = 1;
        list.sudokus[i].board[row][col] = 1;
        if (row == col) {
            list.sudokus[k - 1].board[i][i] = 1;
        }
        if (row == list.total - col - 1) {
            list.sudokus[k - 1].board[i][list.total - i - 1] = 1;
        }
    }
    int rowStart = (row / 3) * 3;
    int colStart = (col / 3) * 3;

    for (int i = rowStart; i < rowStart + 3; i++) {
        for (int j = colStart; j < colStart + 3; j++) {
            list.sudokus[k - 1].board[i][j] = 1;
        }
    }
}

int checkSudokuCell(ListSudoku list, int k, int row, int col) {
    int count[6] = {0}, rowStart = (row / 3) * 3, colStart = (col / 3) * 3;
    for (int i = 0; i < list.total; i++) {
        if (list.sudokus[k - 1].board[row][i] == 0) {
            count[0]++;
        }
        if (list.sudokus[k - 1].board[i][col] == 0) {
            count[1]++;
        }
        if (row == col && list.sudokus[k - 1].board[i][i] == 0) {
            count[2]++;
        }
        if (row == list.total - col - 1 && list.sudokus[k - 1].board[i][list.total - i - 1] == 0) {
            count[3]++;
        }
        if (list.sudokus[i].board[row][col] == 0) {
            count[4]++;
        }
    }
    for (int i = rowStart; i < rowStart + 3; i++) {
        for (int j = colStart; j < colStart + 3; j++) {
            if (list.sudokus[k - 1].board[i][j] == 0) {
                count[5]++;
            }
        }
    }
    return count[0] == 1 || count[1] == 1 || count[2] == 1 || count[3] == 1 || count[4] == 1 || count[5] == 1;
}

void findSudokuAdvanced(Sudoku s) {
    ListSudoku cube;
    cube.sudokus = NULL;
    cube.sudokus = resizeSudokus(cube.sudokus, 0, s.size);
    cube.total = s.size;
    for (int i = 0; i < s.size; i++) {
        cube.sudokus[i].size = s.size;
        cube.sudokus[i].board = createBoard(s.size);
    }

    int count = 0, count_aux = 0;
    //percorrer tabuleiro original
    for (int i = 0; i < s.size; i++) {
        for (int j = 0; j < s.size; j++) {
            if (s.board[i][j] > 0) {
                count++;
                for (int k = 0; k < s.size; k++) {
                    cube.sudokus[k].board[i][j] = 1;
                }
            }
        }
    }

    while (1) {
        if (count_aux == count || count == s.size * s.size) {
            break;
        }
        count_aux = count;
        for (int k = 0; k < s.size; k++) {
            for (int i = 0; i < s.size; i++) {
                for (int j = 0; j < s.size; j++) {
                    if (s.board[i][j] == k + 1 && cube.sudokus[k].board[i][j] == 1) {
                        sudokuFillCell(cube, k + 1, i, j);
                    }
                }
            }
        }

        for (int k = 0; k < s.size; k++) {
            for (int i = 0; i < s.size; i++) {
                for (int j = 0; j < s.size; j++) {
                    if (cube.sudokus[k].board[i][j] == 0 && checkSudokuCell(cube, k + 1, i, j)) {
                        s.board[i][j] = k + 1;
                        sudokuFillCell(cube, k + 1, i, j);
                        count++;
                    }
                }
            }
        }
    }

    findSudokuBruteForce(s.board, 0, 0, s.size);
}