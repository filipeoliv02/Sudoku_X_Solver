#ifndef SUDOKU_DLX_H
#define SUDOKU_DLX_H

/**
 * @file dlx.h
 * @brief Sudoku X as an exact-cover problem, solved with Knuth's Algorithm X on Dancing Links (DLX).
 */

#include "solver_common.h"

/** Search options. A zero-initialised struct means: find every solution, no node limit, deterministic order. */
typedef struct {
    int maxSolutions;   /**< Stop after this many solutions (0 = enumerate all). */
    long long maxNodes; /**< Abort after trying this many rows (0 = unlimited); sets SolverStats::aborted. */
    SudokuRng *rng;     /**< If not NULL, rows of the chosen column are tried starting at a random offset. */
} DlxOptions;

/**
 * @brief Solves a board with Dancing Links.
 * @param size     Board side (must satisfy sudoku_is_valid_size()).
 * @param givens   size*size values, row-major, 0 = empty. Not modified.
 * @param solution If not NULL, receives the first solution found (size*size values).
 * @param options  May be NULL (enumerate all solutions).
 * @param stats    May be NULL; otherwise receives the node/backtrack counters.
 * @return Number of solutions found (0 if the givens are inconsistent or unsolvable, or the size is invalid). When the
 * search is aborted by the node limit the count found so far is returned and stats->aborted is set.
 */
int dlx_solve(int size, const int *givens, int *solution, const DlxOptions *options, SolverStats *stats);

/**
 * @brief Counts solutions up to @p limit (e.g. 2 to test uniqueness).
 * @return The number of solutions found (<= limit), or -1 if the node limit was hit before the answer was known.
 */
int dlx_count_solutions(int size, const int *givens, int limit, long long maxNodes, SolverStats *stats);

#endif // SUDOKU_DLX_H
