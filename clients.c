#include "clients.h"
#include "fileio.h"
#include "utils.h"
#include "algorithms.h"
#include <stdio.h>

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
    for(int i = 0; i < sudoku9.total; i++) {
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
}