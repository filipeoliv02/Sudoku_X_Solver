#include <stdio.h>
#include "algorithms.h"
#include "fileio.h"


int main() {
    Boards allBoards;

    int selection, exit = 0;
    int **boards = NULL;
    while (!exit) {
        printf("Menu Principal\n");
        printf("1- Ver tabuleiros em memoria\n"
               "2- Carregar tabuleiros do ficheiro\n"
               "3- Guardar tabuleiros no ficheiro\n"
               "0- Sair\n");
        scanf("%d", &selection);
        switch (selection) {
            case 1:

                break;


            case 2:
                load_boards(&allBoards);
                for (int k = 0; k < allBoards.num; k++) {
                    for (int i = 0; i < allBoards.sizes[k]; i++) {
                        printf("\n");
                        for (int j = 0; j < allBoards.sizes[k]; j++) {

                            printf("%d ", allBoards.board[k][i][j]);
                        }
                    }
                    printf("\n");
                }
                break;
            case 3:

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
