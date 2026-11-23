#ifndef SUDOKU_SOLVER_COMMON_H
#define SUDOKU_SOLVER_COMMON_H

/**
 * @file solver_common.h
 * @brief Types and helpers shared by the flat-grid solvers (DLX, bitmask), the generator and the benchmark.
 *
 * These solvers work on a flat, row-major board: `cells[r * size + c]`, `0` meaning an empty cell. The same rules as
 * the rest of the project apply: rows, columns and boxes always, both main diagonals only for boards up to
 * SUDOKUX_MAX_DIAGONAL_SIZE.
 */

#include "common/sudoku_common.h"

#include <stdint.h>

/** Work counters reported by a search. They make solvers comparable independently of the machine. */
typedef struct {
    long long nodes;      /**< Search-tree nodes: placements tried by the search (guesses / DLX rows chosen). */
    long long backtracks; /**< Dead ends: branches abandoned because a contradiction was found. */
    int aborted;          /**< 1 if the search stopped because it hit its node limit (result is incomplete). */
} SolverStats;

/** Whether the two main diagonals are constraints for a board of side @p size (the project's Sudoku X rule). */
static inline int sudoku_has_diagonals(int size) { return size <= SUDOKUX_MAX_DIAGONAL_SIZE; }

/**
 * @brief Small, seedable pseudo-random generator (SplitMix64).
 * @details The generator must be reproducible from a seed (`--seed`) and independent of the C library's rand(), whose
 * sequence differs between platforms.
 */
typedef struct {
    uint64_t state;
} SudokuRng;

static inline uint64_t sudoku_rng_next(SudokuRng *rng) {
    uint64_t z = (rng->state += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
}

/** Uniform integer in [0, bound), bound > 0 (the modulo bias is negligible for board-sized bounds). */
static inline int sudoku_rng_below(SudokuRng *rng, int bound) {
    return (int) (sudoku_rng_next(rng) % (uint64_t) bound);
}

/** Number of set bits. */
static inline int bits_popcount(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int n = 0;
    while (x) {
        x &= x - 1;
        n++;
    }
    return n;
#endif
}

/** Index of the lowest set bit; @p x must not be 0. */
static inline int bits_lowest(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(x);
#else
    int n = 0;
    while (!(x & 1u)) {
        x >>= 1;
        n++;
    }
    return n;
#endif
}

/** Mask with the low @p n bits set, valid for n in [0, 64]. */
static inline uint64_t bits_low_mask(int n) { return n >= 64 ? UINT64_MAX : ((UINT64_C(1) << n) - 1); }

#endif // SUDOKU_SOLVER_COMMON_H
