#include "fase1/menus.h"
#include "fase1/clients.h"
#include "fase1/fileio.h"
#include "fase1/utils.h"
#include "fase2/clients_linked.h"
#include "fase2/fileio_linked.h"
#include "fase2/utils_linked.h"

#include <stdio.h>
#include <string.h>

typedef struct {
    const char *name;
    void (*run)(void);
    const char *description;
} ClientEntry;

static void client_compare_optimized(void) { client_compare_algorithms(0); }

static void client_compare_all(void) { client_compare_algorithms(1); }

static const ClientEntry CLIENTS[] = {
        // Fase 1 (arrays)
        {"bruteforce",               client_bruteforce,                 "fase1: brute force on client_test_files/unsolved_client.txt"},
        {"advanced",                 client_advanced,                   "fase1: optimized solver on client_test_files/unsolved_client.txt"},
        {"read",                     client_read,                       "fase1: read client_test_files/client_read.txt"},
        {"show-stored",              client_show_stored,                "fase1: print boards stored in memory"},
        {"write",                    client_write,                      "fase1: write client_test_files/client_write.txt"},
        {"check-consistency",        client_check_consistency,          "fase1: consistency check"},
        {"random-generation",        client_random_generation,          "fase1: random board generation"},
        {"solve-variable-size",      client_solve_variable_size,        "fase1: solve 4x4 .. 36x36 sample boards"},
        {"search-solutions",         client_search_solutions,           "fase1: search known solutions"},
        {"compare",                  client_compare_optimized,          "fase1: benchmark the optimized solver"},
        {"compare-all",              client_compare_all,                "fase1: benchmark optimized and brute force (slow)"},
        // Fase 2 (linked lists)
        {"linked-bruteforce",        client_linked_bruteforce,          "fase2: brute force on data/unsolved/unsolved.txt"},
        {"linked-optimized",         client_linked_optimized,           "fase2: optimized solver on data/unsolved/unsolved.txt"},
        {"linked-random-generation", client_linked_random_generation,   "fase2: random board generation"},
        {"linked-solve-variable-size", client_linked_solve_variable_size, "fase2: solve data/unsolved/unsolved_all_sizes.txt"},
        {"linked-read-write-txt",    client_linked_read_write_txt,      "fase2: text file round trip"},
        {"linked-read-write-bin",    client_linked_read_write_bin,      "fase2: binary file round trip"},
        {"linked-check-consistency", client_linked_check_consistency,   "fase2: consistency check"},
};

static const char *const ALGORITHMS[] = {"optimized", "bruteforce", "linked-optimized", "linked-bruteforce"};

static void usage(FILE *out, const char *prog) {
    fprintf(out,
            "Sudoku X solver\n\n"
            "Usage:\n"
            "  %s                         interactive menu (reads ./unsolved.txt, writes ./solved.txt)\n"
            "  %s --solve FILE [--algorithm NAME]\n"
            "                             solve every board in FILE and print the solutions\n"
            "  %s --client NAME           run one of the original demo clients\n"
            "  %s --list-clients          list the demo clients\n"
            "  %s --help\n\n"
            "Algorithms: optimized (default), bruteforce, linked-optimized, linked-bruteforce\n"
            "Exit status: 0 = every board solved, 1 = at least one board unsolved/invalid, 2 = usage or I/O error\n",
            prog, prog, prog, prog, prog);
}

