#include "fileio.h"
#include "utils.h"

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


