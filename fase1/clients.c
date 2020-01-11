#include "clients.h"
#include "fileio.h"
#include "../fase2/fileio_linked.h"
#include "utils.h"
#include "algorithms.h"
#include <stdio.h>
#include <math.h>

/**
 * @brief Cliente Bruteforce
 * @details Requisito 4-A
 */
void client_bruteforce() {
    SudokuList unsolved = load_sudokus("client_test_files/unsolved_client.txt");
    SudokuList solved = {0, NULL, NULL};

    long long cost;
    for (int i = 0; i < unsolved.total; i++) {
        cost = 0;
        solved.sudokus = NULL;
        solved.total = 0;
        solveSudokuBruteForce(&solved, *(unsolved.sudokus + i), 0, 0, &cost);
        if (solved.total == 0) {
            printf("ID: %d / Size: %d / Cost %lld Nao se encontrou solucoes\n", i, (unsolved.sudokus + i)->size, cost);
        } else {
            printf("ID: %d / Size: %d / Cost: %lld\n", i, (unsolved.sudokus + i)->size, cost);
            for (int j = 0; j < solved.total; j++) {
                printSudoku((solved.sudokus + j)->board, (solved.sudokus + j)->size);
            }
        }
        printf("\n");
    }
}

/**
 * @brief Cliente Algoritmo Optimizado
 * @details Requisito 4-B
 */
void client_advanced() {
    SudokuList unsolved = load_sudokus("client_test_files/unsolved_client.txt");
    SudokuList solved = {0, NULL, NULL};

    long long cost;
    for (int i = 0; i < unsolved.total; i++) {
        cost = 0;
        solved.sudokus = NULL;
        solved.total = 0;
        solveSudokuOptimized(*(unsolved.sudokus + i), &solved, &cost);
        if (solved.total == 0) {
            printf("ID: %d / Size: %d / Cost %lld Nao se encontrou solucoes\n", i, (unsolved.sudokus + i)->size, cost);
        } else {
            printf("ID: %d / Size: %d / Cost: %lld\n", i, (solved.sudokus + i)->size, cost);
            for (int j = 0; j < solved.total; j++) {
                printSudoku((solved.sudokus + j)->board, (solved.sudokus + j)->size);
            }
        }
        printf("\n");
    }
}

/**
 * @brief Cliente de leitura do Ficheiro
 * @details Requisito 1-B
 */
void client_read() {
    SudokuList sudokus = load_sudokus("client_test_files/client_read.txt");
    printAllStoredBoards(sudokus, 0);
}

/**
 * @brief Cliente de Gravação
 * @details Requisito 8
 */
void client_write() {
    SudokuList sudokus = load_sudokus("client_test_files/client_read.txt");
    save_sudokus(sudokus, "client_test_files/client_write.txt");
}

/**
 * @brief Cliente para mostrar os tabuleiros em memória
 * @details Requisito 2
 */
void client_show_stored() {
    SudokuList sudokus = load_sudokus("client_test_files/client_read.txt");
    printAllStoredBoards(sudokus, 0);
}

/**
 * @brief Cliente de teste da Consistência de um tabuleiro
 * @details Requisito 3
 */
void client_check_consistency() {
    SudokuList sudokus = load_sudokus("client_test_files/client_read.txt");
    for (int i = 0; i < sudokus.total; i++) {
        if (isConsistent(*(sudokus.sudokus + i))) {
            printf("%d: Consistente\n", i + 1);
        } else {
            printf("%d: Nao consistente\n", i + 1);
        }
    }
}

/**
 * @brief Cliente de geração de um tabuleiro aleatório
 * @details Requisito 1-A
 */
void client_random_generation() {
    int sizes[] = {25, 9, 9, 36, 9, 16}, cells_filled[] = {100, 30, 15, 500, 50, 50};
    for (int i = 0; i < sizeof(sizes) / sizeof(int); i++) {
        Sudoku rand_sudoku = gen_sudoku(*(sizes + i), *(cells_filled + i));
        if (isConsistent(rand_sudoku)) {
            printf("Tabuleiro consistente ");
        } else {
            printf("Tabuleiro nao consistente ");
        }
        printf("gerado de tamanho %dx%d, com %d celulas preenchidas:\n", *(sizes + i), *(sizes + i),
               *(cells_filled + i));

        printSudoku(rand_sudoku.board, rand_sudoku.size);
    }
}

