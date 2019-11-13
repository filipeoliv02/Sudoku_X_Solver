#include "fileio.h"
#include "utils.h"
void load_boards(Boards *b) {
    char line[200000] = "";
    b->num = 0;
    b->sizes = NULL;
    b->board = NULL;
    int offset = 0;


    FILE *fp = fopen("board.txt", "r");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strcmp(line, "\n") != 0) {
                b->sizes = resizeArray(b->sizes, b->num, b->num + 1);
                b->board = resizeBoards(b->board, b->num, b->num + 1);
                sscanf(line, "%d", &b->sizes[b->num]);

                b->board[b->num] = createBoard(b->board[b->num], b->sizes[b->num]);

                offset = b->sizes[b->num] > 9 ? 3 : 2;
                for (int i = 0; i < b->sizes[b->num]; i++) {

                    for (int j = 0; j < b->sizes[b->num]; j++) {
                        sscanf(&line[(i * b->sizes[b->num] + j) * 2 + offset], "%d", &b->board[b->num][i][j]);
                        if (b->board[b->num][i][j] > 9) {
                            offset++;
                        }

                    }
                }
                b->num++;
            }
        }
        fclose(fp);
    }
}

