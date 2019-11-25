#include "algorithms.h"
#include "utils.h"

#include <math.h>

#define UNASSIGNED 0
#define ASSIGNED 1
int existsInCol(int **sudoku, int num, int col, int side);

int existsInRow(int **sudoku, int num, int row, int side);

int existsInRegion(int **sudoku, int num, int row, int col, int side);

int existsInPrincipalDiagonal(int **sudoku, int num, int row, int col, int side);

int existsInSecundaryDiagonal(int **sudoku, int num, int row, int col, int side);

int checkSudokuCell(ListSudoku list, int k, int row, int col);

void sudokuFillCell(ListSudoku list, int k, int row, int col);

void findPairs(ListSudoku cube, int row, int col);


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

void findSudokuBruteForce(int **sudoku, int row, int col, int side, ListSudoku *solved, int *cost) {
    int newRow = row + ((col + 1) / side), newCol = (col + 1) % side;
    if (row == side) {
        printSudoku(sudoku, side);

        solved->sudokus = resizeSudokus(solved->sudokus, solved->total, solved->total + 1);
        solved->sudokus[solved->total].size = side;
        solved->sudokus[solved->total].board = createBoard(side);
        for (int i = 0; i < side; i++) {
            for (int j = 0; j < side; j++) {
                solved->sudokus[solved->total].board[i][j] = sudoku[i][j];
            }
        }
        solved->total++;
    } else if (sudoku[row][col] > 0) {
        findSudokuBruteForce(sudoku, newRow, newCol, side, solved, cost);
    } else {
        for (int num = 1; num <= side; num++) {
            *cost += 5 * side;
            if (isValidPlacement(sudoku, num, row, col, side)) {
                sudoku[row][col] = num;
                findSudokuBruteForce(sudoku, newRow, newCol, side, solved, cost);
            }
        }
        sudoku[row][col] = 0;
    }
}

void sudokuFillCell(ListSudoku list, int k, int row, int col) {
    int region_size = sqrt(list.sudokus->size);
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
    int rowStart = (row / region_size) * region_size;
    int colStart = (col / region_size) * region_size;

    for (int i = rowStart; i < rowStart + region_size; i++) {
        for (int j = colStart; j < colStart + region_size; j++) {
            list.sudokus[k - 1].board[i][j] = 1;
        }
    }
}

int checkSudokuCell(ListSudoku list, int k, int row, int col) {
    int region_size = sqrt(list.sudokus->size);
    int count[6] = {0}, rowStart = (row / region_size) * region_size, colStart = (col / region_size) * region_size;
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
    for (int i = rowStart; i < rowStart + region_size; i++) {
        for (int j = colStart; j < colStart + region_size; j++) {
            if (list.sudokus[k - 1].board[i][j] == 0) {
                count[5]++;
            }
        }
    }
    return count[0] == 1 || count[1] == 1 || count[2] == 1 || count[3] == 1 || count[4] == 1 || count[5] == 1;
}

void findSudokuAdvanced(Sudoku s, ListSudoku *solved, int *cost) {
    ListSudoku cube;
    cube.sudokus = NULL;
    cube.sudokus = resizeSudokus(cube.sudokus, 0, s.size);
    cube.total = s.size;
    for (int i = 0; i < s.size; i++) {
        cube.sudokus[i].size = s.size;
        cube.sudokus[i].board = createBoard(s.size);
    }

    int count = 0, prev_count = 0;
    //percorrer tabuleiro original
    for (int i = 0; i < s.size; i++) {
        for (int j = 0; j < s.size; j++) {
            (*cost)++;
            if (s.board[i][j] != UNASSIGNED) {
                count++;
                sudokuFillCell(cube, s.board[i][j], i, j);
            }
        }
    }

    while (prev_count != count && count < s.size * s.size) {
        prev_count = count;

        for (int row = 0; row < s.size; row++) {
            for (int col = 0; col < s.size; col++) {
                if (s.board[row][col] == UNASSIGNED) {
                    for (int number = 1; number <= s.size; number++, (*cost)++) {
                        if (cube.sudokus[number - 1].board[row][col] == UNASSIGNED && checkSudokuCell(cube, number, row, col)) {
                            s.board[row][col] = number;
                            sudokuFillCell(cube, number, row, col);
                            count++;
                        }
                    }
                }
            }
        }
    }

    findSudokuBruteForce(s.board, 0, 0, s.size, solved, cost);
}

void findPairs(ListSudoku cube, int row, int col) {
    int size = cube.sudokus[0].size, num = 0, count = 0;

    int **lists = (int **) malloc(size * sizeof(int *));
    for (int i = 0; i < size; ++i) {
        *(lists + i) = (int *) calloc(size, sizeof(int));
    }

}

int isPattern(Sudoku pattern, Sudoku unsolved) {
    if (pattern.size != unsolved.size) {
        return 0;
    }
    for (int i = 0; i < pattern.size; i++) {
        for (int j = 0; j < unsolved.size; j++) {
            if (pattern.board[i][j] != unsolved.board[i][j] && unsolved.board[i][j] > 0) {
                return 0;
            }
        }
    }
    return 1;
}


int searchSudokus(ListSudoku searchList, Sudoku sudoku) {
    for (int i = 0; i < searchList.total; i++) {
        if (isPattern(searchList.sudokus[i], sudoku)) {
            return i;
        }
    }
    return -1;
}


