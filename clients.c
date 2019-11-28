#include "clients.h"
#include "fileio.h"
#include "utils.h"
#include "algorithms.h"
#include <stdio.h>
#include <math.h>

void client_bruteforce() {
    ListSudoku unsolved = load_sudokus("client_test_files/unsolved_client.txt");
    ListSudoku solved;

    long long cost;
    for (int i = 0; i < unsolved.total; i++) {
        cost = 0;
        solved.sudokus = NULL;
        solved.total = 0;
        findSudokuBruteForce(unsolved.sudokus[i].board, 0, 0, unsolved.sudokus[i].size, &solved, &cost);
        if (solved.total == 0) {
            printf("ID: %d / Size: %d / Cost %lld Nao se encontrou solucoes\n", i, unsolved.sudokus[i].size, cost);
        } else {
            printf("ID: %d / Size: %d / Cost: %lld\n", i, unsolved.sudokus[i].size, cost);
            for (int j = 0; j < solved.total; j++) {
                printSudoku(solved.sudokus[j].board, solved.sudokus[j].size);
            }
        }
        printf("\n");
    }
}

void client_advanced() {
    ListSudoku unsolved = load_sudokus("client_test_files/unsolved_client.txt");
    ListSudoku solved;

    long long cost;
    for (int i = 0; i < unsolved.total; i++) {
        cost = 0;
        solved.sudokus = NULL;
        solved.total = 0;
        findSudokuAdvanced(unsolved.sudokus[i], &solved, &cost);
        if (solved.total == 0) {
            printf("ID: %d / Size: %d / Cost %lld Nao se encontrou solucoes\n", i, unsolved.sudokus[i].size, cost);
        } else {
            printf("ID: %d / Size: %d / Cost: %lld\n", i, unsolved.sudokus[i].size, cost);
            for (int j = 0; j < solved.total; j++) {
                printSudoku(solved.sudokus[j].board, solved.sudokus[j].size);
            }
        }
        printf("\n");
    }
}

void client_read() {
    ListSudoku sudokus = load_sudokus("client_test_files/client_read.txt");
    printAllStoredBoards(sudokus);
}


void client_write(){
    ListSudoku sudokus = load_sudokus("client_test_files/client_read.txt");
    save_sudokus(sudokus, "client_test_files/client_write.txt");
}

void client_show_stored(){
    ListSudoku sudokus = load_sudokus("client_test_files/client_read.txt");
    printAllStoredBoards(sudokus);
}

void client_check_consistency() {
    ListSudoku sudokus = load_sudokus("client_test_files/client_read.txt");
    for(int i = 0; i < sudokus.total; i++) {
        if(isConsistent(sudokus.sudokus[i])) {
            printf("%d: Consistente\n", i+1);
        }
        else {
            printf("%d: Nao consistente\n", i+1);
        }
    }
}

void client_random_generation() {
    int sizes[] = {25, 9, 9, 36, 9, 16}, cells_filled[] = {100, 30, 15, 500, 50, 50};
    for(int i = 0; i < sizeof(sizes)/sizeof(int); i++) {
        Sudoku rand_sudoku = gen_sudoku(sizes[i], cells_filled[i]);
        if (isConsistent(rand_sudoku)) {
            printf("Tabuleiro consistente ");
        } else {
            printf("Tabuleiro nao consistente ");
        }
        printf("gerado de tamanho %dx%d, com %d celulas preenchidas:\n", sizes[i], sizes[i], cells_filled[i]);

        printSudoku(rand_sudoku.board, rand_sudoku.size);
    }
}

void client_solve_variable_size() {
    ListSudoku solved;
    long long cost = 0;

    printf("\nTestar tabuleiros 4x4:\n");
    ListSudoku sudoku4 = load_sudokus("client_test_files/client_4x4.txt");
    for(int i = 0; i < sudoku4.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        findSudokuAdvanced(sudoku4.sudokus[i], &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku(solved.sudokus[j].board, solved.sudokus[j].size);
            }
        }
    }

    printf("\nTestar tabuleiros 9x9:\n");
    ListSudoku sudoku9 = load_sudokus("client_test_files/client_9x9.txt");
    for(int i = 0; i < sudoku9.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        findSudokuAdvanced(sudoku9.sudokus[i], &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku(solved.sudokus[j].board, solved.sudokus[j].size);
            }
        }
    }

    printf("\nTestar tabuleiros 16x16:\n");
    ListSudoku sudoku16 = load_sudokus("client_test_files/client_16x16.txt");
    for(int i = 0; i < sudoku16.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        findSudokuAdvanced(sudoku16.sudokus[i], &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku(solved.sudokus[j].board, solved.sudokus[j].size);
            }
        }
    }

    printf("\nTestar tabuleiros 25x25:\n");
    ListSudoku sudoku25 = load_sudokus("client_test_files/client_25x25.txt");
    for(int i = 0; i < sudoku25.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        findSudokuAdvanced(sudoku25.sudokus[i], &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku(solved.sudokus[j].board, solved.sudokus[j].size);
            }
        }
    }
    printf("\nTestar tabuleiros 36x36:\n");
    ListSudoku sudoku36 = load_sudokus("client_test_files/client_36x36.txt");
    for(int i = 0; i < sudoku36.total; i++) {
        solved.total = 0;
        solved.sudokus = NULL;
        printf("Id[%d] - ", i);
        findSudokuAdvanced(sudoku36.sudokus[i], &solved, &cost);
        if (solved.total == 0) {
            printf("Nao se encontrou solucoes\n\n");
        } else {
            printf("Solucoes:\n");
            for (int j = 0; j < solved.total; j++) {
                printSudoku(solved.sudokus[j].board, solved.sudokus[j].size);
            }
        }
    }
}

