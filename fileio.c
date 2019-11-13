#include "fileio.h"

void load_boards(Boards *b) {
    char line[2000] = "";
    b->num = 0;
    b->sizes = NULL;
    b->board = NULL;

    FILE *fp = fopen("board.txt", "r");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strcmp(line, "\n") != 0) {
                b->sizes = resizeArray(b->sizes, b->num, b->num + 1);
                b->board = resizeBoards(b->board, b->num, b->num + 1);
                //sscanf(line,"%d", &b->sizes[b->num]);
                b->sizes[b->num] = line[0] - '0';
                b->board[b->num] = createBoard(b->board[b->num], b->sizes[b->num]);
                for (int i = 0; i < b->sizes[b->num]; i++) {
                    for (int j = 0; j < b->sizes[b->num]; j++) {
                        int *boardLine = *(b->board[b->num] + i);
                        *(boardLine + j) = line[(i * b->sizes[b->num] + j) * 2 + 2] - '0';
                    }
                }
                b->num++;
            }
        }
        fclose(fp);
    }
}