/**
 * @brief Cliente para resolver tabuleiros de tamanho variável
 * @details Requisito 5
 */
void client_solve_variable_size() {
    SudokuList solved = {0, NULL, NULL};
    long long cost = 0;

    printf("\nTestar tabuleiros 4x4:\n");
    SudokuList sudoku4 = load_sudokus("client_test_files/client_4x4.txt");
    for (int i = 0; i < sudoku4.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        solveSudokuOptimized(*(sudoku4.sudokus + i), &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku((solved.sudokus + j)->board, (solved.sudokus + j)->size);
            }
        }
    }

    printf("\nTestar tabuleiros 9x9:\n");
    SudokuList sudoku9 = load_sudokus("client_test_files/client_9x9.txt");
    for (int i = 0; i < sudoku9.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        solveSudokuOptimized(*(sudoku9.sudokus + i), &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku((solved.sudokus + j)->board, (solved.sudokus + j)->size);
            }
        }
    }

    printf("\nTestar tabuleiros 16x16:\n");
    SudokuList sudoku16 = load_sudokus("client_test_files/client_16x16.txt");
    for (int i = 0; i < sudoku16.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        solveSudokuOptimized(*(sudoku16.sudokus + i), &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku((solved.sudokus + j)->board, (solved.sudokus + j)->size);
            }
        }
    }

    printf("\nTestar tabuleiros 25x25:\n");
    SudokuList sudoku25 = load_sudokus("client_test_files/client_25x25.txt");
    for (int i = 0; i < sudoku25.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        solveSudokuOptimized(*(sudoku25.sudokus + i), &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku((solved.sudokus + j)->board, (solved.sudokus + j)->size);
            }
        }
    }

    printf("\nTestar tabuleiros 36x36:\n");
    SudokuList sudoku36 = load_sudokus("client_test_files/client_36x36.txt");
    for (int i = 0; i < sudoku36.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        solveSudokuOptimized(*(sudoku36.sudokus + i), &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku((solved.sudokus + j)->board, (solved.sudokus + j)->size);
            }
        }
    }
}

/**
 * @brief Cliente de Procura de tabuleiros numa lista de soluções
 * @details Requisito 6
 */
void client_search_solutions() {
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
}

/**
 * @brief Cliente de Análise e Comparação dos algoritmos Bruteforce e  otimizado
 * @details Requisito 7
 */
