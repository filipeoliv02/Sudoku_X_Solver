#include "fileio.h"
#include "utils.h"

ListSudoku load_sudokus(char *file) {
    int size;
    ListSudoku s;
    s.sudokus = NULL;
    s.total = 0;

    FILE *fp = fopen(file, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d", &size) != EOF) {

            s.sudokus = resizeSudokus(s.sudokus, s.total, s.total + 1);
            s.sudokus[s.total].size = size;
            s.sudokus[s.total].board = createBoard(size);

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    fscanf(fp, "%d", &(s.sudokus[s.total].board[i][j]));
                }
            }
            s.total++;
        }
        fclose(fp);
    }
    return s;
}

void save_sudokus(ListSudoku list, char *file) {
    FILE *fp = fopen(file, "w");

    if (fp != NULL) {
        for (int i = 0; i < list.total; i++) {
            fprintf(fp, "%d\n", list.sudokus[i].size);
            for (int j = 0; j < list.sudokus[i].size; j++) {
                for (int k = 0; k < list.sudokus[i].size; k++) {
                    fprintf(fp, "%d ", list.sudokus[i].board[j][k]);
                }
                fprintf(fp, "\n");
            }
        }
        fclose(fp);
    }
}