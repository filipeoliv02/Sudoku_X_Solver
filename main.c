#include <stdio.h>
#include "algorithms.h"
#include "fileio.h"
#include "utils.h"


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
                printAllStoredBoards(unsolved);
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

    /*printSudoku(sudoku);
    findSudokuBruteForce(sudoku, 0, 0, 9);
    printSudoku(sudokuAdvanced[0]);
    findSudokuAdvanced(sudokuAdvanced, 0, 0, 9);*/
    return 0;
}
