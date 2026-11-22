#ifndef SUDOKU_COMMON_H
#define SUDOKU_COMMON_H

#include <stddef.h>
#include <stdio.h>

/**
 * @file sudoku_common.h
 * @brief Limits and helpers shared by the array based (fase1) and linked (fase2) implementations.
 */

/** Smallest supported board side (2x2 boxes). */
#define SUDOKU_MIN_SIZE 4

/**
 * Largest supported board side (8x8 boxes). The binary file format stores every value in a single byte, so this
 * must never exceed 255; it also bounds the counting sort in computeOrderBySize().
 */
#define SUDOKU_MAX_SIZE 64

/**
 * Boards up to this side are played as Sudoku X (both main diagonals must hold unique values). Larger boards are
 * solved as classic Sudoku because diagonal constraints make random/sparse large boards impractical to solve.
 */
#define SUDOKUX_MAX_DIAGONAL_SIZE 16

/** Binary files store counts and values in one unsigned byte each. */
#define SUDOKU_BIN_MAX_VALUE 255

/**
 * @brief Integer square root of a board side (the side of a box).
 * @return The box side, or 0 if @p size is not a perfect square.
 */
int sudoku_box_size(int size);

/**
 * @brief Checks whether @p size is a supported board side: a perfect square within
 * [SUDOKU_MIN_SIZE, SUDOKU_MAX_SIZE].
 */
int sudoku_is_valid_size(int size);

/**
 * @brief Reads the next whitespace separated integer from @p fp.
 * @details Unlike fscanf("%d"), whose behaviour is undefined when the number does not fit in an int, the token is read
 * with a bounded width and converted with strtol() and an explicit range check.
 * @return 1 on success, 0 if the next token is not a valid int (too long, not numeric, out of range), EOF at end of
 * input.
 */
int sudoku_read_int(FILE *fp, int *out);

/** malloc() that terminates the program with a message when memory is exhausted. */
void *xmalloc(size_t size);

/** calloc() that terminates the program with a message when memory is exhausted or the size overflows. */
void *xcalloc(size_t count, size_t size);

/** realloc() for arrays that terminates the program when memory is exhausted or the size overflows. */
void *xreallocarray(void *ptr, size_t count, size_t size);

#endif // SUDOKU_COMMON_H
