#include "fileio.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Carregar Tabuleiros
 * @details Carregar tabuleiros para a memória
 * @param file
 * @return
 */
ListSudoku load_sudokus(char *file) {
    int size;
    ListSudoku s = {0, NULL, NULL};
    FILE *fp = fopen(file, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d", &size) != EOF) {

            s.sudokus = resizeSudokus(s.sudokus, s.total, s.total + 1);
            s.orderedList = resizeList(s.orderedList, s.total, s.total + 1);
            (s.sudokus + s.total)->size = size;
            *(s.orderedList + s.total) = 0;
            (s.sudokus + s.total)->board = createBoard(size);

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    fscanf(fp, "%d", (*((s.sudokus + s.total)->board + i) + j));
                }
            }
            s.total++;
        }
        fclose(fp);
        computeOrderBySize(&s);

    }
    return s;
}

/**
 * @brief Guardar Tabuleiros
 * @details Guardar tabuleiros Resolvidos no ficheiro passado
 * @param newlist
 * @param file
 */
void save_sudokus(ListSudoku newlist, char *file) {
    ListSudoku list = load_sudokus(file);
    list = merge_sudokus(list, newlist);

    FILE *fp = fopen(file, "w");
    if (fp != NULL) {
        for (int i = 0; i < list.total; i++) {
            fprintf(fp, "%d\n", (list.sudokus + i)->size);
            for (int j = 0; j < (list.sudokus + i)->size; j++) {
                for (int k = 0; k < (list.sudokus + i)->size; k++) {
                    fprintf(fp, "%d ", *(*((list.sudokus + i)->board + j) + k));
                }
                fprintf(fp, "\n");
            }
        }
        fclose(fp);
    }

}

/**
 * @brief Guardar Tabuleiros
 * @details Guardar tabuleiros Resolvidos no ficheiro binário
 * @param newlist
 * @param file
 */
void save2binary(ListSudoku solved, char *file) {
    FILE *fp = fopen(file, "wb");
    int aux;
    if (fp != NULL) {
        for (int i = 0; i < solved.total; i++) {
            aux = (solved.sudokus + i)->size;
            fwrite(&aux, 1, 1, fp);
            for (int j = 0; j < (solved.sudokus + i)->size; j++) {
                for (int k = 0; k < (solved.sudokus + i)->size; k++) {
                    aux = *(*((solved.sudokus + i)->board + j) + k);
                    fwrite(&aux, 1, 1, fp);
                }
            }
        }
        fclose(fp);
    }
}


SUDOKU_QUEUE *load_sudokus_link(char *file) {
    int size;
    SUDOKU_QUEUE *pqueue, *pqueue_pfirst = NULL, *pqueue_pprev = NULL;
    NODE *node, *node_line = NULL, *node_prevline = NULL, *node_prev;

    FILE *fp = fopen(file, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d", &size) != EOF) {
            pqueue = (SUDOKU_QUEUE *) malloc(sizeof(SUDOKU_QUEUE));
            pqueue->size = size;
            pqueue->pnext = NULL;
            pqueue->pfirst = NULL;

            if (pqueue_pfirst == NULL) {
                pqueue_pfirst = pqueue;
            }
            if (pqueue_pprev != NULL) {
                pqueue_pprev->pnext = pqueue;
            }
            pqueue_pprev = pqueue;

            node_prev = NULL;
            node = NULL;
            node_prevline = NULL;
            node_line = NULL;

            for (int i = 0; i < size; i++) {
                node_prev = NULL;
                for (int j = 0; j < size; j++) {
                    // Criar nó e colocar valor do tabuleiro
                    node = (NODE *) malloc(sizeof(NODE));
                    fscanf(fp, "%d", &node->info);

                    // Se não existir um primeiro nó então é este
                    if(pqueue->pfirst == NULL) {
                        pqueue->pfirst = node;
                    }

                    // Se não existir um nó anterior cria-se
                    if(node_prev == NULL) {
                        node_prev = node;
                    }
                    else {
                        // Existe nó anterior logo liga-se (Este <--> Oeste)
                        node_prev->pe = node;
                        node->po = node_prev;
                        node_prev = node_prev->pe;
                    }

                    // Se existir nó da lina anterior liga-se (Norte <--> Sul)
                    if(node_prevline != NULL) {
                        node_prevline->ps = node;
                        node->pn = node_prevline;
                        node_prevline = node_prevline->pe;
                    }

                    // Ligar se estiver na diagonal principal e não na primeira linha
                    if(i == j && i != 0) {
                        node->pno = node->po->pn;
                        node->pno->pse = node;
                    }

                    // Ligar se estiver na diagonal secundária e não na primeira linha
                    if(i == size - j - 1 && i != 0) {
                        node->pne = node->pn->pe;
                        node->pne->pso = node;
                    }
                }

                // Se não existe linha associar
                if(node_line == NULL) {
                    node_line = pqueue->pfirst;
                }
                else {
                    node_line = node_line->ps;
                }
                node_prevline = node_line;

            }

        }
        fclose(fp);
    }
    return pqueue_pfirst;
}

