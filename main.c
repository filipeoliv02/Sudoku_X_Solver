#include "fase1/menus.h"
#include "fase1/clients.h"
#include "fase2/clients_linked.h"

/**
 * @brief Início do Programa usado para alternar entre menus e clientes
 * @details Função Inicial para chamar clientes ou o menu principal
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char const *argv[]) {
    // Fase 1
    //client_bruteforce();
    //client_advanced();
    //client_read();
    //client_show_stored();
    //client_write();
    //client_check_consistency();
    //client_random_generation();
    //client_solve_variable_size();
    //client_search_solutions();
    //client_compare_algorithms();

    // Fase 2
    client_linked_bruteforce();
    //client_linked_optimized();
    //client_linked_random_generation();
    //client_linked_solve_variable_size();
    //client_linked_read_write_txt();
    //client_linked_read_write_bin();
    //client_linked_check_consistency();
    //client_linked_random_generation();

    //main_menu();
    return 0;
}

