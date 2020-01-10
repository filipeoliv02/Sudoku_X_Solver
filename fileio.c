#include "fileio.h"
#include "utils.h"
#include "math.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Carregar Tabuleiros
 * @details Carregar tabuleiros para a memória
 * @param file
 * @return
 */
SudokuList load_sudokus(char *file) {
    int size;
    SudokuList s = {0, NULL, NULL};
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
void save_sudokus(SudokuList newlist, char *file) {
    SudokuList list = load_sudokus(file);
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
void save_binary(SudokuList solved, char *file) {
    FILE *fp = fopen(file, "wb");
    int aux;
    if (fp != NULL) {
        fwrite(&solved.total, 1, 1, fp);
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


SudokuLinkedNode *load_sudokus_link(char *file) {
    int size;
    SudokuLinkedNode *pqueue, *pqueue_pfirst = NULL, *pqueue_pprev = NULL;
    Node *node, *node_line = NULL, *node_prevline = NULL, *node_prev;

    FILE *fp = fopen(file, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d", &size) != EOF) {
            pqueue = (SudokuLinkedNode *) malloc(sizeof(SudokuLinkedNode));
            pqueue->size = size;
            pqueue->next = NULL;
            pqueue->first = NULL;

            if (pqueue_pfirst == NULL) {
                pqueue_pfirst = pqueue;
            }
            if (pqueue_pprev != NULL) {
                pqueue_pprev->next = pqueue;
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
                    node = (Node *) calloc(1, sizeof(Node));
                    fscanf(fp, "%d ", &node->num);
                    node->row = i;
                    node->col = j;
                    // Se não existir um primeiro nó então é este
                    if (pqueue->first == NULL) {
                        pqueue->first = node;
                    }

                    // Se não existir um nó anterior cria-se
                    if (node_prev == NULL) {
                        node_prev = node;
                    } else {
                        // Existe nó anterior logo liga-se (Este <--> Oeste)
                        node_prev->e = node;
                        node->w = node_prev;
                        node_prev = node_prev->e;


                    }

                    // Se existir nó da linha anterior liga-se (Norte <--> Sul)
                    if (node_prevline != NULL) {
                        node_prevline->s = node;
                        node->n = node_prevline;
                        node_prevline = node_prevline->e;
                    }

                    // Ligar se estiver na diagonal principal e não na primeira linha
                    if (i == j && i != 0) {
                        node->nw = node->w->n;
                        node->nw->se = node;
                    }

                    // Ligar se estiver na diagonal secundária e não na primeira linha
                    if (i == size - j - 1 && i != 0) {
                        node->ne = node->n->e;
                        node->ne->sw = node;
                    }
                    //Ligar Regiões
                    int root = sqrt(size);
                    int rcol, rrow;
                    rcol = j % root;
                    rrow = i % root;
                    Node *rnode = node;
                    if (!(rcol == 0 && rrow == 0)) {
                        if (rcol == 0) {
                            rnode = rnode->n;
                            while (rnode->col % root != (root - 1)) {
                                rnode = rnode->e;
                            }
                            node->bbox = rnode;
                            rnode->fbox = node;
                        } else {
                            node->bbox = node->w;
                            node->w->fbox = node;
                        }

                    }
                }

                //Se não existe linha associar
                if (node_line == NULL) {
                    node_line = pqueue->first;
                } else {
                    node_line = node_line->s;
                }
                node_prevline = node_line;

            }

        }
        fclose(fp);
    }
    return pqueue_pfirst;
}

void save_sudokus_linked(SudokuLinked sudokuLinked, char *file) {
    SudokuList list = load_sudokus(file);
    //list = merge_sudokus(list, newlist);

    /*FILE *fp = fopen(file, "w");
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
    }*/

}

void save_binary_linked(SudokuLinked sudokuLinked, char *file) {
    FILE *fp = fopen(file, "wb");
    if (fp == NULL) {
        printf("Erro ao guardar o ficheiro binário do sudoku em listas ligadas!\n");
        return;
    }

    fwrite(&sudokuLinked.total, 1, 1, fp);

    SudokuLinkedNode *sudokuLinkedNode = sudokuLinked.first;
    Node *node, *nodeLine;

    while (sudokuLinkedNode != NULL) {
        fwrite(&sudokuLinkedNode->size, 1, 1, fp);

        node = sudokuLinkedNode->first;
        nodeLine = node;
        while (nodeLine != NULL) {
            while (node != NULL) {
                fwrite(&node->num, 1, 1, fp);
                node = node->e;
            }
            nodeLine = nodeLine->s;
            node = nodeLine;
        }

        sudokuLinkedNode = sudokuLinkedNode->next;
    }
    fclose(fp);
}

