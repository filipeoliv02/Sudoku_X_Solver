#include "fileio.h"

int **load_boards(int **boards) {
    char line[200] = "";
    int size;
    boards = (int **) malloc(9 * sizeof(int *));
    for (int i = 0; i < 9; i++) {
        *(boards + i) = (int *) malloc(9 * sizeof(int));

    }
    FILE *fp = fopen("board.txt", "r");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strcmp(line, "\n") != 0) {
                size = line[0] - '0';
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        int *boardLine = *(boards + i);
                        *(boardLine + j) = line[(i * size + j) * 2 + 2] - '0';

                    }
                }


            }
        }

        fclose(fp);
    }
    return boards;
}