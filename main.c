#include <stdio.h>
#include "algorithms.h"
#include "fileio.h"
#include "utils.h"
#include <time.h>


void menu_choose_sudoku(ListSudoku list, ListSudoku *solved);

void menu_sudoku(Sudoku s, ListSudoku *solved);

void menu_gen_sudoku(ListSudoku unsolved, ListSudoku *solved);


int main() {
    srand(time(NULL));
    ListSudoku unsolved, solved;
    unsolved.total = 0;
    solved.total = 0;
    solved.sudokus = NULL;
    int selection, exit = 0;
    while (!exit) {
        printf("Menu Principal\n");
        printf("1- Ver tabuleiros em memoria\n"
               "2- Carregar tabuleiros do ficheiro\n"
               "3- Guardar tabuleiros no ficheiro\n"
               "4- Criar Ficheiro Binario com os Tabuleiros Resolvidos \n"
               "5- Gerar novos tabuleiros\n"

               "0- Sair\n");
        scanf("%d", &selection);
        switch (selection) {
            case 1:
                menu_choose_sudoku(unsolved, &solved);
                break;
            case 2:
                unsolved = load_sudokus("unsolved.txt");
                solved = load_sudokus("solved.txt");
                break;
            case 3:
                save_sudokus(solved, "solved.txt");
                break;
            case 4:
                save2binary(solved,"binary_solved.bin");

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
    return 0;
}


void menu_choose_sudoku(ListSudoku list, ListSudoku *solved) {
    int selection, exit = 0;
    while (!exit) {
        if (list.total == 0) {
            printf("Nao tem tabuleiros carregados na memoria\n");
            break;
        }

        printAllStoredBoards(list);
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

void menu_sudoku(Sudoku s, ListSudoku *solved) {
    int selection, exit = 0, searchResult;
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
                findSudokuBruteForce(s.board, 0, 0, s.size, solved);
                break;
            case 2:
                findSudokuAdvanced(s, solved);
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
