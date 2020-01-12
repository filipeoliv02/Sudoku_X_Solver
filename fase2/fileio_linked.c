#include "fileio_linked.h"
#include "utils_linked.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief carrega os sudokus do ficheiro
 * @param file
 * @return
 */
SudokuQueue *loadSudokuQueueFromFile(char *file) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o ficheiro texto do sudoku em listas ligadas!\n");
        return NULL;
    }

    SudokuQueue *sudokuQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudokuQueueNode;
    Node *node, *nodeFirstCol;
    int size;

    while (fscanf(fp, "%d", &size) != EOF) {
        sudokuQueueNode = createEmptySudokuLinked(size);
        enqueueSudoku(sudokuQueue, sudokuQueueNode);

        node = sudokuQueueNode->first;
        nodeFirstCol = node;

        while (nodeFirstCol != NULL) {
            while (node != NULL) {
                fscanf(fp, "%d ", &node->num);
                node = node->e;
            }
            nodeFirstCol = nodeFirstCol->s;
            node = nodeFirstCol;
        }
    }

    fclose(fp);
    return sudokuQueue;
}

/**
 * @brief guarda os sudokus no ficheiro
 * @param sudokuLinked
 * @param file
 */
void saveSudokuQueueToFile(SudokuQueue *sudokuQueue, char *file) {
    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        printf("Erro ao guardar os tabuleiros no ficheiro!");
        return;
    }

    SudokuQueueNode *sudoku;
    Node *node, *nodeFirstCol;

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

    fclose(fp);
}


/**
 * @brief carrega os sudokus do ficheiro binário
 * @param file
 * @return
 */
SudokuQueue *loadSudokuQueueFromBinFile(char *file) {
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) {
        printf("Erro ao abrir o ficheiro binário do sudoku em listas ligadas!\n");
        return NULL;
    }

    SudokuQueue *sudokuQueue = (SudokuQueue *) calloc(1, sizeof(SudokuQueue));
    SudokuQueueNode *sudokuQueueNode;
    Node *node, *nodeFirstCol;
    int total, size;

    fread(&total, 1, 1, fp);
    for (int i = 0; i < total; i++) {
        fread(&size, 1, 1, fp);
        sudokuQueueNode = createEmptySudokuLinked(size);
        enqueueSudoku(sudokuQueue, sudokuQueueNode);

        node = sudokuQueueNode->first;
        nodeFirstCol = node;

        while (nodeFirstCol != NULL) {
            while (node != NULL) {
                fread(&node->num, 1, 1, fp);
                node = node->e;
            }
            nodeFirstCol = nodeFirstCol->s;
            node = nodeFirstCol;
        }
    }

    fclose(fp);
    return sudokuQueue;
}

/**
 * @brief guarda os sudokus em binário
 * @param sudokuLinked
 * @param file
 */
void saveSudokuQueueToBinFile(SudokuQueue *sudokuQueue, char *file) {
    FILE *fp = fopen(file, "wb");
    if (fp == NULL) {
        printf("Erro ao guardar o ficheiro binário do sudoku em listas ligadas!\n");
        return;
    }

    SudokuQueueNode *sudoku;
    Node *node, *nodeLine;

    fwrite(&sudokuQueue->total, 1, 1, fp);
    for (int i = sudokuQueue->total; i > 0; i--) {
        sudoku = dequeueSudoku(sudokuQueue);
        fwrite(&sudoku->size, 1, 1, fp);

        node = sudoku->first;
        nodeLine = node;

        for (int row = 0; row < sudoku->size; row++) {
            for (int col = 0; col < sudoku->size; col++) {
                fwrite(&node->num, 1, 1, fp);
                node = node->e;
            }

            nodeLine = nodeLine->s;
            node = nodeLine;
        }
        freeSudoku(sudoku);
    }

    fclose(fp);
}