static int is_known_algorithm(const char *name) {
    for (size_t i = 0; i < sizeof(ALGORITHMS) / sizeof(ALGORITHMS[0]); i++) {
        if (strcmp(name, ALGORITHMS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static int solve_file_arrays(const char *file, int bruteforce) {
    SudokuList unsolved = load_sudokus(file);
    if (unsolved.total == 0) {
        fprintf(stderr, "No boards loaded from '%s'\n", file);
        return 2;
    }
    int unsolvedCount = 0;
    for (int i = 0; i < unsolved.total; i++) {
        Sudoku s = *(unsolved.sudokus + i);
        SudokuList solved = {0, NULL, NULL};
        long long cost = 0;
        printf("=== Board %d (%dx%d) ===\n", i + 1, s.size, s.size);
        if (!isConsistent(s)) {
            printf("Inconsistent board\n");
            unsolvedCount++;
            continue;
        }
        if (bruteforce) {
            solveSudokuBruteForce(&solved, s, 0, 0, &cost);
        } else {
            solveSudokuOptimized(s, &solved, &cost);
        }
        printf("Solutions: %d, cost: %lld\n", solved.total, cost);
        if (solved.total == 0) {
            unsolvedCount++;
        }
        free_list_sudoku(solved);
    }
    free_list_sudoku(unsolved);
    return unsolvedCount == 0 ? 0 : 1;
}

static int solve_file_linked(const char *file, int bruteforce) {
    SudokuQueue *unsolved = loadSudokuQueueFromFile(file);
    if (unsolved->total == 0) {
        fprintf(stderr, "No boards loaded from '%s'\n", file);
        freeSudokuQueue(unsolved);
        return 2;
    }
    int unsolvedCount = 0, index = 0;
    while (unsolved->first != NULL) {
        SudokuQueueNode *sudoku = dequeueSudoku(unsolved);
        SudokuQueue *solved = createSudokuQueue();
        printf("=== Board %d (%dx%d) ===\n", ++index, sudoku->size, sudoku->size);
        if (!isConsistentLinked(sudoku)) {
            printf("Inconsistent board\n");
        } else if (bruteforce) {
            solveLinkedSudokuBruteForce(solved, sudoku, sudoku->first);
        } else {
            solveLinkedSudokuOptimized(solved, sudoku);
        }
        printf("Solutions: %d\n", solved->total);
        if (solved->total == 0) {
            unsolvedCount++;
        }
        freeSudokuQueue(solved);
        freeSudoku(sudoku);
    }
    freeSudokuQueue(unsolved);
    return unsolvedCount == 0 ? 0 : 1;
}

/**
 * @brief Início do Programa usado para alternar entre menus e clientes
 * @details Sem argumentos abre o menu principal; com argumentos permite resolver ficheiros ou correr os clientes
 * de demonstração de cada fase.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char const *argv[]) {
    const char *prog = argc > 0 ? argv[0] : "sudoku";
    if (argc <= 1) {
        main_menu();
        return 0;
    }

    const char *solveFile = NULL, *algorithm = "optimized", *client = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(stdout, prog);
            return 0;
        } else if (strcmp(argv[i], "--list-clients") == 0) {
            for (size_t c = 0; c < sizeof(CLIENTS) / sizeof(CLIENTS[0]); c++) {
                printf("  %-28s %s\n", CLIENTS[c].name, CLIENTS[c].description);
            }
            return 0;
        } else if (strcmp(argv[i], "--solve") == 0 && i + 1 < argc) {
            solveFile = argv[++i];
        } else if (strcmp(argv[i], "--algorithm") == 0 && i + 1 < argc) {
            algorithm = argv[++i];
        } else if (strcmp(argv[i], "--client") == 0 && i + 1 < argc) {
            client = argv[++i];
        } else {
            fprintf(stderr, "Unknown or incomplete option: %s\n\n", argv[i]);
            usage(stderr, prog);
            return 2;
        }
    }

    if (client != NULL) {
        for (size_t c = 0; c < sizeof(CLIENTS) / sizeof(CLIENTS[0]); c++) {
            if (strcmp(client, CLIENTS[c].name) == 0) {
                CLIENTS[c].run();
                return 0;
            }
        }
        fprintf(stderr, "Unknown client '%s' (see --list-clients)\n", client);
        return 2;
    }

    if (solveFile == NULL) {
        usage(stderr, prog);
        return 2;
    }
    if (!is_known_algorithm(algorithm)) {
        fprintf(stderr, "Unknown algorithm '%s'\n", algorithm);
        return 2;
    }
    if (strncmp(algorithm, "linked-", 7) == 0) {
        return solve_file_linked(solveFile, strcmp(algorithm, "linked-bruteforce") == 0);
    }
    return solve_file_arrays(solveFile, strcmp(algorithm, "bruteforce") == 0);
}
