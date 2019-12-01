#include "menus.h"
#include "clients.h"

/**
 * @brief Início do Programa usado para alternar entre menus e clientes
 * @details Função Inicial para chamar clientes ou o menu principal
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char const *argv[]) {
    client_bruteforce();
    client_advanced();
    client_read();
    client_show_stored();
    client_write();
    client_check_consistency();
    client_random_generation();
    client_solve_variable_size();
    client_search_solutions();
    client_compare_algorithms();
    //main_menu();
    return 0;
}

