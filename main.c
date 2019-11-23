#include <stdio.h>
#include "algorithms.h"
#include "fileio.h"
#include "utils.h"

void menu_choose_sudoku(ListSudoku list);
void menu_sudoku(Sudoku s);

int main() {
    ListSudoku unsolved, solved;

    int selection, exit = 0;
    while (!exit) {
        printf("Menu Principal\n");
        printf("1- Ver tabuleiros em memoria\n"
               "2- Carregar tabuleiros do ficheiro\n"
               "3- Guardar tabuleiros no ficheiro\n"
               "0- Sair\n");
        scanf("%d", &selection);
        switch (selection) {
            case 1:
                menu_choose_sudoku(unsolved);
                break;
            case 2:
                unsolved = load_sudokus("unsolved.txt");
                break;
            case 3:
                //save_sudokus(solved, "solved.txt");
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


void menu_choose_sudoku(ListSudoku list) {
    int selection, exit = 0;
    while (!exit) {
        if(list.total == 0) {
           printf("Nao tem tabuleiros carregados na memoria\n");
           break;
        }

        printAllStoredBoards(list);
        printf("----------------------------------------------\n"
               "Escolha o tabuleiro [1 - %d] ou 0 para sair:\n", list.total);
        scanf("%d", &selection);
        if(selection > 0 && selection <= list.total) {
            menu_sudoku(list.sudokus[selection-1]);
        }
        else if(selection == 0) {
            exit = 1;
        }
        else {
            printf("Escolha uma opcao valida\n");
        }
    }
}

void menu_sudoku(Sudoku s) {

    int selection, exit = 0;
    while (!exit) {
        printSudoku(s.board, s.size);
        printf("1- Resolver usando bruteforce\n"
               "2- Resolver usando algoritmo optimizado\n"
               "0- Sair\n");
        scanf("%d", &selection);
        switch (selection) {
            case 1:
                findSudokuBruteForce(s.board, 0, 0, s.size);
                break;
            case 2:
                findSudokuAdvanced(s);
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