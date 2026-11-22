#include "clients.h"
#include "fileio.h"
#include "utils.h"
#include "algorithms.h"
#include <stdio.h>

/**
 * @brief Liberta e reinicia uma lista de soluções
 * @param list
 */
static void reset_list(SudokuList *list) {
    free_list_sudoku(*list);
    list->total = 0;
    list->sudokus = NULL;
    list->orderedList = NULL;
}

/**
 * @brief Imprime as soluções encontradas para um tabuleiro
 * @param solved
 */
static void print_solutions(SudokuList solved) {
    for (int j = 0; j < solved.total; j++) {
        printSudoku((solved.sudokus + j)->board, (solved.sudokus + j)->size);
    }
}

/**
 * @brief Cliente Bruteforce
 * @details Requisito 4-A
 */
void client_bruteforce(void) {
    SudokuList unsolved = load_sudokus("client_test_files/unsolved_client.txt");
    SudokuList solved = {0, NULL, NULL};

    long long cost;
    for (int i = 0; i < unsolved.total; i++) {
        cost = 0;
        reset_list(&solved);
        solveSudokuBruteForce(&solved, *(unsolved.sudokus + i), 0, 0, &cost);
        if (solved.total == 0) {
            printf("ID: %d / Size: %d / Cost %lld Nao se encontrou solucoes\n", i, (unsolved.sudokus + i)->size, cost);
        } else {
            printf("ID: %d / Size: %d / Cost: %lld\n", i, (unsolved.sudokus + i)->size, cost);
            print_solutions(solved);
        }
        printf("\n");
    }
    free_list_sudoku(solved);
    free_list_sudoku(unsolved);
}

/**
 * @brief Cliente Algoritmo Optimizado
 * @details Requisito 4-B
 */
void client_advanced(void) {
    SudokuList unsolved = load_sudokus("client_test_files/unsolved_client.txt");
    SudokuList solved = {0, NULL, NULL};

    long long cost;
    for (int i = 0; i < unsolved.total; i++) {
        cost = 0;
        reset_list(&solved);
        solveSudokuOptimized(*(unsolved.sudokus + i), &solved, &cost);
        if (solved.total == 0) {
            printf("ID: %d / Size: %d / Cost %lld Nao se encontrou solucoes\n", i, (unsolved.sudokus + i)->size, cost);
        } else {
            printf("ID: %d / Size: %d / Cost: %lld\n", i, (unsolved.sudokus + i)->size, cost);
            print_solutions(solved);
        }
        printf("\n");
    }
    free_list_sudoku(solved);
    free_list_sudoku(unsolved);
}

/**
 * @brief Cliente de leitura do Ficheiro
 * @details Requisito 1-B
 */
void client_read(void) {
    SudokuList sudokus = load_sudokus("client_test_files/client_read.txt");
    printAllStoredBoards(sudokus, 0);
    free_list_sudoku(sudokus);
}

/**
 * @brief Cliente de Gravação
 * @details Requisito 8
 */
void client_write(void) {
    SudokuList sudokus = load_sudokus("client_test_files/client_read.txt");
    save_sudokus(sudokus, "client_test_files/client_write.txt");
    free_list_sudoku(sudokus);
}

/**
 * @brief Cliente para mostrar os tabuleiros em memória
 * @details Requisito 2
 */
void client_show_stored(void) {
    SudokuList sudokus = load_sudokus("client_test_files/client_read.txt");
    printAllStoredBoards(sudokus, 0);
    free_list_sudoku(sudokus);
}

/**
 * @brief Cliente de teste da Consistência de um tabuleiro
 * @details Requisito 3
 */
void client_check_consistency(void) {
    SudokuList sudokus = load_sudokus("client_test_files/client_read.txt");
    for (int i = 0; i < sudokus.total; i++) {
        if (isConsistent(*(sudokus.sudokus + i))) {
            printf("%d: Consistente\n", i + 1);
        } else {
            printf("%d: Nao consistente\n", i + 1);
        }
    }
    free_list_sudoku(sudokus);
}

/**
 * @brief Cliente de geração de um tabuleiro aleatório
 * @details Requisito 1-A
 */
void client_random_generation(void) {
    const int sizes[] = {25, 9, 9, 36, 9, 16}, cells_filled[] = {100, 30, 15, 500, 50, 50};
    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        Sudoku rand_sudoku = gen_sudoku(sizes[i], cells_filled[i]);
        if (isConsistent(rand_sudoku)) {
            printf("Tabuleiro consistente ");
        } else {
            printf("Tabuleiro nao consistente ");
        }
        printf("gerado de tamanho %dx%d, com %d celulas preenchidas:\n", sizes[i], sizes[i], cells_filled[i]);

        printSudoku(rand_sudoku.board, rand_sudoku.size);
        freeBoard(rand_sudoku.board, rand_sudoku.size);
    }
}

/**
 * @brief Cliente para resolver tabuleiros de tamanho variável
 * @details Requisito 5
 */
