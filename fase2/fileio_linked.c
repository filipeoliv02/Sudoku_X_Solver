#include "fileio_linked.h"
#include "../fase1/utils.h"
#include "utils_linked.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


SudokuQueue *loadSudokuQueueFromFile(char *file) {
    int size;
    SudokuQueue *sudokuQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudokuQueueNode;
    Node *node, *node_line = NULL, *node_prevline = NULL, *node_prev;

    FILE *fp = fopen(file, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d", &size) != EOF) {
            sudokuQueueNode = (SudokuQueueNode *) calloc(1, sizeof(SudokuQueueNode));
            enqueueSudoku(sudokuQueue, sudokuQueueNode);
            sudokuQueueNode->size = size;

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
                    if (sudokuQueueNode->first == NULL) {
                        sudokuQueueNode->first = node;
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
                    node_line = sudokuQueueNode->first;
                } else {
                    node_line = node_line->s;
                }
                node_prevline = node_line;
            }
        }
        fclose(fp);
    }
    return sudokuQueue;
}

void saveSudokuQueueToFile(SudokuQueue *sudokuQueue, char *file) {
    SudokuQueueNode *sudoku;
    Node *node, *nodeFirstCol;
    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        printf("Erro ao guardar os tabuleiros no ficheiro!");
        return;
    }

    for (int i = sudokuQueue->total; i > 0; i--) {
        sudoku = dequeueSudoku(sudokuQueue);
        fprintf(fp, "%d\n", sudoku->size);

        node = sudoku->first;
        nodeFirstCol = sudoku->first;

        for (int j = 0; j < sudoku->size; j++) {
            for (int k = 0; k < sudoku->size; k++) {
                fprintf(fp, "%d ", node->num);
                node = node->e;
            }
            fprintf(fp, "\n");

            nodeFirstCol = nodeFirstCol->s;
            node = nodeFirstCol;
        }
        freeSudoku(sudoku);
    }

    free(sudokuQueue);
    fclose(fp);
}

SudokuQueue *loadSudokuQueueFromBinFile(char *file) {
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) {
        printf("Erro ao abrir o ficheiro binário do sudoku em listas ligadas!\n");
        return NULL;
    }

    SudokuQueue *sudokuQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudokuQueueNode;
    Node *node, *node_line = NULL, *node_prevline = NULL, *node_prev;
    int total, size;

    fread(&total, 1, 1, fp);

    for(int i = 0; i < total; i++) {
        sudokuQueueNode = (SudokuQueueNode *) calloc(1, sizeof(SudokuQueueNode));
        enqueueSudoku(sudokuQueue, sudokuQueueNode);

        fread(&size, 1, 1, fp);
        sudokuQueueNode->size = size;

        node_prevline = NULL;
        node_line = NULL;

        for (int row = 0; row < size; row++) {
            node_prev = NULL;
            for (int col = 0; col < size; col++) {
                // Criar nó e colocar valor do tabuleiro
                node = (Node *) calloc(1, sizeof(Node));
                fread(&node->num, 1, 1, fp);
                node->row = row;
                node->col = col;

                // Se não existir um primeiro nó então é este
                if (sudokuQueueNode->first == NULL) {
                    sudokuQueueNode->first = node;
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
                if (row == col && row != 0) {
                    node->nw = node->w->n;
                    node->nw->se = node;
                }

                // Ligar se estiver na diagonal secundária e não na primeira linha
                if (row == size - col - 1 && row != 0) {
                    node->ne = node->n->e;
                    node->ne->sw = node;
                }
                //Ligar Regiões
                int root = sqrt(size);
                int rcol, rrow;
                rcol = col % root;
                rrow = row % root;
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
                node_line = sudokuQueueNode->first;
            } else {
                node_line = node_line->s;
            }
            node_prevline = node_line;
        }
    }

    fclose(fp);
    return sudokuQueue;
}

void saveSudokuQueueToBinFile(SudokuQueue *sudokuQueue, char *file) {
    FILE *fp = fopen(file, "wb");
    if (fp == NULL) {
        printf("Erro ao guardar o ficheiro binário do sudoku em listas ligadas!\n");
        return;
    }

    fwrite(&sudokuQueue->total, 1, 1, fp);

    SudokuQueueNode *sudoku;
    Node *node, *nodeLine;

    for(int i = sudokuQueue->total; i > 0; i--) {
        sudoku = dequeueSudoku(sudokuQueue);
        fwrite(&sudoku->size, 1, 1, fp);

        node = sudoku->first;
        nodeLine = node;

        for(int row = 0; row < sudoku->size; row++) {
            for(int col = 0; col < sudoku->size; col++) {
                fwrite(&node->num, 1, 1, fp);
                node = node->e;
            }

            nodeLine = nodeLine->s;
            node = nodeLine;
        }
        freeSudoku(sudoku);
    }

    free(sudokuQueue);
    fclose(fp);
}

