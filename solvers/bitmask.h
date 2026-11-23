#ifndef SUDOKU_BITMASK_H
#define SUDOKU_BITMASK_H

#include "solver_common.h"

/** Options for the bitmask MRV search. */
typedef struct {
    int maxSolutions;
    long long maxNodes;
} BitmaskOptions;

/** Solves/ enumerates a flat row-major board using MRV and 64-bit candidate masks. */
int bitmask_solve(int size, const int *givens, int *solution,
                  const BitmaskOptions *options, SolverStats *stats);

int bitmask_count_solutions(int size, const int *givens, int limit,
                            long long maxNodes, SolverStats *stats);

#endif
