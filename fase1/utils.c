#define _POSIX_C_SOURCE 200809L

#include "utils.h"
#include "algorithms.h"
#include "../common/sudoku_common.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Imprime o tabuleiro passado
 * @param sudoku
 * @param size
 */
void printSudoku(int **sudoku, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (*(*(sudoku + i) + j) > 9) {
                printf("%d ", *(*(sudoku + i) + j));
            } else {
                printf(" %d ", *(*(sudoku + i) + j));
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * @brief Imprime a lista de sudokus passada, de forma ordenada ou não
 * @param s
 * @param flagOrdered
 */
void printAllStoredBoards(SudokuList s, int flagOrdered) {
    int z;
    for (int k = 0; k < s.total; k++) {
        if (flagOrdered) {
            z = *(s.orderedList + k);
        } else {
            z = k;
        }
        printf("\n %d:", k + 1);
        for (int i = 0; i < (s.sudokus + z)->size; i++) {
            printf("\n");
            for (int j = 0; j < (s.sudokus + z)->size; j++) {
                if (*(*((s.sudokus + z)->board + i) + j) > 9) {
                    printf("%d ", *(*((s.sudokus + z)->board + i) + j));

                } else {
                    printf(" %d ", *(*((s.sudokus + z)->board + i) + j));

                }
            }
        }
        printf("\n");
    }
}

/**
 * @brief Aumenta uma Lista de sudokus
 * @param ptr
 * @param size
 * @param newSize
 * @return
 */
Sudoku *resizeSudokus(Sudoku *ptr, int size, int newSize) {
    (void) size; // realloc preserves the existing elements
    return (Sudoku *) xreallocarray(ptr, (size_t) newSize, sizeof(Sudoku));
}

/**
 * @brief Aumenta uma lista de inteiros
 * @param pList
 * @param size
 * @param newSize
 * @return
 */
int *resizeList(int *pList, int size, int newSize) {
    (void) size; // realloc preserves the existing elements
    return (int *) xreallocarray(pList, (size_t) newSize, sizeof(int));
}


/**
 * @brief Cria uma matriz para guardar o novo tabuleiro
 * @param size
 * @return
 */
int **createBoard(int size) {
    int **pAux = (int **) xcalloc((size_t) size, sizeof(int *));

    for (int i = 0; i < size; i++) {
        *(pAux + i) = (int *) xcalloc((size_t) size, sizeof(int));
    }
    return pAux;
}

/**
 * @brief Liberta a matriz de um tabuleiro
 * @param board
 * @param size
 */
void freeBoard(int **board, int size) {
    if (board == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(*(board + i));
    }
    free(board);
}

/**
 * @brief Cria uma cópia independente (deep copy) de um tabuleiro
 * @param sudoku
 * @return
 */
Sudoku copySudoku(Sudoku sudoku) {
    Sudoku copy;
    copy.size = sudoku.size;
    copy.board = createBoard(sudoku.size);
    for (int i = 0; i < sudoku.size; i++) {
        for (int j = 0; j < sudoku.size; j++) {
            *(*(copy.board + i) + j) = *(*(sudoku.board + i) + j);
        }
    }
    return copy;
}

/**
 * @brief Inicializa o gerador de números aleatórios apenas uma vez
 */
void seedRandomOnce(void) {
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned) time(NULL));
        seeded = 1;
    }
}

/**
 * @brief Gera um sudoku
 * @param size
 * @param n
 * @return
 */
Sudoku gen_sudoku(int size, int n) {
    seedRandomOnce();
    int row, col, num;
    Sudoku s;
    s.size = size;
    s.board = createBoard(size);

    // Limitar o número de células e de tentativas: sem limite, pedidos impossíveis (ex.: mais células do que o
    // tabuleiro permite) ficavam num ciclo infinito.
    if (n > size * size) {
        n = size * size;
    }
    long long attempts = 0;
    const long long maxAttempts = 1000LL * size * size;
    int placed = 0;
    while (placed < n && attempts < maxAttempts) {
        attempts++;
        row = rand() % size;
        col = rand() % size;
        num = (rand() % size) + 1;
        if (*(*(s.board + row) + col) == 0 && isValidPlacement(s, num, row, col)) {
            *(*(s.board + row) + col) = num;
            placed++;
        }
    }
    if (placed < n) {
        fprintf(stderr, "Aviso: so foi possivel gerar %d de %d celulas\n", placed, n);
    }
    return s;
}

/**
 * @brief Compara 2 tabuleiros
 * @param s1
 * @param s2
 * @return 1 - Verdadeiro / 0 - Falso
 */
int isEqual(Sudoku s1, Sudoku s2) {
    if (s1.size != s2.size) {
        return 0;
    }
    for (int i = 0; i < s1.size; i++) {
        for (int j = 0; j < s2.size; j++) {
            if (*(*(s1.board + i) + j) != *(*(s2.board + i) + j)) {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * @brief Cria uma lista de sudokus distintos
 * @param target
 * @param source
 * @return
 */
SudokuList merge_sudokus(SudokuList target, SudokuList source) {
    int alreadyExists;
    for (int i = 0; i < source.total; i++) {
        alreadyExists = 0;
        for (int j = 0; j < target.total; j++) {
            if (isEqual(*(target.sudokus + j), *(source.sudokus + i))) {
                alreadyExists = 1;
            }
        }
        if (alreadyExists == 0) {
            // Copia profunda: a lista de origem continua a ser dona dos seus tabuleiros, evitando double free
            target.sudokus = resizeSudokus(target.sudokus, target.total, target.total + 1);
            target.orderedList = resizeList(target.orderedList, target.total, target.total + 1);
            *(target.sudokus + target.total) = copySudoku(*(source.sudokus + i));
            *(target.orderedList + target.total) = target.total;
            target.total++;
        }
    }
    return target;
}


/**
 * Liberta a memória
 * @param l
 */
void free_list_sudoku(SudokuList l) {
    for (int i = 0; i < l.total; i++) {
        freeBoard((l.sudokus + i)->board, (l.sudokus + i)->size);
    }
    free(l.sudokus);
    free(l.orderedList);
}

/**
 * Testa se o tabuleiro passado cumpre as regras do sudoku
 * @param sudoku
 * @return
 */
int isConsistent(Sudoku sudoku) {
    int auxCell;
    for (int i = 0; i < sudoku.size; i++) {
        for (int j = 0; j < sudoku.size; j++) {
            if (*(*(sudoku.board + i) + j) > 0) {
                auxCell = *(*(sudoku.board + i) + j);
                *(*(sudoku.board + i) + j) = 0;
                if (!isValidPlacement(sudoku, auxCell, i, j)) {
                    *(*(sudoku.board + i) + j) = auxCell;
                    return 0;
                }
                *(*(sudoku.board + i) + j) = auxCell;
            }
        }
    }
    return 1;
}
/**
 * @brief Obter o tempo atual (relógio monotónico) em microssegundos
 * @param time_usec
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int gettimeuseconds(long long *time_usec) {
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
        *time_usec = 0;
        return -1;
    }
    *time_usec = (long long) now.tv_sec * 1000000LL + (long long) (now.tv_nsec / 1000);
    return 0;
}
