#include "menus.h"
#include "fileio.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>

void menu_choose_sudoku(SudokuList list, SudokuList *solved, int flagOrdered);

void menu_sudoku(Sudoku s, SudokuList *solved);

void menu_gen_sudoku(SudokuList unsolved, SudokuList *solved);

void menu_choose_type(SudokuList *unsolved, SudokuList *solved);

void print_linked_board(SudokuLinkedNode board);

/**
 * @brief Menu Principal
 */
void main_menu() {
    SudokuList unsolved = {0, NULL, NULL}, solved = {0, NULL, NULL};
    int selection, exit = 0;
    while (!exit) {
        printf("Menu Principal\n");
        printf("1 - Ver tabuleiros\n"
               "2 - Carregar tabuleiros nao resolvidos do ficheiro\n"
               "3 - Guardar tabuleiros resolvidos no ficheiro\n"
               "4 - Criar Ficheiro Binario com os Tabuleiros Resolvidos \n"
               "5 - Gerar novos tabuleiros\n"
               "0 - Sair\n");
        scanf("%d", &selection);
        switch (selection) {
            case 1:
                menu_choose_type(&unsolved, &solved);
                break;
            case 2:
                unsolved = load_sudokus("unsolved.txt");
                break;
            case 3:
                save_sudokus(solved, "solved.txt");
                break;
            case 4:
                save_binary(solved, "binary_solved.bin");
                break;
            case 5:
                menu_gen_sudoku(unsolved, &solved);
                break;
            case 0:
                exit = 1;
                break;
            default:
                printf("Escolha uma opcao valida\n");
                break;
        }
    }
    free_list_sudoku(solved);
    free_list_sudoku(unsolved);
}

/**
 * Menu para escolher entre tabuleiro resolvido e não resolvido e também ordenação por tamanho ou por inserção
 * @param unsolved
 * @param solved
 */
void menu_choose_type(SudokuList *unsolved, SudokuList *solved) {
    int selection, selection_order = 0, exit = 0;
    while (!exit) {
        printf("\nTipo de tabuleiro:\n");
        printf("1 - Tabuleiros resolvidos\n"
               "2 - Tabuleiros nao resolvidos\n"
               "0 - Voltar\n");
        scanf("%d", &selection);
        if (selection == 1 || selection == 2) {
            printf("\nTipo de ordenacao:\n"
                   "1 - Ordenar por tamanho\n"
                   "0 - Ordenar por insercao\n");
            scanf("%d", &selection_order);
        }
        switch (selection) {
            case 1:
                if (solved->total == 0) {
                    printf("\nNao tem tabuleiros resolvidos na memoria\n");
                } else {
                    computeOrderBySize(solved);
                    printAllStoredBoards(*solved, selection_order);
                }
                break;
            case 2:
                computeOrderBySize(unsolved);
                menu_choose_sudoku(*unsolved, solved, selection_order);
                break;
            case 0:
                exit = 1;
                break;
            default:
                printf("Escolha uma opcao valida\n");
                break;
        }
    }
}

/**
 * @brief Menu de escolha do sudoku
 * @param list
 * @param solved
 * @param flagOrdered
 */
void menu_choose_sudoku(SudokuList list, SudokuList *solved, int flagOrdered) {
    int selection, exit = 0;
    while (!exit) {

        if (list.total == 0) {
            printf("Nao tem tabuleiros carregados na memoria\n");
            break;
        }
        printAllStoredBoards(list, flagOrdered);
        printf("------------------------------------------------\n"
               "Escolha o tabuleiro [1 - %d] ou 0 para sair:\n", list.total);
        scanf("%d", &selection);
        if (selection > 0 && selection <= list.total) {
            menu_sudoku(*(list.sudokus + selection - 1), solved);
        } else if (selection == 0) {
            exit = 1;
        } else {
            printf("Escolha uma opcao valida\n");
        }
    }
}

