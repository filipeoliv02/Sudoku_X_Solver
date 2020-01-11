#include "clients_linked.h"
#include "../fase2/fileio_linked.h"
#include "../fase1/algorithms.h"

void client_linked_algorithms() {
    SudokuLinkedNode *queue = load_sudokus_link("unsolved.txt");
    solveSudokuBruteForceLink(*queue, queue->first);
    solveSudokuOptimizedLink(*queue);
}