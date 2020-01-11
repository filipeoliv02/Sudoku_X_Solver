#include "algorithms.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define UNASSIGNED 0

// Tamanho máximo para um tabuleiro de SudokuX, tabuleiros de tamanho superior são considerados como Sudoku normal
#define MAX_SUDOKUX_SIZE 16

int checkSudokuCell(SudokuList list, int k, int row, int col, int size);

void sudokuFillCell(SudokuList list, int k, int row, int col);

void update_solved(SudokuList *solved, Sudoku sudoku);

int existsInPrincipalDiagonal(Sudoku sudoku, int num, int row, int col);

int existsInSecundaryDiagonal(Sudoku sudoku, int num, int row, int col);

int existsInCol(Sudoku sudoku, int num, int col);

int existsInRow(Sudoku sudoku, int num, int row);

int existsInRegion(Sudoku sudoku, int num, int row, int col, int regionSize);

int findPairs(SudokuList cube);

void writePossibilities(SudokuLinkedNode sudoku);

int isValidPlacementLink(SudokuLinkedNode sudoku, Node *node, int num);

int checkNakedSingle(Node *node, int size);

int checkHiddenSingle(Node *node, int size);

Node *createPossibilities(SudokuLinkedNode sudoku);

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
void solveSudokuBruteForce(SudokuList *solved, Sudoku sudoku, int row, int col, long long *cost) {
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
void update_solved(SudokuList *solved, Sudoku sudoku) {
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
void sudokuFillCell(SudokuList list, int k, int row, int col) {
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
int checkSudokuCell(SudokuList list, int k, int row, int col, int size) {
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
void solveSudokuOptimized(Sudoku unsolved, SudokuList *solved, long long *cost) {
    SudokuList cube;
    Sudoku s;
    s.size = unsolved.size;
    s.board = createBoard(s.size);
    for (int row = 0; row < s.size; row++) {
        for (int col = 0; col < s.size; col++) {
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
int findPairs(SudokuList cube) {
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
                            if (*(*((cube.sudokus + i)->board + row) + col) !=
                                *(*((cube.sudokus + i)->board + row_pair) + col)) {
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
                            if (*(*((cube.sudokus + i)->board + row) + col) !=
                                *(*((cube.sudokus + i)->board + row) + col_pair)) {
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
                                if (*(*((cube.sudokus + i)->board + row) + col) !=
                                    *(*((cube.sudokus + i)->board + diag_pair) + diag_pair)) {
                                    found = 0;
                                    break;
                                }
                            }
                            if (found) {
                                for (int diag_remove = 0; diag_remove < size; diag_remove++) {
                                    if (diag_remove != row && diag_remove != diag_pair) {
                                        for (int num_remove = 0; num_remove < size; num_remove++) {
                                            if (*(*((cube.sudokus + num_remove)->board + row) + col) == 0) {
                                                *(*((cube.sudokus + num_remove)->board + diag_remove) +
                                                  diag_remove) = 1;
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
                                                *(*((cube.sudokus + num_remove)->board + diagsec_remove) + size -
                                                  diagsec_remove - 1) = 1;
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
                                if (*(*((cube.sudokus + num)->board + row) + col) !=
                                    *(*((cube.sudokus + num)->board + i) + j)) {
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
int searchSudokus(SudokuList searchList, Sudoku sudoku) {
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
void computeOrderBySize(SudokuList *a) {
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

void solveSudokuBruteForceLink(SudokuLinkedNode queue, Node *node) {
    if (node == NULL) {
        printf("Solucao:\n");
        print_linked_board(queue);
        return;
    }

    Node *newNode = node;
    if (newNode->e == NULL) {
        while (newNode->w != NULL) newNode = newNode->w;
        newNode = newNode->s;
    } else {
        newNode = newNode->e;
    }

    if (node->num > 0) {
        solveSudokuBruteForceLink(queue, newNode);
    } else {
        for (int num = 1; num <= queue.size; num++) {
            if (isValidPlacementLink(queue, node, num)) {
                node->num = num;
                solveSudokuBruteForceLink(queue, newNode);
            }
        }
        node->num = 0;
    }
}

int isValidPlacementLink(SudokuLinkedNode sudoku, Node *node, int num) {
    Node *nodeAux = NULL;
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->s;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->n;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->w;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->e;
    }
    //diagonais
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->nw;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->se;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->ne;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->sw;
    }
    //Regiões

    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->fbox;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->bbox;
    }
    return 1;
}


void solveSudokuOptimizedLink(SudokuLinkedNode sudoku) {
    Node *origin = createPossibilities(sudoku);
    print_linked_boards(origin, sudoku.size);
    int count = 0, found = 1;

    while (found) {
        found = 0;

        if (stratSingles(origin, sudoku.first)) {
            count++;
            found = 1;
        } else if (stratIntersectionRemoval(origin) || stratNakedGroups(origin)) {
            found = 1;
        }

        print_linked_boards(origin, sudoku.size);
    }

    //solveSudokuBruteForceLink(sudoku, sudoku.first);

    print_linked_boards(origin, sudoku.size);

    printf("Solucao - avancado linked (%d):\n", count);
    print_linked_board(sudoku);
}


int stratSingles(Node *origin, Node *first) {
    return checkSingles(origin, first, nodeE)       // Hidden Single na linha
           || checkSingles(origin, first, nodeS)    // Hidden Single na coluna
           || checkSingles(origin, first, nodeFBOX) // Hidden Single na região
           || checkSingles(origin, first, nodeSE)   // Hidden Single na diagonal principal
           || checkSingles(origin, first, nodeSW)   // Hidden Single na diagonal secundária
           || checkSingles(origin, first, nodeUP);  // Naked Single
}

int stratIntersectionRemoval(Node *origin) {
    return checkIntersectionRemoval(origin, nodeS, nodeFBOX, nodeBBOX, isSameBox)     // Box/Line reduct. na coluna
           || checkIntersectionRemoval(origin, nodeE, nodeFBOX, nodeBBOX, isSameBox)  // Box/Line reduct. na linha
           || checkIntersectionRemoval(origin, nodeSE, nodeFBOX, nodeBBOX, isSameBox) // Box/Line reduct. na diag. p.
           || checkIntersectionRemoval(origin, nodeSW, nodeFBOX, nodeBBOX, isSameBox) // Box/Line reduct. na diag. s.
           || checkIntersectionRemoval(origin, nodeFBOX, nodeS, nodeN, isSameCol)     // Pointing pairs na coluna
           || checkIntersectionRemoval(origin, nodeFBOX, nodeE, nodeW, isSameRow)     // Pointing pairs na linha
           || checkIntersectionRemoval(origin, nodeFBOX, nodeSE, nodeNW, isSamePDiag) // Pointing pairs na diag. p.
           || checkIntersectionRemoval(origin, nodeFBOX, nodeSW, nodeNE, isSameSDiag);// Pointing pairs na diag. s.
}

#define FLAG_DYNAMIC_GROUPS 0

int stratNakedGroups(Node *origin) {
    int maxGroups = FLAG_DYNAMIC_GROUPS ? origin->num / 2 : 4;
    for (int groupSize = 2; groupSize <= maxGroups; groupSize++) {
        if (checkNakedGroups(origin, groupSize, nodeE, nodeW)
            || checkNakedGroups(origin, groupSize, nodeS, nodeN)
            || checkNakedGroups(origin, groupSize, nodeSE, nodeNW)
            || checkNakedGroups(origin, groupSize, nodeSW, nodeNE)
            || checkNakedGroups(origin, groupSize, nodeFBOX, nodeBBOX)) {
            return 1;
        }
    }
    return 0;
}

int checkSingles(Node *origin, Node *first, Node *(*nextNode)(Node *)) {
    Node *rule = nextNode(origin)->fRule, *node;
    while (rule != NULL) {
        node = nextNode(rule);
        if (node != NULL && nextNode(node) == NULL) {
            //printf("Single: %d (%d, %d)\n", node->num, node->row, node->col);
            addNumber(first, node);
            removeConnectedNodes(node);
            free(node);
            return 1;
        }
        rule = rule->fRule;
    }
    return 0;
}

int checkIntersectionRemoval(Node *origin, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                             Node *(*prevRemoveNode)(Node *), int (*isSameUnit)(Node *, Node *, int)) {
    Node *rule = nextNode(origin)->fRule, *node;
    int found;
    while (rule != NULL) {
        found = 0;

        // Procurar interseção
        node = nextNode(rule);
        while (nextNode(node) != NULL) {
            found = 1;
            if (!isSameUnit(node, nextNode(node), origin->num)) {
                found = 0;
                break;
            }
            node = nextNode(node);
        }

        // Se interseção encontrada, remover os outros nós
        if (found && clearIntersection(rule, nextNode, nextRemoveNode, prevRemoveNode)) {
            //printf("Intersect Rmv: %d (%d, %d)\n", nextNode(rule)->num, nextNode(rule)->row, nextNode(rule)->col);
            return 1;
        }

        rule = rule->fRule;
    }
    return 0;
}

int checkNakedGroups(Node *origin, int groupSize, Node *(*nextNode)(Node *), Node *(*prevNode)(Node *)) {
    Node *rule = origin->up->fRule, *node;
    int *dict = (int *) calloc(origin->num + 1, sizeof(int)), count = 0;

    while (rule != NULL) {
        for (int i = 0; i <= origin->num; i++) {
            *(dict + i) = 0;
        }
        node = rule->up;
        while (node != NULL) {
            (*(dict + node->num))++;
            (*dict)++;
            node = node->up;
        }

        if (findGroup(rule, dict, groupSize, groupSize - 1, &count, nextNode)) {
            // Remover
            count = removeGroupNodes(rule, NULL, dict, prevNode) + count;
            /*node = prevNode(rule);
            while (node != NULL) {
                count++;
                nodeRemove = node->up;
                while (nodeRemove != NULL) {
                    nodeAux = nodeRemove->up;
                    if (*(dict + nodeRemove->num - 1) != 0) {
                        disconnectNode(nodeRemove);
                        free(nodeRemove);
                    }
                    nodeRemove = nodeAux;
                }
                node = prevNode(node);
            }

            node = nextNode(rule);
            while (node != ruleAux) {
                count++;
                nodeRemove = node->up;
                while (nodeRemove != NULL) {
                    nodeAux = nodeRemove->up;
                    if (*(dict + nodeRemove->num - 1) != 0) {
                        disconnectNode(nodeRemove);
                        free(nodeRemove);
                    }
                    nodeRemove = nodeAux;
                }
                node = nextNode(node);
            }

            node = nextNode(ruleAux);
            while (node != NULL) {
                count++;
                nodeRemove = node->up;
                while (nodeRemove != NULL) {
                    nodeAux = nodeRemove->up;
                    if (*(dict + nodeRemove->num - 1) != 0) {
                        disconnectNode(nodeRemove);
                        free(nodeRemove);
                    }
                    nodeRemove = nodeAux;
                }
                node = nextNode(node);
            }*/
            if(count > 0) {
                //printf("<-------------------> Group %d, %d: (%d,%d)\n", rule->up->num, rule->up->up->num, rule->up->row, rule->up->col);
                return 1;
            }

        }
        rule = rule->fRule;
    }
    return 0;
}

int findGroup(Node *rule, int *dict, int total, int num, int *count, Node *(*nextNode)(Node *)) {
    if (*dict > total || num == 0) {
        if(*dict == total && num == 0) {
            *count = removeGroupNodes(rule, NULL, dict, nextNode) + (*count);
            return 1;
        }
        else {
            return 0;
        }
    }

    Node *node, *nextRule = nextNode(rule);

    while (nextRule != NULL) {
        node = nextRule->up;
        while (node != NULL) {
            if (*(dict + node->num) == 0) {
                (*dict)++;
            }
            (*(dict + node->num))++;
            node = node->up;
        }

        if (findGroup(nextRule, dict, total, num - 1, count, nextNode)) {
            *count = removeGroupNodes(rule, nextRule, dict, nextNode) + (*count);
            return 1;
        }

        node = nextRule->up;
        while (node != NULL) {
            (*(dict + node->num))--;
            if (*(dict + node->num) == 0) {
                (*dict)--;
            }
            node = node->up;
        }

        nextRule = nextNode(nextRule);
    }

    return 0;
}

int removeGroupNodes(Node *ruleStart, Node *ruleStop, const int *dict, Node *(*nextNode)(Node *)) {
    Node *rule = nextNode(ruleStart), *node, *nodeNext;
    int count = 0;

    while (rule != ruleStop) {
        node = rule->up;
        while (node != NULL) {
            nodeNext = node->up;
            if (*(dict + node->num) != 0) {
                count++;
                disconnectNode(node);
                free(node);
            }
            node = nodeNext;
        }

        rule = nextNode(rule);
    }

    return count;
}

void addNumber(Node *first, Node *new) {
    Node *node = first;
    for (int row = 0; row < new->row; row++) {
        node = node->s;
    }
    for (int col = 0; col < new->col; col++) {
        node = node->e;
    }

    node->num = new->num;
}

int clearIntersection(Node *nodeRule, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                      Node *(*prevRemoveNode)(Node *)) {
    int count = 0;
    Node *node, *nodeAux;

    // Começar na unit onde se vai remover
    node = nextNode(nodeRule);
    while (prevRemoveNode(node) != NULL) node = prevRemoveNode(node);

    // Remover todos os nós entre dois nós
    nodeAux = nextNode(nodeRule);
    while (node != NULL) {
        count = removeNodesBetweenTwoNodes(node, nodeAux, nextRemoveNode) + count;
        node = nodeAux;
        if (nodeAux != NULL) {
            nodeAux = nextNode(nodeAux);
        }
    }

    return count;
}

Node *createPossibilities(SudokuLinkedNode sudoku) {
    Node *node, *node_line = NULL, *node_prevline = NULL, *node_prev = NULL, *node_layer = NULL, *nodeAux = NULL;
    Node *node_sudoku, *node_sudoku_line;
    int root = (int) sqrt(sudoku.size), rrow, rcol;

    Node *origin = (Node *) calloc(1, sizeof(Node));
    Node *row = (Node *) calloc(1, sizeof(Node)), *col = (Node *) calloc(1, sizeof(Node));
    Node *box = (Node *) calloc(1, sizeof(Node)), *number = (Node *) calloc(1, sizeof(Node));
    Node *pdiag = (Node *) calloc(1, sizeof(Node)), *sdiag = (Node *) calloc(1, sizeof(Node));

    origin->num = sudoku.size;
    origin->e = row;
    origin->s = col;
    origin->fbox = box;
    origin->se = pdiag;
    origin->sw = sdiag;
    origin->up = number;

    for (int num = 0; num <= sudoku.size; num++) {

        node_prevline = NULL;
        node_line = NULL;

        for (int i = 0; i < sudoku.size; i++) {
            for (int j = 0; j < sudoku.size; j++) {
                node = (Node *) calloc(1, sizeof(Node));

                node->num = num;
                node->row = i;
                node->col = j;

                // Se não existir um primeiro nó então é este
                if (i == 0 && j == 0 && num == 0) {
                    node_layer = node;
                }

                // Ligar linha
                if (j != 0) {
                    // Não está na primeira coluna, ligar ao nó anterior
                    node_prev->e = node;
                    node->w = node_prev;
                    node_prev = node_prev->e;
                } else {
                    // Está na primeira coluna, não existe um nó anterior, associa-se
                    node_prev = node;

                    // Ligar à unidade das linhas
                    if (num > 0) {
                        row->fRule = (Node *) calloc(1, sizeof(Node));
                        row->fRule->bRule = row;
                        row = row->fRule;

                        row->e = node;
                        node->w = row;
                    }
                }

                // Ligar coluna
                if (i != 0) {
                    // Não está na primeira linha, ligar ao nó anterior
                    node_prevline->s = node;
                    node->n = node_prevline;
                    node_prevline = node_prevline->e;
                } else if (num > 0) {
                    // Está na primeira linha, ligar à unidade das colunas
                    col->fRule = (Node *) calloc(1, sizeof(Node));
                    col->fRule->bRule = col;
                    col = col->fRule;

                    col->s = node;
                    node->n = col;
                }

                // Ligar entre camadas, se estiver acima da primeira
                if (num > 0) {
                    if (i == 0 && j == 0) {
                        node->down = node_layer;
                        node_layer = node;
                    } else if (j == 0) {
                        node->down = node->n->down->s;
                    } else {
                        node->down = node->w->down->e;
                    }
                    node->down->up = node;
                }

                // Ligar a regra dos números únicos se estiver na primeira camada
                if (num == 0) {
                    number->fRule = node;
                    number->fRule->bRule = number;
                    number = number->fRule;
                }

                // Ligar diagonais se o tabuleiro for do tamanho do SudokuX
                if (sudoku.size <= 16) {

                    // Diagonal principal
                    if (i == j) {
                        if (i != 0) {
                            // Não está na primeira linha, ligar ao nó anterior
                            node->nw = node->w->n;
                            node->nw->se = node;
                        } else if (num > 0) {
                            // Está na primeira linha, ligar à unidade das diagonais principais
                            pdiag->fRule = (Node *) calloc(1, sizeof(Node));
                            pdiag->fRule->bRule = pdiag;
                            pdiag = pdiag->fRule;

                            pdiag->se = node;
                            node->nw = pdiag;
                        }
                    }

                    // Diagonal secundária
                    if (i == sudoku.size - j - 1) {
                        if (i != 0) {
                            // Não está na primeira linha, ligar ao nó anterior
                            node->ne = node->n->e;
                            node->ne->sw = node;
                        } else if (num > 0) {
                            // Está na primeira linha, ligar à unidade das diagonais secundárias
                            sdiag->fRule = (Node *) calloc(1, sizeof(Node));
                            sdiag->fRule->bRule = sdiag;
                            sdiag = sdiag->fRule;

                            sdiag->sw = node;
                            node->ne = sdiag;
                        }
                    }
                }

                //Ligar Regiões
                rcol = j % root;
                rrow = i % root;
                nodeAux = node;
                if (rcol == 0 && rrow == 0) {
                    // Está no ínicio da região, ligar à unidade das regiões
                    if (num > 0) {
                        box->fRule = (Node *) calloc(1, sizeof(Node));
                        box->fRule->bRule = box;
                        box = box->fRule;

                        box->fbox = node;
                        node->bbox = box;
                    }
                } else {
                    // Não está no ínicio da região, ligar ao nó anterior da região
                    if (rcol == 0) {
                        nodeAux = nodeAux->n;
                        while (nodeAux->col % root != (root - 1)) {
                            nodeAux = nodeAux->e;
                        }
                        node->bbox = nodeAux;
                        nodeAux->fbox = node;
                    } else {
                        node->bbox = node->w;
                        node->w->fbox = node;
                    }
                }
            }

            //Se não existe linha associar
            if (i == 0) {
                node_line = node_layer;
            } else {
                node_line = node_line->s;
            }
            node_prevline = node_line;
        }
    }

    node_sudoku = sudoku.first;
    node_sudoku_line = sudoku.first;
    node = origin->up->fRule;
    while (node_sudoku_line != NULL) {
        while (node_sudoku != NULL) {
            if (node_sudoku->num != 0) {
                nodeAux = node;
                node = node->bRule;
                while (nodeAux->num != node_sudoku->num) {
                    nodeAux = nodeAux->up;
                }
                removeConnectedNodes(nodeAux);
                free(nodeAux);
            }
            node_sudoku = node_sudoku->e;
            node = node->fRule;
        }
        node_sudoku_line = node_sudoku_line->s;
        node_sudoku = node_sudoku_line;
    }

    return origin;
}

void removeConnectedNodes(Node *node) {
    removeNodesBetweenTwoNodes(node, NULL, nodeN);
    removeNodesBetweenTwoNodes(node, NULL, nodeS);
    removeNodesBetweenTwoNodes(node, NULL, nodeW);
    removeNodesBetweenTwoNodes(node, NULL, nodeE);
    removeNodesBetweenTwoNodes(node, NULL, nodeNE);
    removeNodesBetweenTwoNodes(node, NULL, nodeNW);
    removeNodesBetweenTwoNodes(node, NULL, nodeSE);
    removeNodesBetweenTwoNodes(node, NULL, nodeSW);
    removeNodesBetweenTwoNodes(node, NULL, nodeUP);
    removeNodesBetweenTwoNodes(node, NULL, nodeDOWN);
    removeNodesBetweenTwoNodes(node, NULL, nodeFBOX);
    removeNodesBetweenTwoNodes(node, NULL, nodeBBOX);
}

int removeNodesBetweenTwoNodes(Node *nodeStart, Node *nodeStop, Node *(*nextNode)(Node *)) {
    Node *nodeCurr = nextNode(nodeStart), *nodeNext;
    int count = 0;

    while (nodeCurr != nodeStop) {
        count++;
        nodeNext = nextNode(nodeCurr);
        disconnectNode(nodeCurr);
        free(nodeCurr);
        nodeCurr = nodeNext;
    }

    return count;
}

void disconnectNode(Node *node) {
    if (node->n != NULL) {
        node->n->s = node->s;
    }

    if (node->s != NULL) {
        node->s->n = node->n;
    }

    if (node->w != NULL) {
        node->w->e = node->e;
    }

    if (node->e != NULL) {
        node->e->w = node->w;
    }

    if (node->nw != NULL) {
        node->nw->se = node->se;
    }

    if (node->ne != NULL) {
        node->ne->sw = node->sw;
    }

    if (node->se != NULL) {
        node->se->nw = node->nw;
    }

    if (node->sw != NULL) {
        node->sw->ne = node->ne;
    }

    if (node->bbox != NULL) {
        node->bbox->fbox = node->fbox;
    }

    if (node->fbox != NULL) {
        node->fbox->bbox = node->bbox;
    }

    if (node->down != NULL) {
        node->down->up = node->up;
    }

    if (node->up != NULL) {
        node->up->down = node->down;
    }

    if (node->bRule != NULL) {
        node->bRule->fRule = node->fRule;
    }

    if (node->fRule != NULL) {
        node->fRule->bRule = node->bRule;
    }
}

int isSameRow(Node *node1, Node *node2, int size) {
    return node1->row == node2->row;
}

int isSameCol(Node *node1, Node *node2, int size) {
    return node1->col == node2->col;
}

int isSamePDiag(Node *node1, Node *node2, int size) {
    return node1->col == node1->row && node2->col == node2->row;
}

int isSameSDiag(Node *node1, Node *node2, int size) {
    return node1->row == size - node1->col - 1 && node2->row == size - node2->col - 1;
}

int isSameBox(Node *node1, Node *node2, int size) {
    int root = sqrt(size);
    return node1->row / root == node2->row / root && node1->col / root == node2->col / root;
}


Node *nodeN(Node *node) {
    return node->n;
}

Node *nodeS(Node *node) {
    return node->s;
}

Node *nodeW(Node *node) {
    return node->w;
}

Node *nodeE(Node *node) {
    return node->e;
}

Node *nodeNE(Node *node) {
    return node->ne;
}

Node *nodeNW(Node *node) {
    return node->nw;
}

Node *nodeSE(Node *node) {
    return node->se;
}

Node *nodeSW(Node *node) {
    return node->sw;
}

Node *nodeUP(Node *node) {
    return node->up;
}

Node *nodeDOWN(Node *node) {
    return node->down;
}

Node *nodeFBOX(Node *node) {
    return node->fbox;
}

Node *nodeBBOX(Node *node) {
    return node->bbox;
}

Node *nodeFRULE(Node *node) {
    return node->fRule;
}

Node *nodeBRULE(Node *node) {
    return node->bRule;
}