/**
 * @brief Menu de opções para um sudoku
 * @param s
 * @param solved
 */
void menu_sudoku(Sudoku s, SudokuList *solved) {
    int selection, exit = 0, searchResult;
    long long time_usec_init, time_usec_end, cost = 0;
    long elapsed_time;
    while (!exit) {
        printf("\nTabuleiro escolhido:\n");
        printSudoku(s.board, s.size);
        printf("1 - Resolver usando bruteforce\n"
               "2 - Resolver usando algoritmo optimizado\n"
               "3 - Pesquisar em tabuleiros ja resolvidos\n"
               "4 - Testar se o tabuleiro e consistente\n"
               "0 - Sair\n");
        scanf("%d", &selection);
        switch (selection) {
            case 1:
                if (!isConsistent(s)) {
                    printf("Tabuleiro inconsistente\n");
                    break;
                }
                gettimeuseconds(&time_usec_init); // init time
                solveSudokuBruteForce(solved, s, 0, 0, &cost);
                gettimeuseconds(&time_usec_end); // end time

                elapsed_time = (long) (time_usec_end - time_usec_init);
                printf("Bruteforce\nusec: %ld - sec: %lf\ncost: %lld\n", elapsed_time, (elapsed_time * pow(10, -6)),
                       cost);
                cost = 0;
                break;
            case 2:
                if (!isConsistent(s)) {
                    printf("Tabuleiro inconsistente\n");
                    break;
                }
                gettimeuseconds(&time_usec_init); // init time
                solveSudokuOptimized(s, solved, &cost);
                gettimeuseconds(&time_usec_end); // end time

                elapsed_time = (long) (time_usec_end - time_usec_init);
                printf("Optimized\nusec: %ld - sec: %lf\ncost: %lld\n", elapsed_time, (elapsed_time * pow(10, -6)),
                       cost);
                cost = 0;
                break;
            case 3:
                // Procurar em memória primeiro
                searchResult = searchSudokus(*solved, s);
                if (searchResult == -1) {
                    SudokuList file_solved = load_sudokus("solved.txt");

                    // Procurar no ficheiro em segundo
                    searchResult = searchSudokus(file_solved, s);
                    if (searchResult == -1) {
                        printf("Tabuleiro nao encontrado\n");

                    } else {
                        printf("Solucao encontrada com sucesso:\n");
                        printSudoku((file_solved.sudokus + searchResult)->board, s.size);
                    }
                } else {
                    printf("Solucao encontrada com sucesso:\n");
                    printSudoku((solved->sudokus + searchResult)->board, s.size);
                }
                break;
            case 4:

                if (isConsistent(s)) {
                    printf("Tabuleiro consistente\n");
                } else {
                    printf("Tabuleiro inconsistente\n");
                }
                break;
            case 0:
                exit = 1;
                break;
            default:
                printf("Escolha uma opcao valida\n");
                break;
        }
    }
}

/**
 * @brief Menu para Gerar tabuleiros novos
 * @param unsolved
 * @param solved
 */
void menu_gen_sudoku(SudokuList unsolved, SudokuList *solved) {

    Sudoku s;
    int selection, exit = 0, size = 0, input;
    while (!exit) {
        printf("Escolha uma opcao:\n"
               "1 - 9x9\n"
               "2 - 16x16\n"
               "3 - 25x25\n"
               "4 - 36x36\n"
               "0 - Sair\n");
        scanf("%d", &selection);
        switch (selection) {
            case 1:
            case 2:
            case 3:
            case 4:
                printf("Quantos numeros pretende gerar?\n");
                scanf("%d", &input);
                size = selection + 2;
                size *= size;
                s = gen_sudoku(size, input);
                menu_sudoku(s, solved);

                break;
            case 0:
                exit = 1;
                break;
            default:
                printf("Escolha uma opcao valida\n");
                break;
        }
    }
}