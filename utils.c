#include "utils.h"
#include "algorithms.h"
#include <time.h>
#include <sys/time.h>
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
void printAllStoredBoards(ListSudoku s, int flagOrdered) {
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
 * @brief Imprime o tabuleiro (usando linked lists) passado
 * @param board
 */
void print_linked_board(SUDOKU_QUEUE board) {
    NODE *node = board.pfirst, *node_line = board.pfirst;
    while(node_line != NULL) {
        while(node != NULL) {
            printf(" %d ", node->info);
            node = node->pe;
        }
        printf("\n");
        node_line = node_line->ps;
        node = node_line;
    }
    printf("\n\n");
}

/**
 * @brief Aumenta uma Lista de sudokus
 * @param ptr
 * @param size
 * @param newSize
 * @return
 */
Sudoku *resizeSudokus(Sudoku *ptr, int size, int newSize) {
    Sudoku *pAux = (Sudoku *) malloc(newSize * sizeof(Sudoku));

    for (int i = 0; i < size; i++) {
        *(pAux + i) = *(ptr + i);
    }
    free(ptr);

    return pAux;
}

/**
 * @brief Aumenta uma lista de inteiros
 * @param pList
 * @param size
 * @param newSize
 * @return
 */
int *resizeList(int *pList, int size, int newSize) {

    int *pAux = (int *) malloc(newSize * sizeof(int));

    for (int i = 0; i < size; i++) {
        *(pAux + i) = *(pList + i);
    }
    free(pList);

    return pAux;
}


/**
 * @brief Cria uma matriz para guardar o novo tabuleiro
 * @param size
 * @return
 */
int **createBoard(int size) {
    int **pAux = (int **) malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++) {
        *(pAux + i) = (int *) calloc(size, sizeof(int));
    }
    return pAux;
}

/**
 * @brief Gera um sudoku
 * @param size
 * @param n
 * @return
 */
Sudoku gen_sudoku(int size, int n) {
    srand(time(NULL));
    int row, col, num;
    Sudoku s;
    s.board = NULL;
    s.size = size;
    s.board = createBoard(size);
    for (int i = 0; i < n; i++) {
        row = rand() % size;
        col = rand() % size;
        num = (rand() % size) + 1;
        if (isValidPlacement(s, num, row, col) && *(*(s.board + row) + col) == 0) {
            *(*(s.board + row) + col) = num;

        } else {
            i--;
        }
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
ListSudoku merge_sudokus(ListSudoku target, ListSudoku source) {
    int alreadyExists;
    for (int i = 0; i < source.total; i++) {
        alreadyExists = 0;
        for (int j = 0; j < target.total; j++) {
            if (isEqual(*(target.sudokus + j), *(source.sudokus + i))) {
                alreadyExists = 1;
            }
        }
        if (alreadyExists == 0) {
            target.sudokus = resizeSudokus(target.sudokus, target.total, target.total + 1);
            (target.sudokus + target.total)->size = (source.sudokus + i)->size;
            (target.sudokus + target.total)->board = (source.sudokus + i)->board;
            target.total++;
        }
    }
    return target;
}


/**
 * Liberta a memória
 * @param l
 */
void free_list_sudoku(ListSudoku l) {
    for (int i = 0; i < l.total; i++) {
        for (int row = 0; row < (l.sudokus + i)->size; row++) {
            free(*((l.sudokus + i)->board + row));
        }
        free((l.sudokus + i)->board);
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
 * Obter o tempo atual do computador
 * @param time_usec
 * @return
 */
/*#ifdef WIN32
int gettimeuseconds(long long * time_usec) {
    union {
        long long ns100; //time since 1 Jan 1601 in 100ns units
        FILETIME ft;
    } now;

    GetSystemTimeAsFileTime( &(now.ft) ); // win32 function!
    *time_usec=(long long) (now.ns100 / 10LL);
    return 0;
}
#else*/
int gettimeuseconds(long long *time_usec) {
    struct timeval time;
    gettimeofday(&time, NULL);

    *time_usec = (long long) (time.tv_sec * 1000000 + time.tv_usec);
    return 0;
}

//#endif
