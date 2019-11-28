#include "algorithms.h"
#include "utils.h"

#include <math.h>

#define UNASSIGNED 0
#define ASSIGNED 1

int existsInPrincipalDiagonal(int **sudoku, int num, int row, int col, int side);

int existsInSecundaryDiagonal(int **sudoku, int num, int row, int col, int side);

int checkSudokuCell(ListSudoku list, int k, int row, int col);

void sudokuFillCell(ListSudoku list, int k, int row, int col);

void findPairs(ListSudoku cube);

int isValidPlacement(int **sudoku, int num, int row, int col, int side) {
    int s = (int) sqrt(side);
    if (side <= 16) {
        return !(existsInCol(sudoku, num, col, side) ||
                 existsInRow(sudoku, num, row, side) ||
                 existsInRegion(sudoku, num, (row / s) * s, (col / s) * s, s) ||
                 existsInPrincipalDiagonal(sudoku, num, row, col, side) ||
                 existsInSecundaryDiagonal(sudoku, num, row, col, side));
    } else {
        return !(existsInCol(sudoku, num, col, side) ||
                 existsInRow(sudoku, num, row, side) ||
                 existsInRegion(sudoku, num, (row / s) * s, (col / s) * s, s));
    }
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

void findSudokuBruteForce(int **sudoku, int row, int col, int side, ListSudoku *solved, long long *cost) {
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
        findSudokuBruteForce(sudoku, newRow, newCol, side, solved, cost);
    } else {
        for (int num = 1; num <= side; num++) {
            (*cost)++;
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
        if (list.sudokus->size <= 16) {
            if (row == col) {
                list.sudokus[k - 1].board[i][i] = 1;
            }
            if (row == list.total - col - 1) {
                list.sudokus[k - 1].board[i][list.total - i - 1] = 1;
            }
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
        if (list.sudokus->size <= 16) {
            if (row == col && list.sudokus[k - 1].board[i][i] == 0) {
                count[2]++;
            }
            if (row == list.total - col - 1 && list.sudokus[k - 1].board[i][list.total - i - 1] == 0) {
                count[3]++;
            }
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

void findSudokuAdvanced(Sudoku s, ListSudoku *solved, long long *cost) {
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
            if (s.board[i][j] != UNASSIGNED) {
                count++;
                sudokuFillCell(cube, s.board[i][j], i, j);
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
                        if (cube.sudokus[number - 1].board[row][col] == UNASSIGNED &&
                            checkSudokuCell(cube, number, row, col)) {
                            s.board[row][col] = number;
                            sudokuFillCell(cube, number, row, col);
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
    findSudokuBruteForce(s.board, 0, 0, s.size, solved, cost);
}

void findPairs(ListSudoku cube) {
    int size = cube.sudokus[0].size, count = 0, found;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            count = 0;
            for (int num = 0; num < size; num++) {
                if (cube.sudokus[num].board[row][col] == UNASSIGNED) {
                    count++;
                }
            }
            if (count == 2) {
                for (int row_pair = 0; row_pair < size; row_pair++) {
                    if (row != row_pair) {
                        found = 1;
                        for (int i = 0; i < size; i++) {
                            if (cube.sudokus[i].board[row][col] != cube.sudokus[i].board[row_pair][col]) {
                                found = 0;
                                break;
                            }
                        }
                        if (found) {
                            printf("s[%d][%d] - ", row_pair, col);
                            for (int i = 0; i < size; i++) {
                                printf("%d ", cube.sudokus[i].board[row_pair][col]);
                            }
                            printf("\n");
                            for (int i = 0; i < size; i++) {
                                printf("%d-\n", i + 1);
                                printSudoku(cube.sudokus[i].board, size);
                            }

                            for (int row_remove = 0; row_remove < size; row_remove++) {
                                if (row_remove != row && row_remove != row_pair) {
                                    for (int num_remove = 0; num_remove < size; num_remove++) {
                                        if (cube.sudokus[num_remove].board[row][col] == 0) {
                                            cube.sudokus[num_remove].board[row_remove][col] = 1;
                                        }
                                    }
                                }
                            }
                            for (int i = 0; i < size; i++) {
                                printf("%d-\n", i + 1);
                                printSudoku(cube.sudokus[i].board, size);
                            }
                            break;
                        }
                    }
                }

                for (int col_pair = 0; col_pair < size; col_pair++) {
                    if (col != col_pair) {
                        found = 1;
                        for (int i = 0; i < size; i++) {
                            if (cube.sudokus[i].board[row][col] != cube.sudokus[i].board[row][col_pair]) {
                                found = 0;
                                break;
                            }
                        }
                        if (found) {
                            printf("s[%d][%d] - ", row, col_pair);
                            for (int i = 0; i < size; i++) {
                                printf("%d ", cube.sudokus[i].board[row][col_pair]);
                            }
                            printf("\n");
                            for (int col_remove = 0; col_remove < size; col_remove++) {

                                if (col_remove != row && col_remove != col_pair) {
                                    for (int num_remove = 0; num_remove < size; num_remove++) {
                                        if (cube.sudokus[num_remove].board[row][col] == 0) {
                                            cube.sudokus[num_remove].board[row][col_remove] = 1;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
                /*if (row == col) {
                    for (int diag_pair = 0; diag_pair < size; diag_pair++) {
                        if (row != diag_pair && col != diag_pair) {
                            found = 1;
                            for (int i = 0; i < size; i++) {
                                if (cube.sudokus[i].board[row][col] != cube.sudokus[i].board[diag_pair][diag_pair]) {
                                    found = 0;
                                    break;
                                }
                            }
                            if (found) {
                                for (int diag_remove = 0; diag_remove < size; diag_remove++) {
                                    if (diag_remove != row && diag_remove != diag_pair) {
                                        for (int num_remove = 0; num_remove < size; num_remove++) {
                                            if (cube.sudokus[num_remove].board[row][col] == 0) {
                                                cube.sudokus[num_remove].board[diag_remove][diag_remove] = 1;
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }

                if (row + col == size - 1) {
                    for (int diagsec_pair = 0; diagsec_pair < size; diagsec_pair++) {
                        if (row != diagsec_pair && col != size - diagsec_pair - 1) {
                            found = 1;
                            for (int i = 0; i < size; i++) {
                                if (cube.sudokus[i].board[row][col] !=
                                    cube.sudokus[i].board[diagsec_pair][size - diagsec_pair - 1]) {
                                    found = 0;
                                    break;
                                }
                            }
                            if (found) {
                                for (int diagsec_remove = 0; diagsec_remove < size; diagsec_remove++) {
                                    if (diagsec_remove != row && diagsec_remove != diagsec_pair) {
                                        for (int num_remove = 0; num_remove < size; num_remove++) {
                                            if (cube.sudokus[num_remove].board[row][col] == 0) {
                                                cube.sudokus[num_remove].board[diagsec_remove][size - diagsec_pair - 1] = 1;
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }*/
                int region_size = sqrt(size);
                int rowStart = (row / region_size) * region_size, colStart = (col / region_size) * region_size;
                for (int i = rowStart; i < rowStart + region_size; i++) {
                    for (int j = colStart; j < colStart + region_size; j++) {
                        found = 1;
                        for (int num = 0; num < size; num++) {
                            if (cube.sudokus[num].board[row][col] != cube.sudokus[num].board[i][j]) {
                                found = 0;
                                break;
                            }
                        }
                        if (found) {
                            for (int i_remove = rowStart; i_remove < rowStart + region_size; i_remove++) {
                                for (int j_remove = colStart; j_remove < colStart + region_size; j_remove++) {
                                    if (i_remove != row && j_remove != col) {
                                        for (int num_remove = 0; num_remove < size; num_remove++) {
                                            if (cube.sudokus[num_remove].board[row][col] == 0) {
                                                cube.sudokus[num_remove].board[i_remove][j_remove] = 1;
                                            }
                                        }
                                    }
                                }

                            }
                            break;
                        }
                    }
                }
            }
        }
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


