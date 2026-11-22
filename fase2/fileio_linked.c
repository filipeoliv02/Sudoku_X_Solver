#include "fileio_linked.h"
#include "utils_linked.h"
#include "../common/sudoku_common.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Carrega os sudokus de um ficheiro de texto
 * @details Os dados são validados (tamanho suportado, valores em [0, tamanho], tabuleiro completo). Perante dados
 * inválidos a leitura termina e mantêm-se os tabuleiros válidos lidos até aí.
 * @param file
 * @return Fila de sudokus (nunca NULL; vazia em caso de erro)
 */
SudokuQueue *loadSudokuQueueFromFile(const char *file) {
    SudokuQueue *sudokuQueue = createSudokuQueue();
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro texto do sudoku em listas ligadas: '%s'\n", file);
        return sudokuQueue;
    }

    SudokuQueueNode *sudokuQueueNode;
    Node *node, *nodeFirstCol;
    int size, scanned;

    while ((scanned = sudoku_read_int(fp, &size)) == 1) {
        if (!sudoku_is_valid_size(size)) {
            fprintf(stderr, "%s: tamanho de tabuleiro invalido (%d)\n", file, size);
            break;
        }
        sudokuQueueNode = createEmptySudokuLinked(size);

        int ok = 1;
        node = sudokuQueueNode->first;
        nodeFirstCol = node;
        while (nodeFirstCol != NULL && ok) {
            while (node != NULL && ok) {
                if (sudoku_read_int(fp, &node->num) != 1 || node->num < 0 || node->num > size) {
                    fprintf(stderr, "%s: valor invalido ou em falta (linha %d, coluna %d)\n", file,
                            node->row + 1, node->col + 1);
                    ok = 0;
                }
                node = node->e;
            }
            nodeFirstCol = nodeFirstCol->s;
            node = nodeFirstCol;
        }

        if (!ok) {
            freeSudoku(sudokuQueueNode);
            break;
        }
        enqueueSudoku(sudokuQueue, sudokuQueueNode);
    }
    if (scanned == 0) {
        fprintf(stderr, "%s: conteudo nao numerico no ficheiro\n", file);
    }

    fclose(fp);
    return sudokuQueue;
}

/**
 * @brief Guarda os sudokus num ficheiro de texto
 * @details A fila é esvaziada (os tabuleiros são libertados), mas a estrutura da fila continua a pertencer ao chamador.
 * @param sudokuQueue
 * @param file
 * @return 1 em caso de sucesso, 0 em caso de erro
 */
int saveSudokuQueueToFile(SudokuQueue *sudokuQueue, const char *file) {
    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao guardar os tabuleiros no ficheiro '%s'\n", file);
        return 0;
    }

    SudokuQueueNode *sudoku;
    Node *node, *nodeFirstCol;

    while (sudokuQueue->total > 0) {
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

    int ok = !ferror(fp);
    ok = (fclose(fp) == 0) && ok;
    if (!ok) {
        fprintf(stderr, "Erro ao escrever o ficheiro '%s'\n", file);
    }
    return ok;
}

/**
 * @brief Lê um byte sem sinal do ficheiro
 * @return O valor lido (0-255) ou -1 em caso de EOF/erro
 */
static int readByte(FILE *fp) {
    int c = fgetc(fp);
    return c == EOF ? -1 : c;
}

/**
 * @brief Carrega os sudokus de um ficheiro binário
 * @details Formato: 1 byte com o número de tabuleiros; para cada tabuleiro 1 byte com o tamanho seguido de
 * tamanho*tamanho bytes com os valores. Todos os valores são validados.
 * @param file
 * @return Fila de sudokus (nunca NULL; vazia em caso de erro)
 */
SudokuQueue *loadSudokuQueueFromBinFile(const char *file) {
    SudokuQueue *sudokuQueue = createSudokuQueue();
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro binario do sudoku em listas ligadas: '%s'\n", file);
        return sudokuQueue;
    }

    SudokuQueueNode *sudokuQueueNode;
    Node *node, *nodeFirstCol;

    int total = readByte(fp);
    if (total < 0) {
        fprintf(stderr, "%s: ficheiro binario vazio\n", file);
    }
    for (int i = 0; i < total; i++) {
        int size = readByte(fp);
        if (!sudoku_is_valid_size(size)) {
            fprintf(stderr, "%s: tamanho de tabuleiro invalido (%d) no tabuleiro %d\n", file, size, i + 1);
            break;
        }
        sudokuQueueNode = createEmptySudokuLinked(size);

        int ok = 1;
        node = sudokuQueueNode->first;
        nodeFirstCol = node;
        while (nodeFirstCol != NULL && ok) {
            while (node != NULL && ok) {
                node->num = readByte(fp);
                if (node->num < 0 || node->num > size) {
                    fprintf(stderr, "%s: valor invalido ou em falta no tabuleiro %d\n", file, i + 1);
                    ok = 0;
                }
                node = node->e;
            }
            nodeFirstCol = nodeFirstCol->s;
            node = nodeFirstCol;
        }

        if (!ok) {
            freeSudoku(sudokuQueueNode);
            break;
        }
        enqueueSudoku(sudokuQueue, sudokuQueueNode);
    }

    fclose(fp);
    return sudokuQueue;
}

/**
 * @brief Guarda os sudokus num ficheiro binário
 * @details A fila é esvaziada (os tabuleiros são libertados), mas a estrutura da fila continua a pertencer ao chamador.
 * @param sudokuQueue
 * @param file
 * @return 1 em caso de sucesso, 0 em caso de erro
 */
int saveSudokuQueueToBinFile(SudokuQueue *sudokuQueue, const char *file) {
    if (sudokuQueue->total > SUDOKU_BIN_MAX_VALUE) {
        fprintf(stderr, "O formato binario suporta no maximo %d tabuleiros\n", SUDOKU_BIN_MAX_VALUE);
        return 0;
    }
    FILE *fp = fopen(file, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao guardar o ficheiro binario do sudoku em listas ligadas: '%s'\n", file);
        return 0;
    }

    SudokuQueueNode *sudoku;
    Node *node, *nodeLine;

    int ok = fputc((unsigned char) sudokuQueue->total, fp) != EOF;
    while (sudokuQueue->total > 0) {
        sudoku = dequeueSudoku(sudokuQueue);
        ok = ok && fputc((unsigned char) sudoku->size, fp) != EOF;

        node = sudoku->first;
        nodeLine = node;

        for (int row = 0; row < sudoku->size; row++) {
            for (int col = 0; col < sudoku->size; col++) {
                ok = ok && fputc((unsigned char) node->num, fp) != EOF;
                node = node->e;
            }

            nodeLine = nodeLine->s;
            node = nodeLine;
        }
        freeSudoku(sudoku);
    }

    ok = (fclose(fp) == 0) && ok;
    if (!ok) {
        fprintf(stderr, "Erro ao escrever o ficheiro binario '%s'\n", file);
    }
    return ok;
}
