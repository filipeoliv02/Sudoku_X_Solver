#include "menus.h"
#include "clients.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    char client_bf[] = "client_bruteforce";
    char client_adv[] = "client_advanced";
    srand(time(NULL));
    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            if(strcmp(argv[i], client_bf) == 0) {
                client_bruteforce();
            }
            else if(strcmp(argv[i], client_adv) == 0) {
                client_advanced();
            }
        }
    }
    else {
        main_menu();
    }

    return 0;
}