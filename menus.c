#include "menus.h"
#include "fileio.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include "algorithms.h"

void menu_choose_sudoku(ListSudoku list, ListSudoku *solved,int flagOrdered);

void menu_sudoku(Sudoku s, ListSudoku *solved);

void menu_gen_sudoku(ListSudoku unsolved, ListSudoku *solved);

/**
 * @brief Menu Principal
 */
void main_menu() {
    ListSudoku unsolved, solved;
    unsolved.total = 0;
    solved.total = 0;
    solved.sudokus = NULL;
    int selection, exit = 0;
    while (!exit) {
        printf("Menu Principal\n");
        printf("1- Ver tabuleiros Ordenados por Insercao\n"
               "2- Ver tabuleiros Ordenados por Tamanho\n"
               "3- Carregar tabuleiros do ficheiro\n"
               "4- Guardar tabuleiros no ficheiro\n"
               "5- Criar Ficheiro Binario com os Tabuleiros Resolvidos \n"
               "6- Gerar novos tabuleiros\n"

               "0- Sair\n");
        scanf("%d", &selection);
        switch (selection) {
            case 1:
                menu_choose_sudoku(unsolved, &solved,0);
                break;
            case 2:
                menu_choose_sudoku(unsolved, &solved,1);
                break;
            case 3:
                unsolved = load_sudokus("unsolved.txt");
                //solved = load_sudokus("solved.txt");
                break;
            case 4:
                save_sudokus(solved, "solved.txt");
                break;
            case 5:
                save2binary(solved,"binary_solved.bin");

                break;
            case 6:
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
}

/**
 * @brief Menu de escolha do sudoku
 * @param list
 * @param solved
 * @param flagOrdered
 */
void menu_choose_sudoku(ListSudoku list, ListSudoku *solved,int flagOrdered) {
    int selection, exit = 0;
    while (!exit) {
        if (list.total == 0) {
            printf("Nao tem tabuleiros carregados na memoria\n");
            break;
        }

        printAllStoredBoards(list,flagOrdered);
        printf("------------------------------------------------\n"
               "Escolha o tabuleiro [1 - %d] ou 0 para sair:\n", list.total);
        scanf("%d", &selection);
        if (selection > 0 && selection <= list.total) {
            menu_sudoku(list.sudokus[selection - 1], solved);
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
void menu_sudoku(Sudoku s, ListSudoku *solved) {
    int selection, exit = 0, searchResult;
    long long time_usec_init, time_usec_end, cost = 0;
    long elapsed_time;
    while (!exit) {
        printf("\nTabuleiro escolhido:\n");
        printSudoku(s.board, s.size);
        printf("1- Resolver usando bruteforce\n"
               "2- Resolver usando algoritmo optimizado\n"
               "3- Pesquisar em tabuleiros ja resolvidos\n"
               "4- Testar se o tabuleiro e consistente\n"
               "0- Sair\n");
        scanf("%d", &selection);
        switch (selection) {
            case 1:
                gettimeuseconds(&time_usec_init); // init time
                findSudokuBruteForce(s.board, 0, 0, s.size, solved, &cost);
                gettimeuseconds(&time_usec_end); // end time

                elapsed_time = (long) (time_usec_end - time_usec_init);
                printf("Bruteforce\nusec: %ld - sec: %lf\ncost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);
                cost = 0;
                break;
            case 2:
                gettimeuseconds(&time_usec_init); // init time
                findSudokuAdvanced(s, solved, &cost);
                gettimeuseconds(&time_usec_end); // end time

                elapsed_time = (long) (time_usec_end - time_usec_init);
                printf("Optimized\nusec: %ld - sec: %lf\ncost: %lld\n", elapsed_time,(elapsed_time*pow(10,-6)), cost);
                cost = 0;
                break;
            case 3:
                searchResult = searchSudokus(*solved, s);
                if (searchResult == -1) {
                    printf("Tabuleiro nao encontrado\n");

                } else {
                    printf("Solucao encontrada com sucesso:\n");
                    printSudoku(solved->sudokus[searchResult].board, s.size);
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
void menu_gen_sudoku(ListSudoku unsolved, ListSudoku *solved) {

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
        size = selection + 2;
        size *= size;
        switch (selection) {
            case 1:
            case 2:
            case 3:
            case 4:
                printf("Quantos numeros pretende gerar?\n");
                scanf("%d", &input);
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