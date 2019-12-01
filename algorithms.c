#include "algorithms.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define UNASSIGNED 0

// Tamanho máximo para um tabuleiro de SudokuX, tabuleiros de tamanho superior são considerados como Sudoku normal
#define MAX_SUDOKUX_SIZE 16

int checkSudokuCell(ListSudoku list, int k, int row, int col, int size);

void sudokuFillCell(ListSudoku list, int k, int row, int col);

void update_solved(ListSudoku *solved, Sudoku sudoku);

int existsInPrincipalDiagonal(Sudoku sudoku, int num, int row, int col);

int existsInSecundaryDiagonal(Sudoku sudoku, int num, int row, int col);

int existsInCol(Sudoku sudoku, int num, int col);

int existsInRow(Sudoku sudoku, int num, int row);

int existsInRegion(Sudoku sudoku, int num, int row, int col, int regionSize);

int findPairs(ListSudoku cube);

/**
 * @brief Verifica o número passado pode ser colocado na (linha, coluna) do tabuleiro
 * @param sudoku
 * @param num
 * @param row
 * @param col
 * @return
 */
int isValidPlacement(Sudoku sudoku, int num, int row, int col) {
    int s = (int) sqrt(sudoku.size);
    return !(existsInCol(sudoku, num, col) ||
             existsInRow(sudoku, num, row) ||
             existsInRegion(sudoku, num, (row / s) * s, (col / s) * s, s) ||
             existsInPrincipalDiagonal(sudoku, num, row, col) ||
             existsInSecundaryDiagonal(sudoku, num, row, col));
}

/**
 * @brief   Verifica a coluna
 * @details Vê se existe na coluna o número passado
 * @param sudoku
 * @param num
 * @param col
 * @param side
 * @return 1 - Verdadeiro / 0 - Falso
 */
int existsInCol(Sudoku sudoku, int num, int col) {
    for (int i = 0; i < sudoku.size; i++) {
        if (*(*(sudoku.board + i) + col) == num)
            return 1;
    }
    return 0;
}

/**
 * @brief   Verifica a Linha
 * @details Vê se existe na linha o número passado
 * @param sudoku
 * @param num
 * @param row
 * @param side
 * @return 1 - Verdadeiro / 0 - Falso
 */
int existsInRow(Sudoku sudoku, int num, int row) {
    for (int i = 0; i < sudoku.size; i++) {
        if (*(*(sudoku.board + row) + i) == num)
            return 1;
    }
    return 0;
}

/**
 * @brief   Verifica a Região
 * @details Verifica-se o número passsado existe na região testada
 * @param sudoku
 * @param num
 * @param rowStart
 * @param colStart
 * @param regionSide
 * @return 1 - Verdadeiro / 0 - Falso
 */