void client_solve_variable_size(void) {
    static const struct {
        const char *label;
        const char *file;
    } cases[] = {
            {"4x4",   "client_test_files/client_4x4.txt"},
            {"9x9",   "client_test_files/client_9x9.txt"},
            {"16x16", "client_test_files/client_16x16.txt"},
            {"25x25", "client_test_files/client_25x25.txt"},
            {"36x36", "client_test_files/client_36x36.txt"},
    };
    SudokuList solved = {0, NULL, NULL};
    long long cost = 0;

    for (size_t c = 0; c < sizeof(cases) / sizeof(cases[0]); c++) {
        printf("\nTestar tabuleiros %s:\n", cases[c].label);
        SudokuList sudokus = load_sudokus(cases[c].file);
        for (int i = 0; i < sudokus.total; i++) {
            reset_list(&solved);
            printf("Id[%d] - ", i);
            solveSudokuOptimized(*(sudokus.sudokus + i), &solved, &cost);
            if (solved.total == 0) {
                printf("Nao se encontrou solucoes\n\n");
            } else {
                printf("Solucoes:\n");
                print_solutions(solved);
            }
        }
        free_list_sudoku(sudokus);
    }
    free_list_sudoku(solved);
}

/**
 * @brief Cliente de Procura de tabuleiros numa lista de soluções
 * @details Requisito 6
 */
void client_search_solutions(void) {
    SudokuList solved = load_sudokus("client_test_files/client_search_solutions.txt");
    SudokuList unsolved = load_sudokus("client_test_files/client_9x9.txt");
    int searchResult;
    for (int i = 0; i < unsolved.total; i++) {
        searchResult = searchSudokus(solved, *(unsolved.sudokus + i));
        printf("Tabuleiro Original:\n");
        printSudoku((unsolved.sudokus + i)->board, (unsolved.sudokus + i)->size);
        if (searchResult == -1) {
            printf("Solucao nao encontrada.\n");

        } else {
            printf("Solucao encontrada com sucesso:\n");
            printSudoku((solved.sudokus + searchResult)->board, (solved.sudokus + searchResult)->size);
        }
        printf("\n\n");
    }
    free_list_sudoku(solved);
    free_list_sudoku(unsolved);
}

typedef void (*SolverFn)(Sudoku sudoku, SudokuList *solved, long long *cost);

static void run_optimized(Sudoku sudoku, SudokuList *solved, long long *cost) {
    solveSudokuOptimized(sudoku, solved, cost);
}

static void run_bruteforce(Sudoku sudoku, SudokuList *solved, long long *cost) {
    solveSudokuBruteForce(solved, sudoku, 0, 0, cost);
}

/**
 * @brief Mede o custo e o tempo médio de um algoritmo por tamanho de tabuleiro
 * @param name
 * @param solver
 * @param unsolved
 */
static void benchmark(const char *name, SolverFn solver, SudokuList unsolved) {
    enum { SIZES = 4 };
    static const int sizes[SIZES] = {4, 9, 16, 25};
    long long total_cost[SIZES] = {0}, total_time[SIZES] = {0}, count[SIZES] = {0};
    SudokuList solved = {0, NULL, NULL};

    for (int i = 0; i < unsolved.total; i++) {
        // Os algoritmos alteram o tabuleiro durante a pesquisa, trabalhar sobre uma cópia
        Sudoku copy = copySudoku(*(unsolved.sudokus + i));
        long long cost = 0, time_usec_init, time_usec_end;
        gettimeuseconds(&time_usec_init); // init time
        solver(copy, &solved, &cost);
        gettimeuseconds(&time_usec_end); // end time
        freeBoard(copy.board, copy.size);

        for (int k = 0; k < SIZES; k++) {
            if (sizes[k] == copy.size) {
                total_cost[k] += cost;
                total_time[k] += time_usec_end - time_usec_init;
                count[k]++;
            }
        }
    }
    free_list_sudoku(solved);

    printf("\n\n%s\n", name);
    for (int k = 0; k < SIZES; k++) {
        if (count[k] != 0) {
            long long elapsed_time = total_time[k] / count[k];
            printf("Sudokus %dx%d:\nAverage time: usec %lld - sec %lf\nAverage cost: %lld\n", sizes[k], sizes[k],
                   elapsed_time, elapsed_time / 1e6, total_cost[k] / count[k]);
        }
    }
}

/**
 * @brief Cliente de Análise e Comparação dos algoritmos Bruteforce e  otimizado
 * @details Requisito 7
 * @param include_bruteforce Também mede o bruteforce (pode demorar muito em tabuleiros 16x16 ou maiores)
 */
void client_compare_algorithms(int include_bruteforce) {
    SudokuList unsolved = load_sudokus("client_test_files/unsolved_client.txt");
    benchmark("Otimizado", run_optimized, unsolved);
    if (include_bruteforce) {
        benchmark("Bruteforce", run_bruteforce, unsolved);
    }
    free_list_sudoku(unsolved);
}
