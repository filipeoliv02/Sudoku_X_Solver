#include "menus.h"
#include "fileio.h"
#include "utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void menu_choose_sudoku(SudokuList list, SudokuList *solved, int flagOrdered);

static void menu_sudoku(Sudoku s, SudokuList *solved);

static void menu_gen_sudoku(SudokuList *solved);

static void menu_choose_type(SudokuList *unsolved, SudokuList *solved);

/**
 * @brief Lê um inteiro do stdin de forma segura
 * @details Descarta o resto da linha. Em caso de entrada inválida devolve -1 (opção inválida); em caso de EOF devolve
 * 0, que em todos os menus significa sair/voltar, evitando ciclos infinitos quando o stdin termina.
 * @return
 */
static int read_int(void) {
    char line[64];
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }
    // Linha demasiado longa: descartar o resto
    if (strchr(line, '\n') == NULL) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
    char *end;
    long value = strtol(line, &end, 10);
    if (end == line || value < INT_MIN || value > INT_MAX) {
        return -1;
    }
    while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') {
        end++;
    }
    return *end == '\0' ? (int) value : -1;
}

/**
 * @brief Menu Principal
 */
void main_menu(void) {
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
        selection = read_int();
        switch (selection) {
            case 1:
                menu_choose_type(&unsolved, &solved);
                break;
            case 2:
                free_list_sudoku(unsolved);
                unsolved = load_sudokus("unsolved.txt");
                printf("%d tabuleiros carregados\n", unsolved.total);
                break;
            case 3:
                if (save_sudokus(solved, "solved.txt")) {
                    printf("Tabuleiros guardados em solved.txt\n");
                }
                break;
            case 4:
                if (save_binary(solved, "binary_solved.bin")) {
                    printf("Tabuleiros guardados em binary_solved.bin\n");
                }
                break;
            case 5:
                menu_gen_sudoku(&solved);
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
static void menu_choose_type(SudokuList *unsolved, SudokuList *solved) {
    int selection, selection_order = 0, exit = 0;
    while (!exit) {
        printf("\nTipo de tabuleiro:\n");
        printf("1 - Tabuleiros resolvidos\n"
               "2 - Tabuleiros nao resolvidos\n"
               "0 - Voltar\n");
        selection = read_int();
        if (selection == 1 || selection == 2) {
            printf("\nTipo de ordenacao:\n"
                   "1 - Ordenar por tamanho\n"
                   "0 - Ordenar por insercao\n");
            selection_order = read_int();
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
static void menu_choose_sudoku(SudokuList list, SudokuList *solved, int flagOrdered) {
    int selection, exit = 0;
    while (!exit) {

        if (list.total == 0) {
            printf("Nao tem tabuleiros carregados na memoria\n");
            break;
        }
        printAllStoredBoards(list, flagOrdered);
        printf("------------------------------------------------\n"
               "Escolha o tabuleiro [1 - %d] ou 0 para sair:\n", list.total);
        selection = read_int();
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
static void menu_sudoku(Sudoku s, SudokuList *solved) {
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
        selection = read_int();
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
                printf("Bruteforce\nusec: %ld - sec: %lf\ncost: %lld\n", elapsed_time, (elapsed_time / 1e6),
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
                printf("Optimized\nusec: %ld - sec: %lf\ncost: %lld\n", elapsed_time, (elapsed_time / 1e6),
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
                    if (searchResult < 0) {
                        printf("Tabuleiro nao encontrado\n");

                    } else {
                        printf("Solucao encontrada com sucesso:\n");
                        printSudoku((file_solved.sudokus + searchResult)->board, s.size);
                    }
                    free_list_sudoku(file_solved);
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
 * @param solved
 */
static void menu_gen_sudoku(SudokuList *solved) {

    Sudoku s;
    int selection, exit = 0, size = 0, input;
    while (!exit) {
        printf("Escolha uma opcao:\n"
               "1 - 9x9\n"
               "2 - 16x16\n"
               "3 - 25x25\n"
               "4 - 36x36\n"
               "0 - Sair\n");
        selection = read_int();
        switch (selection) {
            case 1:
            case 2:
            case 3:
            case 4:
                printf("Quantos numeros pretende gerar?\n");
                input = read_int();
                size = selection + 2;
                size *= size;
                if (input < 0) {
                    printf("Escolha um numero valido\n");
                    break;
                }
                s = gen_sudoku(size, input);
                menu_sudoku(s, solved); // as soluções são copiadas para 'solved'
                freeBoard(s.board, s.size);

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