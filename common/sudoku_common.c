#include "sudoku_common.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int sudoku_box_size(int size) {
    if (size <= 0) {
        return 0;
    }
    for (int root = 1; root <= size / root; root++) {
        if (root * root == size) {
            return root;
        }
    }
    return 0;
}

int sudoku_is_valid_size(int size) {
    return size >= SUDOKU_MIN_SIZE && size <= SUDOKU_MAX_SIZE && sudoku_box_size(size) != 0;
}

int sudoku_read_int(FILE *fp, int *out) {
    char token[32];
    if (fscanf(fp, "%31s", token) != 1) {
        return EOF;
    }
    // If the token filled the buffer and is followed by more non-space characters, it was truncated
    int next = fgetc(fp);
    if (next != EOF) {
        ungetc(next, fp);
        if (!isspace(next)) {
            return 0;
        }
    }

    char *end;
    errno = 0;
    long value = strtol(token, &end, 10);
    if (end == token || *end != '\0' || errno == ERANGE || value < INT_MIN || value > INT_MAX) {
        return 0;
    }
    *out = (int) value;
    return 1;
}

static void out_of_memory(void) {
    fputs("Fatal: out of memory\n", stderr);
    exit(EXIT_FAILURE);
}

void *xmalloc(size_t size) {
    void *ptr = malloc(size == 0 ? 1 : size);
    if (ptr == NULL) {
        out_of_memory();
    }
    return ptr;
}

void *xcalloc(size_t count, size_t size) {
    void *ptr = calloc(count == 0 ? 1 : count, size == 0 ? 1 : size);
    if (ptr == NULL) {
        out_of_memory();
    }
    return ptr;
}

void *xreallocarray(void *ptr, size_t count, size_t size) {
    if (size != 0 && count > SIZE_MAX / size) {
        out_of_memory();
    }
    size_t bytes = count * size;
    void *newPtr = realloc(ptr, bytes == 0 ? 1 : bytes);
    if (newPtr == NULL) {
        out_of_memory();
    }
    return newPtr;
}