void client_search_solutions() {
    ListSudoku solved = load_sudokus("client_test_files/client_search_solutions.txt");
    ListSudoku unsolved = load_sudokus("client_test_files/client_9x9.txt");
    int searchResult;
    for(int i = 0; i< unsolved.total; i++) {
        searchResult = searchSudokus(solved, unsolved.sudokus[i]);
        printf("Tabuleiro Original:\n");
        printSudoku(unsolved.sudokus[i].board, unsolved.sudokus[i].size);
        if (searchResult == -1) {
            printf("Solucao nao encontrada.\n");

        } else {
            printf("Solucao encontrada com sucesso:\n");
            printSudoku(solved.sudokus[searchResult].board, solved.sudokus[searchResult].size);
        }
        printf("\n\n");
    }
}

void client_compare_algorithms() {
    long long time_usec_init, time_usec_end, cost;
    long elapsed_time;
    long long average_cost4 = 0, average_cost9 = 0, average_cost16 = 0, average_cost25 = 0;
    long average_time4 = 0, average_time9 = 0, average_time16 = 0, average_time25 = 0;
    long long count4 = 0, count9 = 0, count16 = 0, count25 = 0;
    ListSudoku unsolved = load_sudokus("client_test_files/unsolved_client.txt");
    ListSudoku solved;
    solved.total = 0;
    solved.sudokus = NULL;
    for(int i = 0; i < unsolved.total; i++) {
        cost = 0;
        gettimeuseconds(&time_usec_init); // init time
        findSudokuAdvanced(unsolved.sudokus[i], &solved, &cost);
        gettimeuseconds(&time_usec_end); // end time
        elapsed_time = (long) (time_usec_end - time_usec_init);

        switch(unsolved.sudokus[i].size) {
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
    cost = average_cost4/count4;
    elapsed_time = average_time4/count4;
    printf("Sudokus 4x4:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);

    cost = average_cost9/count9;
    elapsed_time = average_time9/count9;
    printf("Sudokus 9x9:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);

    cost = average_cost16/count16;
    elapsed_time = average_time16/count16;
    printf("Sudokus 16x16:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);

    cost = average_cost25/count25;
    elapsed_time = average_time25/count25;
    printf("Sudokus 25x25:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);

    average_cost4 = 0, average_cost9 = 0, average_cost16 = 0;
    average_time4 = 0, average_time9 = 0, average_time16 = 0;
    count4 = 0, count9 = 0, count16 = 0;
    free_list_sudoku(unsolved);
    unsolved = load_sudokus("client_test_files/unsolved_client.txt");

    for(int i = 0; i < unsolved.total; i++) {
        cost = 0;
        gettimeuseconds(&time_usec_init); // init time
        findSudokuBruteForce(unsolved.sudokus[i].board, 0, 0, unsolved.sudokus[i].size, &solved, &cost);
        gettimeuseconds(&time_usec_end); // end time
        elapsed_time = (long) (time_usec_end - time_usec_init);

        switch(unsolved.sudokus[i].size) {
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
    cost = average_cost4/count4;
    elapsed_time = average_time4/count4;
    printf("Sudokus 4x4:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);

    cost = average_cost9/count9;
    elapsed_time = average_time9/count9;
    printf("Sudokus 9x9:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);

    cost = average_cost16/count16;
    elapsed_time = average_time16/count16;
    printf("Sudokus 16x16:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);

    cost = average_cost25/count25;
    elapsed_time = average_time25/count25;
    printf("Sudokus 25x25:\nAverage time: usec %ld - sec %lf\nAverage cost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);
    free_list_sudoku(unsolved);
    free_list_sudoku(solved);
}