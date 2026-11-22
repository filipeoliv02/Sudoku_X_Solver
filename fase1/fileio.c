#include "fileio.h"
#include "utils.h"
#include "../common/sudoku_common.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Carregar Tabuleiros
 * @details Carregar tabuleiros para a memória. O formato é, para cada tabuleiro, o tamanho N seguido de N*N inteiros
 * (0 = célula vazia). Os dados do ficheiro são validados: tamanhos não suportados, valores fora de [0, N] ou
 * tabuleiros incompletos terminam a leitura (os tabuleiros válidos lidos até aí são mantidos).
 * @param file
 * @return
 */
SudokuList load_sudokus(const char *file) {
    int size;
    SudokuList s = {0, NULL, NULL};
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro '%s'\n", file);
        return s;
    }

    int scanned;
    while ((scanned = sudoku_read_int(fp, &size)) == 1) {
        if (!sudoku_is_valid_size(size)) {
            fprintf(stderr, "%s: tamanho de tabuleiro invalido (%d) no tabuleiro %d\n", file, size, s.total + 1);
            break;
        }

        int **board = createBoard(size);
        int ok = 1;
        for (int i = 0; i < size && ok; i++) {
            for (int j = 0; j < size && ok; j++) {
                int *cell = *(board + i) + j;
                if (sudoku_read_int(fp, cell) != 1 || *cell < 0 || *cell > size) {
                    fprintf(stderr, "%s: valor invalido ou em falta no tabuleiro %d (linha %d, coluna %d)\n",
                            file, s.total + 1, i + 1, j + 1);
                    ok = 0;
                }
            }
        }
        if (!ok) {
            freeBoard(board, size);
            break;
        }

        s.sudokus = resizeSudokus(s.sudokus, s.total, s.total + 1);
        s.orderedList = resizeList(s.orderedList, s.total, s.total + 1);
        (s.sudokus + s.total)->size = size;
        (s.sudokus + s.total)->board = board;
        *(s.orderedList + s.total) = s.total;
        s.total++;
    }
    if (scanned == 0) {
        fprintf(stderr, "%s: conteudo nao numerico apos o tabuleiro %d\n", file, s.total);
    }
    fclose(fp);
    computeOrderBySize(&s);
    return s;
}

/**
 * @brief Guardar Tabuleiros
 * @details Guardar tabuleiros Resolvidos no ficheiro passado (junta-os aos que já existiam no ficheiro, sem repetidos)
 * @param newlist
 * @param file
 * @return 1 em caso de sucesso, 0 em caso de erro
 */
int save_sudokus(SudokuList newlist, const char *file) {
    SudokuList list = {0, NULL, NULL};
    FILE *existing = fopen(file, "r");
    if (existing != NULL) {
        fclose(existing);
        list = load_sudokus(file);
    }
    list = merge_sudokus(list, newlist);

    int ok = 0;
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
        ok = !ferror(fp);
        ok = (fclose(fp) == 0) && ok;
    }
    if (!ok) {
        fprintf(stderr, "Erro ao guardar os tabuleiros em '%s'\n", file);
    }
    free_list_sudoku(list);
    return ok;
}

/**
 * @brief Guardar Tabuleiros
 * @details Guardar tabuleiros Resolvidos no ficheiro binário. Formato: 1 byte com o número de tabuleiros e, para cada
 * tabuleiro, 1 byte com o tamanho seguido de tamanho*tamanho bytes com os valores (compatível com a fase 2).
 * @param solved
 * @param file
 * @return 1 em caso de sucesso, 0 em caso de erro
 */
int save_binary(SudokuList solved, const char *file) {
    if (solved.total > SUDOKU_BIN_MAX_VALUE) {
        fprintf(stderr, "O formato binario suporta no maximo %d tabuleiros\n", SUDOKU_BIN_MAX_VALUE);
        return 0;
    }
    FILE *fp = fopen(file, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro binario '%s'\n", file);
        return 0;
    }

    int ok = fputc((unsigned char) solved.total, fp) != EOF;
    for (int i = 0; i < solved.total && ok; i++) {
        int size = (solved.sudokus + i)->size;
        ok = fputc((unsigned char) size, fp) != EOF;
        for (int j = 0; j < size && ok; j++) {
            for (int k = 0; k < size && ok; k++) {
                ok = fputc((unsigned char) *(*((solved.sudokus + i)->board + j) + k), fp) != EOF;
            }
        }
    }
    ok = (fclose(fp) == 0) && ok;
    if (!ok) {
        fprintf(stderr, "Erro ao escrever o ficheiro binario '%s'\n", file);
    }
    return ok;
}
