/**
 * @file test_sudoku.c
 * @brief Automated tests for the Sudoku X solvers.
 *
 * Usage:
 *   test_sudoku solve FILE [BRUTEFORCE_MAX_SIZE]
 *       Loads every board in FILE and solves it with the array (fase1) and linked (fase2) optimized solvers, and with
 *       both brute force solvers for boards up to BRUTEFORCE_MAX_SIZE (default 9). Every reported solution is checked by
 *       an independent validator (rows, columns, boxes, both diagonals for boards up to 16x16, and the original givens),
 *       and all solvers must agree on whether each board is solvable.
 *   test_sudoku robustness
 *       Feeds malformed text/binary files to both loaders, checks file round trips and bounded random generation.
 */
#include "common/sudoku_common.h"
#include "fase1/algorithms.h"
#include "fase1/fileio.h"
#include "fase1/utils.h"
#include "fase2/algorithms_linked.h"
#include "fase2/fileio_linked.h"
#include "fase2/utils_linked.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define CHECK(cond, ...)                                                       \
    do {                                                                       \
        if (!(cond)) {                                                         \
            failures++;                                                        \
            fprintf(stderr, "FAIL %s:%d: %s -- ", __FILE__, __LINE__, #cond);  \
            fprintf(stderr, __VA_ARGS__);                                      \
            fputc('\n', stderr);                                               \
        }                                                                      \
    } while (0)

/* ------------------------------------------------------------------------- */
/* Independent validator (does not reuse any solver code)                     */
/* ------------------------------------------------------------------------- */

/**
 * Checks that @p cells (row-major, size*size) is a complete, valid board that keeps every non-zero value of @p givens.
 * Diagonals are enforced for boards up to SUDOKUX_MAX_DIAGONAL_SIZE, matching the project's rules.
 */
static int is_valid_solution(int size, const int *cells, const int *givens, char *why, size_t whyLen) {
    int box = sudoku_box_size(size);
    unsigned char *seen = calloc((size_t) size + 1, 1);
    if (seen == NULL || box == 0) {
        free(seen);
        snprintf(why, whyLen, "bad size %d", size);
        return 0;
    }
    int ok = 1;

    for (int i = 0; i < size * size && ok; i++) {
        if (cells[i] < 1 || cells[i] > size) {
            snprintf(why, whyLen, "cell %d has value %d", i, cells[i]);
            ok = 0;
        } else if (givens != NULL && givens[i] != 0 && givens[i] != cells[i]) {
            snprintf(why, whyLen, "given at (%d,%d) changed from %d to %d", i / size, i % size, givens[i], cells[i]);
            ok = 0;
        }
    }

    /* unit 0..size-1 rows, then columns, boxes, and the two diagonals */
    int units = 3 * size + (size <= SUDOKUX_MAX_DIAGONAL_SIZE ? 2 : 0);
    for (int u = 0; u < units && ok; u++) {
        memset(seen, 0, (size_t) size + 1);
        for (int k = 0; k < size && ok; k++) {
            int r, c;
            if (u < size) {
                r = u, c = k;
            } else if (u < 2 * size) {
                r = k, c = u - size;
            } else if (u < 3 * size) {
                int b = u - 2 * size;
                r = (b / box) * box + k / box;
                c = (b % box) * box + k % box;
            } else if (u == 3 * size) {
                r = k, c = k;
            } else {
                r = k, c = size - 1 - k;
            }
            int v = cells[r * size + c];
            if (seen[v]) {
                snprintf(why, whyLen, "duplicate %d in unit %d (cell %d,%d)", v, u, r, c);
                ok = 0;
            }
            seen[v] = 1;
        }
    }
    free(seen);
    return ok;
}

static int *array_cells(Sudoku s) {
    int *cells = calloc((size_t) s.size * (size_t) s.size, sizeof(int));
    if (cells == NULL) {
        exit(EXIT_FAILURE);
    }
    for (int r = 0; r < s.size; r++) {
        for (int c = 0; c < s.size; c++) {
            cells[r * s.size + c] = s.board[r][c];
        }
    }
    return cells;
}

static int *linked_cells(SudokuQueueNode *s) {
    int *cells = calloc((size_t) s->size * (size_t) s->size, sizeof(int));
    if (cells == NULL) {
        exit(EXIT_FAILURE);
    }
    int i = 0;
    for (Node *line = s->first; line != NULL; line = line->s) {
        for (Node *n = line; n != NULL; n = n->e) {
            if (i < s->size * s->size) {
                cells[i] = n->num;
            }
            i++;
        }
    }
    if (i != s->size * s->size) {
        fprintf(stderr, "linked board has %d cells, expected %d\n", i, s->size * s->size);
        exit(EXIT_FAILURE);
    }
    return cells;
}

static int validate_array_solutions(const char *label, int idx, SudokuList solved, const int *givens) {
    char why[160];
    for (int k = 0; k < solved.total; k++) {
        int *cells = array_cells(solved.sudokus[k]);
        CHECK(is_valid_solution(solved.sudokus[k].size, cells, givens, why, sizeof why),
              "%s board %d solution %d invalid: %s", label, idx, k, why);
        free(cells);
    }
    return solved.total;
}

static int validate_linked_solutions(const char *label, int idx, SudokuQueue *solved, const int *givens) {
    char why[160];
    int k = 0;
    for (SudokuQueueNode *s = solved->first; s != NULL; s = s->next, k++) {
        int *cells = linked_cells(s);
        CHECK(is_valid_solution(s->size, cells, givens, why, sizeof why),
              "%s board %d solution %d invalid: %s", label, idx, k, why);
        free(cells);
    }
    return solved->total;
}

/* ------------------------------------------------------------------------- */
/* Solve test                                                                 */
/* ------------------------------------------------------------------------- */

static int test_solve_file(const char *file, int bruteforceMaxSize) {
    SudokuList boards = load_sudokus(file);
    SudokuQueue *linkedBoards = loadSudokuQueueFromFile(file);
    CHECK(boards.total > 0, "no boards loaded from %s", file);
    CHECK(boards.total == linkedBoards->total, "fase1 loaded %d boards, fase2 loaded %d", boards.total,
          linkedBoards->total);

    int solvedBoards = 0;
    for (int i = 0; i < boards.total && linkedBoards->first != NULL; i++) {
        Sudoku board = boards.sudokus[i];
        SudokuQueueNode *linkedBoard = dequeueSudoku(linkedBoards);
        int *givens = array_cells(board);
        int consistent = isConsistent(board);
        CHECK(consistent == isConsistentLinked(linkedBoard), "board %d: consistency checks disagree", i);
        fprintf(stderr, "[%s] board %d (%dx%d)%s\n", file, i, board.size, board.size,
                consistent ? "" : " inconsistent, skipped");

        if (consistent) {
            long long cost = 0;
            SudokuList solved = {0, NULL, NULL};
            solveSudokuOptimized(board, &solved, &cost);
            int arrayOptimized = validate_array_solutions("optimized", i, solved, givens);
            free_list_sudoku(solved);

            SudokuQueue *linkedSolved = createSudokuQueue();
            solveLinkedSudokuOptimized(linkedSolved, linkedBoard);
            int linkedOptimized = validate_linked_solutions("linked-optimized", i, linkedSolved, givens);
            freeSudokuQueue(linkedSolved);

            CHECK((arrayOptimized > 0) == (linkedOptimized > 0),
                  "board %d: optimized found %d solution(s), linked-optimized found %d", i, arrayOptimized,
                  linkedOptimized);

            if (board.size <= bruteforceMaxSize) {
                SudokuList bf = {0, NULL, NULL};
                cost = 0;
                solveSudokuBruteForce(&bf, board, 0, 0, &cost);
                int arrayBrute = validate_array_solutions("bruteforce", i, bf, givens);
                free_list_sudoku(bf);

                SudokuQueue *linkedBf = createSudokuQueue();
                solveLinkedSudokuBruteForce(linkedBf, linkedBoard, linkedBoard->first);
                int linkedBrute = validate_linked_solutions("linked-bruteforce", i, linkedBf, givens);
                freeSudokuQueue(linkedBf);

                /* Brute force enumerates every solution, so both implementations must find the same number */
                CHECK(arrayBrute == linkedBrute, "board %d: bruteforce found %d, linked-bruteforce found %d", i,
                      arrayBrute, linkedBrute);
                CHECK((arrayBrute > 0) == (arrayOptimized > 0),
                      "board %d: bruteforce found %d solution(s), optimized found %d", i, arrayBrute,
                      arrayOptimized);
            }
            if (arrayOptimized > 0) {
                solvedBoards++;
            }

            /* The solvers must leave the input board untouched */
            int *after = array_cells(board);
            CHECK(memcmp(after, givens, sizeof(int) * (size_t) board.size * (size_t) board.size) == 0,
                  "board %d was modified by a solver", i);
            free(after);
        }
        free(givens);
        freeSudoku(linkedBoard);
    }
    CHECK(solvedBoards > 0, "no board in %s was solved", file);
    fprintf(stderr, "[%s] %d/%d boards solved and validated\n", file, solvedBoards, boards.total);

    free_list_sudoku(boards);
    freeSudokuQueue(linkedBoards);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* ------------------------------------------------------------------------- */
/* Robustness test                                                            */
/* ------------------------------------------------------------------------- */

static void write_file(const char *path, const void *data, size_t len) {
    FILE *fp = fopen(path, "wb");
    if (fp == NULL || fwrite(data, 1, len, fp) != len || fclose(fp) != 0) {
        fprintf(stderr, "cannot write %s\n", path);
        exit(EXIT_FAILURE);
    }
}

static void expect_text_boards(const char *name, const char *content, int expected) {
    const char *path = "robustness_input.txt";
    write_file(path, content, strlen(content));

    SudokuList list = load_sudokus(path);
    CHECK(list.total == expected, "[%s] fase1 loaded %d boards, expected %d", name, list.total, expected);
    free_list_sudoku(list);

    SudokuQueue *queue = loadSudokuQueueFromFile(path);
    CHECK(queue->total == expected, "[%s] fase2 loaded %d boards, expected %d", name, queue->total, expected);
    freeSudokuQueue(queue);
    remove(path);
}

static void expect_bin_boards(const char *name, const unsigned char *content, size_t len, int expected) {
    const char *path = "robustness_input.bin";
    write_file(path, content, len);
    SudokuQueue *queue = loadSudokuQueueFromBinFile(path);
    CHECK(queue->total == expected, "[%s] binary loader loaded %d boards, expected %d", name, queue->total,
          expected);
    freeSudokuQueue(queue);
    remove(path);
}

static const char VALID_4X4[] = "4\n1 0 0 0\n0 0 0 0\n0 0 0 0\n0 0 0 0\n";

static int test_robustness(void) {
    /* Text loaders: malformed or hostile input must be rejected without crashing */
    expect_text_boards("empty file", "", 0);
    expect_text_boards("size zero", "0\n", 0);
    expect_text_boards("negative size", "-9\n", 0);
    expect_text_boards("huge size", "2147483647\n1\n", 0);
    expect_text_boards("size overflowing int", "99999999999999999999\n1\n", 0);
    expect_text_boards("very long token", "0000000000000000000000000000000000000000000000004\n", 0);
    expect_text_boards("value overflowing int",
                       "4\n1 0 0 0\n0 0 0 0\n0 0 0 0\n0 0 0 4294967297\n", 0);
    expect_text_boards("non-square size", "5\n0 0 0 0 0\n", 0);
    expect_text_boards("size above maximum", "81\n0\n", 0);
    expect_text_boards("value above size", "4\n9 0 0 0\n0 0 0 0\n0 0 0 0\n0 0 0 0\n", 0);
    expect_text_boards("negative value", "4\n-1 0 0 0\n0 0 0 0\n0 0 0 0\n0 0 0 0\n", 0);
    expect_text_boards("truncated board", "4\n1 2 3\n", 0);
    expect_text_boards("text instead of numbers", "hello world\n", 0);
    expect_text_boards("valid board", VALID_4X4, 1);
    {
        char buf[256];
        snprintf(buf, sizeof buf, "%s%sgarbage\n", VALID_4X4, VALID_4X4);
        expect_text_boards("valid boards then garbage", buf, 2);
        snprintf(buf, sizeof buf, "%s4\n1 2\n", VALID_4X4);
        expect_text_boards("valid board then truncated board", buf, 1);
    }
    {
        SudokuList missing = load_sudokus("this-file-does-not-exist.txt");
        CHECK(missing.total == 0, "missing file returned %d boards", missing.total);
        free_list_sudoku(missing);
        SudokuQueue *q = loadSudokuQueueFromFile("this-file-does-not-exist.txt");
        CHECK(q != NULL && q->total == 0, "missing file returned boards (linked)");
        freeSudokuQueue(q);
        q = loadSudokuQueueFromBinFile("this-file-does-not-exist.bin");
        CHECK(q != NULL && q->total == 0, "missing binary file returned boards");
        freeSudokuQueue(q);
    }

    /* Binary loader */
    {
        unsigned char good[2 + 16] = {1, 4, 1, 0};
        expect_bin_boards("valid binary", good, sizeof good, 1);
        expect_bin_boards("empty binary", good, 0, 0);
        expect_bin_boards("truncated binary", good, 10, 0);

        unsigned char claimsMore[2 + 16];
        memcpy(claimsMore, good, sizeof good);
        claimsMore[0] = 200; /* header claims 200 boards, only one present */
        expect_bin_boards("binary count larger than content", claimsMore, sizeof claimsMore, 1);

        unsigned char badSize[2 + 16];
        memcpy(badSize, good, sizeof good);
        badSize[1] = 5;
        expect_bin_boards("binary non-square size", badSize, sizeof badSize, 0);
        badSize[1] = 0;
        expect_bin_boards("binary size zero", badSize, sizeof badSize, 0);

        unsigned char badValue[2 + 16];
        memcpy(badValue, good, sizeof good);
        badValue[5] = 200;
        expect_bin_boards("binary value above size", badValue, sizeof badValue, 0);
    }

    /* Round trips: fase1 text writer -> both readers, fase1 binary writer -> fase2 binary reader */
    {
        const char *src = "roundtrip_src.txt", *txt = "roundtrip_out.txt", *bin = "roundtrip_out.bin";
        char buf[512];
        snprintf(buf, sizeof buf, "%s9\n"
                                  "1 0 0 0 0 0 0 0 9\n0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0\n"
                                  "0 0 0 0 0 0 0 0 0\n0 0 0 0 5 0 0 0 0\n0 0 0 0 0 0 0 0 0\n"
                                  "0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0\n9 0 0 0 0 0 0 0 1\n", VALID_4X4);
        write_file(src, buf, strlen(buf));
        remove(txt);

        SudokuList original = load_sudokus(src);
        CHECK(original.total == 2, "round trip source loaded %d boards", original.total);
        CHECK(save_sudokus(original, txt), "save_sudokus failed");
        CHECK(save_sudokus(original, txt), "save_sudokus (second time) failed");
        CHECK(save_binary(original, bin), "save_binary failed");

        SudokuList reloaded = load_sudokus(txt);
        CHECK(reloaded.total == original.total, "text round trip: %d boards, expected %d (no duplicates)",
              reloaded.total, original.total);
        for (int i = 0; i < reloaded.total && i < original.total; i++) {
            CHECK(isEqual(reloaded.sudokus[i], original.sudokus[i]), "text round trip board %d differs", i);
        }

        SudokuQueue *fromBin = loadSudokuQueueFromBinFile(bin);
        CHECK(fromBin->total == original.total, "binary round trip: %d boards", fromBin->total);
        int i = 0;
        for (SudokuQueueNode *s = fromBin->first; s != NULL && i < original.total; s = s->next, i++) {
            int *a = array_cells(original.sudokus[i]), *b = linked_cells(s);
            CHECK(s->size == original.sudokus[i].size &&
                  memcmp(a, b, sizeof(int) * (size_t) s->size * (size_t) s->size) == 0,
                  "binary round trip board %d differs", i);
            free(a);
            free(b);
        }

        /* fase2 binary writer -> fase2 binary reader */
        CHECK(saveSudokuQueueToBinFile(fromBin, bin), "saveSudokuQueueToBinFile failed");
        CHECK(fromBin->total == 0, "saveSudokuQueueToBinFile should drain the queue");
        SudokuQueue *again = loadSudokuQueueFromBinFile(bin);
        CHECK(again->total == original.total, "linked binary round trip: %d boards", again->total);

        freeSudokuQueue(again);
        freeSudokuQueue(fromBin);
        free_list_sudoku(reloaded);
        free_list_sudoku(original);
        remove(src);
        remove(txt);
        remove(bin);
    }

    /* Random generation: impossible requests must terminate, generated boards must be consistent */
    {
        Sudoku g = gen_sudoku(9, 1000);
        CHECK(isConsistent(g), "generated 9x9 board is inconsistent");
        freeBoard(g.board, g.size);

        g = gen_sudoku(4, 6);
        CHECK(isConsistent(g), "generated 4x4 board is inconsistent");
        int filled = 0;
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                filled += g.board[r][c] != 0;
            }
        }
        CHECK(filled == 6, "generated 4x4 board has %d filled cells, expected 6", filled);
        freeBoard(g.board, g.size);

        SudokuQueueNode *l = generateRandomSudokuLinked(4, 100);
        CHECK(l != NULL && isConsistentLinked(l), "generated linked board is inconsistent");
        freeSudoku(l);
        CHECK(generateRandomSudokuLinked(7, 3) == NULL, "invalid size should not generate a board");
    }

    /* Inconsistent givens must not crash the linked optimized solver */
    {
        const char *path = "inconsistent.txt";
        const char *content = "4\n1 1 0 0\n0 0 0 0\n0 0 0 0\n0 0 0 0\n";
        write_file(path, content, strlen(content));
        SudokuQueue *q = loadSudokuQueueFromFile(path);
        CHECK(q->total == 1, "inconsistent board not loaded");
        if (q->first != NULL) {
            CHECK(!isConsistentLinked(q->first), "inconsistent board reported as consistent");
            SudokuQueue *solved = createSudokuQueue();
            solveLinkedSudokuOptimized(solved, q->first);
            CHECK(solved->total == 0, "inconsistent board produced %d solutions", solved->total);
            freeSudokuQueue(solved);
        }
        freeSudokuQueue(q);
        remove(path);
    }

    /* Size helpers */
    CHECK(sudoku_is_valid_size(4) && sudoku_is_valid_size(9) && sudoku_is_valid_size(36) && sudoku_is_valid_size(64),
          "valid sizes rejected");
    CHECK(!sudoku_is_valid_size(0) && !sudoku_is_valid_size(1) && !sudoku_is_valid_size(8) &&
          !sudoku_is_valid_size(81) && !sudoku_is_valid_size(-4), "invalid sizes accepted");

    fprintf(stderr, "robustness: %s\n", failures == 0 ? "all checks passed" : "FAILURES");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

int main(int argc, char **argv) {
    if (argc >= 3 && strcmp(argv[1], "solve") == 0) {
        int bruteforceMaxSize = argc >= 4 ? atoi(argv[3]) : 9;
        return test_solve_file(argv[2], bruteforceMaxSize);
    }
    if (argc == 2 && strcmp(argv[1], "robustness") == 0) {
        return test_robustness();
    }
    fprintf(stderr, "usage: %s solve FILE [BRUTEFORCE_MAX_SIZE] | robustness\n", argv[0]);
    return 2;
}
