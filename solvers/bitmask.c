#include "bitmask.h"

#include <stdlib.h>
#include <string.h>

#define NO_RNG

typedef struct {
    int size;
    int box;
    int n;
    int *board;
    uint64_t *rowUsed;
    uint64_t *colUsed;
    uint64_t *boxUsed;
    uint64_t *diagUsed;
    const BitmaskOptions *options;
    int *solution;
    int found;
    SolverStats *stats;
} BitmaskSearch;

static int inconsistent_givens(BitmaskSearch *b, const int *givens) {
    uint64_t all = bits_low_mask(b->size);
    for (int r = 0; r < b->size; r++) {
        for (int c = 0; c < b->size; c++) {
            int v = givens[r * b->size + c];
            if (v < 0 || v > b->size) return 1;
            if (v == 0) continue;
            uint64_t bit = UINT64_C(1) << (v - 1);
            int bi = (r / b->box) * b->box + c / b->box;
            if (((b->rowUsed[r] | b->colUsed[c] | b->boxUsed[bi]) & bit) != 0) return 1;
            if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && r == c &&
                (b->diagUsed[0] & bit) != 0) return 1;
            if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && r + c == b->size - 1 &&
                (b->diagUsed[1] & bit) != 0) return 1;
            b->rowUsed[r] |= bit;
            b->colUsed[c] |= bit;
            b->boxUsed[bi] |= bit;
            if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && r == c) b->diagUsed[0] |= bit;
            if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && r + c == b->size - 1) b->diagUsed[1] |= bit;
            b->board[r * b->size + c] = v;
        }
    }
    (void) all;
    return 0;
}

static uint64_t candidates(BitmaskSearch *b, int r, int c) {
    int bi = (r / b->box) * b->box + c / b->box;
    uint64_t used = b->rowUsed[r] | b->colUsed[c] | b->boxUsed[bi];
    if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && r == c) used |= b->diagUsed[0];
    if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && r + c == b->size - 1) used |= b->diagUsed[1];
    return bits_low_mask(b->size) & ~used;
}

static void search(BitmaskSearch *b) {
    if (b->options && b->options->maxSolutions > 0 && b->found >= b->options->maxSolutions) return;
    if (b->options && b->options->maxNodes > 0 && b->stats &&
        b->stats->nodes >= b->options->maxNodes) {
        b->stats->aborted = 1;
        return;
    }

    int mr = -1, mc = -1;
    uint64_t mm = 0;
    int empty = 0;
    for (int r = 0; r < b->size; r++) {
        for (int c = 0; c < b->size; c++) {
            int i = r * b->size + c;
            if (b->board[i] != 0) continue;
            empty = 1;
            uint64_t mask = candidates(b, r, c);
            if (mask == 0) {
                if (b->stats) b->stats->backtracks++;
                return;
            }
            int count = bits_popcount(mask);
            if (mr < 0 || count < bits_popcount(mm)) {
                mr = r; mc = c; mm = mask;
                if (count == 1) break;
            }
        }
        if (mr >= 0 && bits_popcount(mm) == 1) break;
    }

    if (!empty) {
        b->found++;
        if (b->solution) memcpy(b->solution, b->board, (size_t)b->n * sizeof(int));
        return;
    }

    int bi = (mr / b->box) * b->box + mc / b->box;
    while (mm) {
        if (b->options && b->options->maxNodes > 0 && b->stats &&
            b->stats->nodes >= b->options->maxNodes) {
            b->stats->aborted = 1;
            break;
        }
        int bitIndex = bits_lowest(mm);
        mm &= mm - 1;
        uint64_t bit = UINT64_C(1) << bitIndex;
        int v = bitIndex + 1;
        int i = mr * b->size + mc;
        b->board[i] = v;
        b->rowUsed[mr] |= bit;
        b->colUsed[mc] |= bit;
        b->boxUsed[bi] |= bit;
        if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && mr == mc) b->diagUsed[0] |= bit;
        if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && mr + mc == b->size - 1) b->diagUsed[1] |= bit;
        if (b->stats) b->stats->nodes++;

        search(b);

        b->board[i] = 0;
        b->rowUsed[mr] &= ~bit;
        b->colUsed[mc] &= ~bit;
        b->boxUsed[bi] &= ~bit;
        if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && mr == mc) b->diagUsed[0] &= ~bit;
        if (b->size <= SUDOKUX_MAX_DIAGONAL_SIZE && mr + mc == b->size - 1) b->diagUsed[1] &= ~bit;
        if (b->options && b->options->maxSolutions > 0 && b->found >= b->options->maxSolutions) break;
    }
}

int bitmask_solve(int size, const int *givens, int *solution,
                  const BitmaskOptions *options, SolverStats *stats) {
    if (stats) memset(stats, 0, sizeof(*stats));
    if (!sudoku_is_valid_size(size) || size > 64 || givens == NULL) {
        return 0;
    }
    if (options && options->maxSolutions < 0) return 0;

    int n = size * size;
    BitmaskSearch b;
    memset(&b, 0, sizeof(b));
    b.size = size;
    b.box = sudoku_box_size(size);
    b.n = n;
    b.options = options;
    b.solution = solution;
    b.stats = stats;
    b.board = (int *)calloc((size_t)n, sizeof(int));
    b.rowUsed = (uint64_t *)calloc((size_t)size, sizeof(uint64_t));
    b.colUsed = (uint64_t *)calloc((size_t)size, sizeof(uint64_t));
    b.boxUsed = (uint64_t *)calloc((size_t)size, sizeof(uint64_t));
    b.diagUsed = (uint64_t *)calloc(2, sizeof(uint64_t));
    if (!b.board || !b.rowUsed || !b.colUsed || !b.boxUsed || !b.diagUsed) {
        free(b.board); free(b.rowUsed); free(b.colUsed); free(b.boxUsed); free(b.diagUsed);
        return 0;
    }

    int inconsistent = inconsistent_givens(&b, givens);
    if (!inconsistent) search(&b);

    free(b.board); free(b.rowUsed); free(b.colUsed); free(b.boxUsed); free(b.diagUsed);
    return b.found;
}

int bitmask_count_solutions(int size, const int *givens, int limit,
                            long long maxNodes, SolverStats *stats) {
    if (limit <= 0) limit = 0;
    BitmaskOptions options = {limit, maxNodes};
    int count = bitmask_solve(size, givens, NULL, &options, stats);
    return stats && stats->aborted ? -1 : count;
}