int existsInRegion(Sudoku sudoku, int num, int rowStart, int colStart, int regionSize) {
    for (int i = rowStart; i < rowStart + regionSize; i++) {
        for (int j = colStart; j < colStart + regionSize; j++) {
            if (*(*(sudoku.board + i) + j) == num) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief Verifica a diagonal principal
 * @details Verifica-se a célula passada pertence à diagonal principal, e se sim se o seu número já se encontra na diagonal
 * @param sudoku
 * @param num
 * @param row
 * @param col
 * @param side
 * @return 1 - Verdadeiro / 0 - Falso
 *
 */
int existsInPrincipalDiagonal(Sudoku sudoku, int num, int row, int col) {
    if (sudoku.size <= MAX_SUDOKUX_SIZE && row == col) {
        for (int i = 0; i < sudoku.size; i++) {
            if (*(*(sudoku.board + i) + i) == num)
                return 1;
        }
    }
    return 0;
}

/**
 * @brief Verifica a diagonal secundária
 * @details Verifica-se a célula passada pertence à diagonal secundária, e se sim se o seu número já se encontra na diagonal
 * @param sudoku
 * @param num
 * @param row
 * @param col
 * @param side
 * @return 1 - Verdadeiro / 0 - Falso
 */
int existsInSecundaryDiagonal(Sudoku sudoku, int num, int row, int col) {
    if (sudoku.size <= MAX_SUDOKUX_SIZE && row == sudoku.size - col - 1) {
        for (int i = 0; i < sudoku.size; i++) {
            if (*(*(sudoku.board + i) + sudoku.size - i - 1) == num)
                return 1;
        }
    }
    return 0;
}

/**
 * @brief Algoritmo BruteForce
 * @details Algoritmo do tipo Backtracking Search usado na resolução de tabuleiros através de bruteforce(Recursivo)
 * @param sudoku
 * @param row
 * @param col
 * @param side
 * @param solved
 * @param cost
 */
void solveSudokuBruteForce(ListSudoku *solved, Sudoku sudoku, int row, int col, long long *cost) {
    int newRow = row + ((col + 1) / sudoku.size), newCol = (col + 1) % sudoku.size;
    if (row == sudoku.size) {
        printf("Solucao:\n");
        printSudoku(sudoku.board, sudoku.size);
        update_solved(solved, sudoku);
    } else if (*(*(sudoku.board + row) + col) > 0) {
        solveSudokuBruteForce(solved, sudoku, newRow, newCol, cost);
    } else {
        for (int num = 1; num <= sudoku.size; num++) {
            (*cost)++;
            if (isValidPlacement(sudoku, num, row, col)) {
                *(*(sudoku.board + row) + col) = num;
                solveSudokuBruteForce(solved, sudoku, newRow, newCol, cost);
            }
        }
        *(*(sudoku.board + row) + col) = 0;
    }
}

/**
 * @brief Atualiza a lista de soluções
 * @param solved
 * @param sudoku
 */
void update_solved(ListSudoku *solved, Sudoku sudoku) {
    for (int j = 0; j < solved->total; j++) {
        if (isEqual(*(solved->sudokus + j), sudoku)) {
            return;
        }
    }
    solved->sudokus = resizeSudokus(solved->sudokus, solved->total, solved->total + 1);
    solved->orderedList = resizeList(solved->orderedList, solved->total, solved->total + 1);
    (solved->sudokus + solved->total)->size = sudoku.size;
    (solved->sudokus + solved->total)->board = createBoard(sudoku.size);
    for (int i = 0; i < sudoku.size; i++) {
        for (int j = 0; j < sudoku.size; j++) {
            *(*((solved->sudokus + solved->total)->board + i) + j) = *(*(sudoku.board + i) + j);
        }
    }
    solved->total++;
}

/**
 * @brief Coloca as posições a proibir
 * @details Coloca a coluna, a linha, a região, as possibilidades e as diagonais (se pertencer) da célula passada a proibido
 * @param list
 * @param k
 * @param row
 * @param col
 */
void sudokuFillCell(ListSudoku list, int k, int row, int col) {
    int region_size = sqrt(list.total);
    for (int i = 0; i < list.total; i++) {
        *(*((list.sudokus + k - 1)->board + row) + i) = 1;
        *(*((list.sudokus + k - 1)->board + i) + col) = 1;
        *(*((list.sudokus + i)->board + row) + col) = 1;
        if (list.total <= MAX_SUDOKUX_SIZE) {
            if (row == col) {
                *(*((list.sudokus + k - 1)->board + i) + i) = 1;
            }
            if (row == list.total - col - 1) {
                *(*((list.sudokus + k - 1)->board + i) + list.total - i - 1) = 1;
            }
        }
    }
    int rowStart = (row / region_size) * region_size;
    int colStart = (col / region_size) * region_size;

    for (int i = rowStart; i < rowStart + region_size; i++) {
        for (int j = colStart; j < colStart + region_size; j++) {
            *(*((list.sudokus + k - 1)->board + i) + j) = 1;
        }
    }
}

/**
 * @brief Verificar se a posição é válida
 * @details Testa a linha , a coluna, a região, possibilidades e diagonais (se pertencer)
 * @param list
 * @param k
 * @param row
 * @param col
 * @return
 */
int checkSudokuCell(ListSudoku list, int k, int row, int col, int size) {
    int region_size = sqrt(size);
    int count[6] = {0}, rowStart = (row / region_size) * region_size, colStart = (col / region_size) * region_size;
    for (int i = 0; i < size; i++) {
        if (*(*((list.sudokus + k - 1)->board + row) + i) == 0) {
            count[0]++;
        }
        if (*(*((list.sudokus + k - 1)->board + i) + col) == 0) {
            count[1]++;
        }
        if (size <= MAX_SUDOKUX_SIZE) {
            if (row == col && *(*((list.sudokus + k - 1)->board + i) + i) == 0) {
                count[2]++;
            }
            if (row == size - col - 1 && *(*((list.sudokus + k - 1)->board + i) + list.total - i - 1) == 0) {
                count[3]++;
            }
        }
        if (*(*((list.sudokus + i)->board + row) + col) == 0) {
            count[4]++;
        }
    }
    for (int i = rowStart; i < rowStart + region_size; i++) {
        for (int j = colStart; j < colStart + region_size; j++) {
            if (*(*((list.sudokus + k - 1)->board + i) + j) == 0) {
                count[5]++;
            }
        }
    }
    return count[0] == 1 || count[1] == 1 || count[2] == 1 || count[3] == 1 || count[4] == 1 || count[5] == 1;
}

/**
 * @brief Algoritmo Otimizado
 * @details Algoritmo Otimizado usado na resolução de tabuleiros. Se o algoritmo não conseguir cumprir por si próprio, este chama o algoritmo Bruteforce
 * @param s
 * @param solved
 * @param cost
 */
void solveSudokuOptimized(Sudoku unsolved, ListSudoku *solved, long long *cost) {
    ListSudoku cube;
    Sudoku s;
    s.size = unsolved.size;
    s.board = createBoard(s.size);
    for(int row = 0; row < s.size; row++) {
        for(int col = 0; col < s.size; col++) {
            *(*(s.board + row) + col) = *(*(unsolved.board + row) + col);
        }
    }

    cube.sudokus = NULL;
    cube.sudokus = resizeSudokus(cube.sudokus, 0, s.size);
    cube.total = s.size;
    for (int i = 0; i < s.size; i++) {
        (cube.sudokus + i)->size = s.size;
        (cube.sudokus + i)->board = createBoard(s.size);
    }

    int count = 0, prev_count = 0, try_pairs = 1;
    //percorrer tabuleiro original
    for (int i = 0; i < s.size; i++) {
        for (int j = 0; j < s.size; j++) {
            if (*(*(s.board + i) + j) != UNASSIGNED) {
                count++;
                sudokuFillCell(cube, *(*(s.board + i) + j), i, j);
            }
        }
    }
    while (prev_count != count && count < s.size * s.size) {
        prev_count = count;

        for (int row = 0; row < s.size; row++) {
            for (int col = 0; col < s.size; col++) {
                if (*(*(s.board + row) + col) == UNASSIGNED) {
                    for (int number = 1; number <= s.size; number++, (*cost)++) {
                        if (*(*((cube.sudokus + number - 1)->board + row) + col) == UNASSIGNED) {
                            (*cost)++;
                            if (checkSudokuCell(cube, number, row, col, s.size)) {
                                *(*(s.board + row) + col) = number;
                                sudokuFillCell(cube, number, row, col);
                                count++;
                                try_pairs = 1;
                            }
                        }
                    }
                }
            }
        }
        if (prev_count == count) {
            (*cost)++;
            if (try_pairs && findPairs(cube)) {
                try_pairs = 0;
                prev_count--;
            }
        }

    }

    for (int i = 0; i < cube.total; i++)
        free((cube.sudokus + i)->board);
    free(cube.sudokus);

    if (count != s.size * s.size) {
        printf("Cost %lld - Otimizado nao encontrou solucoes, ir para o bruteforce\n", *cost);
    } else {
        printf("Solucao:\n");
        printSudoku(s.board, s.size);
    }
    solveSudokuBruteForce(solved, s, 0, 0, cost);
}

/**
 * @brief Procurar pares de numeros no tabuleiro
 * @param cube
 */
int findPairs(ListSudoku cube) {
    int size = (cube.sudokus)->size, count = 0, found;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            count = 0;
            for (int num = 0; num < size; num++) {
                if (*(*((cube.sudokus + num)->board + row) + col) == UNASSIGNED) {
                    count++;
                }
            }
            if (count == 2) {
                for (int row_pair = 0; row_pair < size; row_pair++) {
                    if (row != row_pair) {
                        found = 1;
                        for (int i = 0; i < size; i++) {
                            if (*(*((cube.sudokus + i)->board + row) + col) != *(*((cube.sudokus + i)->board + row_pair) + col)) {
                                found = 0;
                                break;
                            }
                        }
                        if (found) {
                            for (int row_remove = 0; row_remove < size; row_remove++) {
                                if (row_remove != row && row_remove != row_pair) {
                                    for (int num_remove = 0; num_remove < size; num_remove++) {
                                        if (*(*((cube.sudokus + num_remove)->board + row) + col) == 0) {
                                            *(*((cube.sudokus + num_remove)->board + row_remove) + col) = 1;
                                        }
                                    }
                                }
                            }
                            return 1;
                        }
                    }
                }
                for (int col_pair = 0; col_pair < size; col_pair++) {
                    if (col != col_pair) {
                        found = 1;
                        for (int i = 0; i < size; i++) {
                            if (*(*((cube.sudokus + i)->board + row) + col) != *(*((cube.sudokus + i)->board + row) + col_pair)) {
                                found = 0;
                                break;
                            }
                        }
                        if (found) {
                            for (int col_remove = 0; col_remove < size; col_remove++) {
                                if (col_remove != row && col_remove != col_pair) {
                                    for (int num_remove = 0; num_remove < size; num_remove++) {
                                        if (*(*((cube.sudokus + num_remove)->board + row) + col) == 0) {
                                            *(*((cube.sudokus + num_remove)->board + row) + col_remove) = 1;
                                        }
                                    }
                                }
                            }
                            return 1;
                        }
                    }
                }
                if (row == col && size <= MAX_SUDOKUX_SIZE) {
                    for (int diag_pair = 0; diag_pair < size; diag_pair++) {
                        if (row != diag_pair && col != diag_pair) {
                            found = 1;
                            for (int i = 0; i < size; i++) {
                                if (*(*((cube.sudokus + i)->board + row) + col) != *(*((cube.sudokus + i)->board + diag_pair) + diag_pair)) {
                                    found = 0;
                                    break;
                                }
                            }
                            if (found) {
                                for (int diag_remove = 0; diag_remove < size; diag_remove++) {
                                    if (diag_remove != row && diag_remove != diag_pair) {
                                        for (int num_remove = 0; num_remove < size; num_remove++) {
                                            if (*(*((cube.sudokus + num_remove)->board + row) + col) == 0) {
                                                *(*((cube.sudokus + num_remove)->board + diag_remove) + diag_remove) = 1;
                                            }
                                        }
                                    }
                                }
                                return 1;
                            }
                        }
                    }
                }

                if (row + col == size - 1 && size <= MAX_SUDOKUX_SIZE) {
                    for (int diagsec_pair = 0; diagsec_pair < size; diagsec_pair++) {
                        if (row != diagsec_pair && col != size - diagsec_pair - 1) {
                            found = 1;
                            for (int i = 0; i < size; i++) {
                                if (*(*((cube.sudokus + i)->board + row) + col) !=
                                    *(*((cube.sudokus + i)->board + diagsec_pair) + size - diagsec_pair - 1)) {
                                    found = 0;
                                    break;
                                }
                            }
                            if (found) {
                                for (int diagsec_remove = 0; diagsec_remove < size; diagsec_remove++) {
                                    if (diagsec_remove != row && diagsec_remove != diagsec_pair) {
                                        for (int num_remove = 0; num_remove < size; num_remove++) {
                                            if (*(*((cube.sudokus + num_remove)->board + row) + col) == 0) {
                                                *(*((cube.sudokus + num_remove)->board + diagsec_remove) + size - diagsec_remove - 1) = 1;
                                            }
                                        }
                                    }
                                }
                                return 1;
                            }
                        }
                    }
                }
                int region_size = sqrt(size);
                int rowStart = (row / region_size) * region_size, colStart = (col / region_size) * region_size;
                for (int i = rowStart; i < rowStart + region_size; i++) {
                    for (int j = colStart; j < colStart + region_size; j++) {
                        if (i != row && j != col) {
                            found = 1;
                            for (int num = 0; num < size; num++) {
                                if (*(*((cube.sudokus + num)->board + row) + col) != *(*((cube.sudokus + num)->board + i) + j)) {
                                    found = 0;
                                    break;
                                }
                            }
                            if (found) {
                                for (int i_remove = rowStart; i_remove < rowStart + region_size; i_remove++) {
                                    for (int j_remove = colStart; j_remove < colStart + region_size; j_remove++) {
                                        if (i_remove != row && j_remove != col && i_remove != i && j_remove != j) {
                                            for (int num_remove = 0; num_remove < size; num_remove++) {
                                                if (*(*((cube.sudokus + num_remove)->board + row) + col) == 0) {
                                                    *(*((cube.sudokus + num_remove)->board + i_remove) + j_remove) = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/**
 * @brief Compara 2 tabuleiros sudoku
 * @details
 * @param pattern
 * @param unsolved
 * @return 1 - Verdadeiro / 0 - Falso
 */
int isPattern(Sudoku pattern, Sudoku unsolved) {
    if (pattern.size != unsolved.size) {
        return 0;
    }
    for (int i = 0; i < pattern.size; i++) {
        for (int j = 0; j < unsolved.size; j++) {
            if (*(*(pattern.board + i) + j) != *(*(unsolved.board + i) + j) && *(*(unsolved.board + i) + j) > 0) {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * @brief Procura um Sudoku numa lista de soluções
 * @details
 * @param searchList
 * @param sudoku
 * @return
 */
int searchSudokus(ListSudoku searchList, Sudoku sudoku) {
    for (int i = 0; i < searchList.total; i++) {
        if (isPattern(*(searchList.sudokus + i), sudoku)) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Ordena os tabuleiros por tamanho
 * @details Cria um índice de forma a ordenar eficazmente os tabuleiros
 * @param a
 */
void computeOrderBySize(ListSudoku *a) {
    int i, r, c;
    int R = 100;
    int count[100] = {0};

    //Frequência absoluta.
    for (i = 0; i < a->total; i++)
        count[(a->sudokus + i)->size]++;

    // Frequências acumuladas.
    for (r = 0; r < R; r++)
        count[r + 1] += count[r];

    // Distribuir
    for (i = 0; i < a->total; i++) {
        c = (a->sudokus + i)->size - 1;
        *(a->orderedList + count[c]) = i;
        count[c]++;
    }
}