void client_compare_algorithms() {
    long long time_usec_init, time_usec_end, cost;
    long elapsed_time;
    long long average_cost4 = 0, average_cost9 = 0, average_cost16 = 0, average_cost25 = 0;
    long average_time4 = 0, average_time9 = 0, average_time16 = 0, average_time25 = 0;
    long long count4 = 0, count9 = 0, count16 = 0, count25 = 0;
    SudokuList unsolved = load_sudokus("client_test_files/unsolved_client.txt");
    SudokuList solved = {0, NULL, NULL};
    for (int i = 0; i < unsolved.total; i++) {
        cost = 0;
        gettimeuseconds(&time_usec_init); // init time
        solveSudokuOptimized(*(unsolved.sudokus + i), &solved, &cost);
        gettimeuseconds(&time_usec_end); // end time
        elapsed_time = (long) (time_usec_end - time_usec_init);

        switch ((unsolved.sudokus + i)->size) {
            case 4:
                average_cost4 += cost;
                average_time4 += elapsed_time;
                count4++;
                break;
            case 9:
                average_cost9 += cost;
                average_time9 += elapsed_time;
                count9++;
                break;
            case 16:
                average_cost16 += cost;
                average_time16 += elapsed_time;
                count16++;
                break;
            case 25:
                average_cost25 += cost;
                average_time25 += elapsed_time;
                count25++;
                break;
            default:
                break;
        }
    }
    printf("\n\nOtimizado\n");
    if (count4 != 0) {
        cost = average_cost4 / count4;
        elapsed_time = average_time4 / count4;
        printf("Sudokus 4x4:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,
               (elapsed_time * pow(10, -6)), cost);
    }

    if (count9 != 0) {
        cost = average_cost9 / count9;
        elapsed_time = average_time9 / count9;
        printf("Sudokus 9x9:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,
               (elapsed_time * pow(10, -6)), cost);
    }

    if (count16 != 0) {
        cost = average_cost16 / count16;
        elapsed_time = average_time16 / count16;
        printf("Sudokus 16x16:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,
               (elapsed_time * pow(10, -6)), cost);
    }

    if (count25 != 0) {
        cost = average_cost25 / count25;
        elapsed_time = average_time25 / count25;
        printf("Sudokus 25x25:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,
               (elapsed_time * pow(10, -6)), cost);
    }
    average_cost4 = 0, average_cost9 = 0, average_cost16 = 0;
    average_time4 = 0, average_time9 = 0, average_time16 = 0;
    count4 = 0, count9 = 0, count16 = 0;
    /*free_list_sudoku(unsolved);
    unsolved = load_sudokus("client_test_files/unsolved_client.txt");

    for (int i = 0; i < unsolved.total; i++) {
        cost = 0;
        gettimeuseconds(&time_usec_init); // init time
        solveSudokuBruteForce(&solved, *(unsolved.sudokus + i), 0, 0, &cost);
        gettimeuseconds(&time_usec_end); // end time
        elapsed_time = (long) (time_usec_end - time_usec_init);

        switch ((unsolved.sudokus + i)->size) {
            case 4:
                average_cost4 += cost;
                average_time4 += elapsed_time;
                count4++;
                break;
            case 9:
                average_cost9 += cost;
                average_time9 += elapsed_time;
                count9++;
                break;
            case 16:
                average_cost16 += cost;
                average_time16 += elapsed_time;
                count16++;
                break;
            case 25:
                average_cost25 += cost;
                average_time25 += elapsed_time;
                count25++;
                break;
            default:
                break;
        }
    }
    printf("\n\nBruteforce\n");
    if (count4 != 0) {
        cost = average_cost4 / count4;
        elapsed_time = average_time4 / count4;
        printf("Sudokus 4x4:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,
               (elapsed_time * pow(10, -6)), cost);
    }

    if (count9 != 0) {
        cost = average_cost9 / count9;
        elapsed_time = average_time9 / count9;
        printf("Sudokus 9x9:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,
               (elapsed_time * pow(10, -6)), cost);
    }

    if (count16 != 0) {
        cost = average_cost16 / count16;
        elapsed_time = average_time16 / count16;
        printf("Sudokus 16x16:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,
               (elapsed_time * pow(10, -6)), cost);
    }

    if (count25 != 0) {
        cost = average_cost25 / count25;
        elapsed_time = average_time25 / count25;
        printf("Sudokus 25x25:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,
               (elapsed_time * pow(10, -6)), cost);
    }
*/
    //free_list_sudoku(unsolved);
    //free_list_sudoku(solved);

    average_cost4 = 0, average_cost9 = 0, average_cost16 = 0;
    average_time4 = 0, average_time9 = 0, average_time16 = 0;
    count4 = 0, count9 = 0, count16 = 0;
    SudokuLinkedNode *sudokus = load_sudokus_link("client_test_files/unsolved_client.txt");

    while(sudokus != NULL) {
        gettimeuseconds(&time_usec_init); // init time
        solveSudokuOptimizedLink(*sudokus);
        gettimeuseconds(&time_usec_end); // end time
        elapsed_time = (long) (time_usec_end - time_usec_init);

        switch (sudokus->size) {
            case 4:
                average_time4 += elapsed_time;
                count4++;
                break;
            case 9:
                average_time9 += elapsed_time;
                count9++;
                break;
            case 16:
                average_time16 += elapsed_time;
                count16++;
                break;
            case 25:
                average_time25 += elapsed_time;
                count25++;
                break;
            default:
                break;
        }
        sudokus = sudokus->next;
    }
    printf("\n\nOtimizado Listas Ligadas\n");
    if (count4 != 0) {
        elapsed_time = average_time4 / count4;
        printf("Sudokus 4x4:\nAverage time: usec %ld - sec %lf\n", elapsed_time, (elapsed_time * pow(10, -6)));
    }

    if (count9 != 0) {
        elapsed_time = average_time9 / count9;
        printf("Sudokus 9x9:\nAverage time: usec %ld - sec %lf\n", elapsed_time, (elapsed_time * pow(10, -6)));
    }

    if (count16 != 0) {
        elapsed_time = average_time16 / count16;
        printf("Sudokus 16x16:\nAverage time: usec %ld - sec %lf\n", elapsed_time, (elapsed_time * pow(10, -6)));
    }

    if (count25 != 0) {
        elapsed_time = average_time25 / count25;
        printf("Sudokus 25x25:\nAverage time: usec %ld - sec %lf\n", elapsed_time, (elapsed_time * pow(10, -6)));
    }
}

