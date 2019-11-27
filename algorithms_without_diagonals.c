#include "algorithms_without_diagonals.h"
#include "algorithms.h"
#include "utils.h"
#include <math.h>

#define UNASSIGNED 0
#define ASSIGNED 1

int isValidPlacementNoDiags(int **sudoku, int num, int row, int col, int side);
void sudokuFillCellNoDiags(ListSudoku list, int k, int row, int col);
int checkSudokuCellNoDiags(ListSudoku list, int k, int row, int col);

int isValidPlacementNoDiags(int **sudoku, int num, int row, int col, int side) {
    int s = (int) sqrt(side);
    return !(existsInCol(sudoku, num, col, side) ||
             existsInRow(sudoku, num, row, side) ||
             existsInRegion(sudoku, num, (row / s) * s, (col / s) * s, s));
}

void findSudokuBruteForceNoDiags(int **sudoku, int row, int col, int side, ListSudoku *solved, long long *cost) {
    int newRow = row + ((col + 1) / side), newCol = (col + 1) % side;
    if (row == side) {
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
        findSudokuBruteForceNoDiags(sudoku, newRow, newCol, side, solved, cost);
    } else {
        for (int num = 1; num <= side; num++) {
            (*cost)++;
            if (isValidPlacementNoDiags(sudoku, num, row, col, side)) {
                sudoku[row][col] = num;
                findSudokuBruteForceNoDiags(sudoku, newRow, newCol, side, solved, cost);
            }
        }
        sudoku[row][col] = 0;
    }
}

void sudokuFillCellNoDiags(ListSudoku list, int k, int row, int col) {
    int region_size = sqrt(list.sudokus->size);
    for (int i = 0; i < list.total; i++) {
        list.sudokus[k - 1].board[row][i] = 1;
        list.sudokus[k - 1].board[i][col] = 1;
        list.sudokus[i].board[row][col] = 1;
    }
    int rowStart = (row / region_size) * region_size;
    int colStart = (col / region_size) * region_size;

    for (int i = rowStart; i < rowStart + region_size; i++) {
        for (int j = colStart; j < colStart + region_size; j++) {
            list.sudokus[k - 1].board[i][j] = 1;
        }
    }
}

int checkSudokuCellNoDiags(ListSudoku list, int k, int row, int col) {
    int region_size = sqrt(list.sudokus->size);
    int count[4] = {0}, rowStart = (row / region_size) * region_size, colStart = (col / region_size) * region_size;
    for (int i = 0; i < list.total; i++) {
        if (list.sudokus[k - 1].board[row][i] == 0) {
            count[0]++;
        }
        if (list.sudokus[k - 1].board[i][col] == 0) {
            count[1]++;
        }
        if (list.sudokus[i].board[row][col] == 0) {
            count[2]++;
        }
    }
    for (int i = rowStart; i < rowStart + region_size; i++) {
        for (int j = colStart; j < colStart + region_size; j++) {
            if (list.sudokus[k - 1].board[i][j] == 0) {
                count[3]++;
            }
        }
    }
    return count[0] == 1 || count[1] == 1 || count[2] == 1 || count[3] == 1;
}

void findSudokuOptimizedNoDiags(Sudoku s, ListSudoku *solved, long long *cost) {
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
                sudokuFillCellNoDiags(cube, s.board[i][j], i, j);
            }
        }
    }

    while (prev_count != count && count < s.size * s.size) {
        prev_count = count;
        //findPairs(cube);
        for (int row = 0; row < s.size; row++) {
            for (int col = 0; col < s.size; col++) {
                if (s.board[row][col] == UNASSIGNED) {
                    for (int number = 1; number <= s.size; number++, (*cost)++) {
                        if (cube.sudokus[number - 1].board[row][col] == UNASSIGNED && checkSudokuCellNoDiags(cube, number, row, col)) {
                            s.board[row][col] = number;
                            sudokuFillCellNoDiags(cube, number, row, col);
                            count++;
                        }
                    }
                }
            }
        }

    }

    for (int i = 0; i < cube.total; i++)
        free(cube.sudokus[i].board);
    free(cube.sudokus);
    if (count != s.size * s.size) {
        printf("Cost %lld - Otimizado nao encontrou solucoes, ir para o bruteforce\n", *cost);
    }
    findSudokuBruteForceNoDiags(s.board, 0, 0, s.size, solved, cost);
}