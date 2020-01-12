#include "algorithms_linked.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../fase1/utils.h"

void writePossibilities(SudokuLinkedNode sudoku);

int isValidPlacementLink(SudokuLinkedNode sudoku, Node *node, int num);

int checkNakedSingle(Node *node, int size);

int checkHiddenSingle(Node *node, int size);

Node *createPossibilities(int size);

int initPossibilities(Node *origin, Node *sudoku_first);

/**
 * @brief Resolve sudokus através do algoritmo bruteforce
 * @details Resolve sudokus através do algoritmo bruteforce usando listas ligadas
 * @param queue
 * @param node
 */
void solveSudokuBruteForceLink(SudokuLinkedNode queue, Node *node) {
    if (node == NULL) {
        printf("Solucao:\n");
        print_linked_board(queue);
        return;
    }

    Node *newNode = node;
    if (newNode->e == NULL) {
        while (newNode->w != NULL) newNode = newNode->w;
        newNode = newNode->s;
    } else {
        newNode = newNode->e;
    }

    if (node->num > 0) {
        solveSudokuBruteForceLink(queue, newNode);
    } else {
        for (int num = 1; num <= queue.size; num++) {
            if (isValidPlacementLink(queue, node, num)) {
                node->num = num;
                solveSudokuBruteForceLink(queue, newNode);
            }
        }
        node->num = 0;
    }
}

/**
 * @brief Verifica se o número passado pode ser colocado no tabuleiro
 * @details Verifica se o número passado pode ser colocado na (linha, coluna, região, diagonal) do tabuleiro usando listas ligadas
 * @param sudoku
 * @param node
 * @param num
 * @return
 */

int isValidPlacementLink(SudokuLinkedNode sudoku, Node *node, int num) {
    Node *nodeAux = NULL;
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->s;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->n;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->w;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->e;
    }
    //diagonais
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->nw;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->se;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->ne;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->sw;
    }
    //Regiões

    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->fbox;
    }
    nodeAux = node;
    while (nodeAux != NULL) {
        if (nodeAux->num == num) {
            return 0;
        }
        nodeAux = nodeAux->bbox;
    }
    return 1;
}

/**
 * @brief Resolve sudokus através do algoritmo optimizado
 * @details Resolve sudokus através do algoritmo optimizado usando listas ligadas
 * @param sudoku
 */

void solveSudokuOptimizedLink(SudokuLinkedNode sudoku) {
    Node *origin = createPossibilities(sudoku.size);
    int initialCount = initPossibilities(origin, sudoku.first);
    int count = initialCount;

    while (stratSingles(origin, sudoku.first, &count) || stratIntersectionRemoval(origin)
           || stratNakedGroups(origin) || stratXWing(origin));

    printf("Tabuleiro com %d celulas iniciais\n", initialCount);
    printf("Otimizado (linked) encontrou: %d\n", count-initialCount);
    printf("Total final: %d", count);

    if(count == sudoku.size * sudoku.size) {
        printf("\n\nSolucao:\n");
        print_linked_board(sudoku);
    }
    else {
        printf(" (continuar com Bruteforce)\n");
        solveSudokuBruteForceLink(sudoku, sudoku.first);
    }
}

/**
 * @brief Encontra os Hidden Singles e os Naked Singles
 * @details Encontra os Hidden Singles (Colunas, linhas, Diagonais e regiões) e os Naked Singles. https://www.sudokuwiki.org/Getting_Started
 * @param origin
 * @param first
 * @param count
 * @return
 */

int stratSingles(Node *origin, Node *first, int *count) {
    if(checkSingles(origin, first, nodeE)           // Hidden Single na linha
       || checkSingles(origin, first, nodeS)    // Hidden Single na coluna
       || checkSingles(origin, first, nodeFBOX) // Hidden Single na região
       || checkSingles(origin, first, nodeSE)   // Hidden Single na diagonal principal
       || checkSingles(origin, first, nodeSW)   // Hidden Single na diagonal secundária
       || checkSingles(origin, first, nodeUP)) {// Naked Single
        (*count)++;
        return 1;
    }
    return 0;
}

/**
 * @brief Estratégia de Intersection Removal
 * @details Resolve as sub estratégias de intersection removal: Box/Line Reduction e pointing groups nas linhas, colunas, regiões e diagonais https://www.sudokuwiki.org/Intersection_Removal
 * @param origin
 * @return
 */

int stratIntersectionRemoval(Node *origin) {
    return checkIntersectionRemoval(origin, nodeS, nodeFBOX, nodeBBOX, isSameBox)     // Box/Line reduct. na coluna
           || checkIntersectionRemoval(origin, nodeE, nodeFBOX, nodeBBOX, isSameBox)  // Box/Line reduct. na linha
           || checkIntersectionRemoval(origin, nodeSE, nodeFBOX, nodeBBOX, isSameBox) // Box/Line reduct. na diag. p.
           || checkIntersectionRemoval(origin, nodeSW, nodeFBOX, nodeBBOX, isSameBox) // Box/Line reduct. na diag. s.
           || checkIntersectionRemoval(origin, nodeFBOX, nodeS, nodeN, isSameCol)     // Pointing pairs na coluna
           || checkIntersectionRemoval(origin, nodeFBOX, nodeE, nodeW, isSameRow)     // Pointing pairs na linha
           || checkIntersectionRemoval(origin, nodeFBOX, nodeSE, nodeNW, isSamePDiag) // Pointing pairs na diag. p.
           || checkIntersectionRemoval(origin, nodeFBOX, nodeSW, nodeNE, isSameSDiag);// Pointing pairs na diag. s.
}

#define FLAG_DYNAMIC_GROUPS 0

/**
 * @brief Encontra os naked groups
 * @details Encontra os naked groups nas linhas, colunas, diagonais e regiões https://www.sudokuwiki.org/Naked_Candidates
 * @param origin
 * @return
 */

int stratNakedGroups(Node *origin) {
    int maxGroups = FLAG_DYNAMIC_GROUPS ? origin->num / 2 : 4;
    for (int groupSize = 2; groupSize <= maxGroups; groupSize++) {
        if (checkNakedGroups(origin, groupSize, nodeE, nodeW)
            || checkNakedGroups(origin, groupSize, nodeS, nodeN)
            || checkNakedGroups(origin, groupSize, nodeSE, nodeNW)
            || checkNakedGroups(origin, groupSize, nodeSW, nodeNE)
            || checkNakedGroups(origin, groupSize, nodeFBOX, nodeBBOX)) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Estratégia X-Wing
 * @param origin
 * @return
 */

int stratXWing(Node *origin) {
    return checkXWing(origin, nodeE, nodeW, nodeS, nodeN, unitCol) || checkXWing(origin, nodeS, nodeN, nodeE, nodeW, unitRow);
}

/**
 * @brief Verifica a existência de singles
 * @param origin
 * @param first
 * @param nextNode
 * @return
 */

int checkSingles(Node *origin, Node *first, Node *(*nextNode)(Node *)) {
    Node *rule = nextNode(origin)->fRule, *node;
    while (rule != NULL) {
        node = nextNode(rule);
        if (node != NULL && nextNode(node) == NULL) {
            //printf("Single: %d (%d, %d)\n", node->num, node->row, node->col);
            addNumber(first, node);
            removeConnectedNodes(node);
            free(node);
            return 1;
        }
        rule = rule->fRule;
    }
    return 0;
}

/**
 * @brief Verifica a existência da possibilidade de ocorrer intersection removal
 * @param origin
 * @param nextNode
 * @param nextRemoveNode
 * @param prevRemoveNode
 * @param isSameUnit
 * @return
 */

int checkIntersectionRemoval(Node *origin, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                             Node *(*prevRemoveNode)(Node *), int (*isSameUnit)(Node *, Node *, int)) {
    Node *rule = nextNode(origin)->fRule, *node;
    int found;
    while (rule != NULL) {
        found = 0;

        // Procurar interseção
        node = nextNode(rule);
        while (nextNode(node) != NULL) {
            found = 1;
            if (!isSameUnit(node, nextNode(node), origin->num)) {
                found = 0;
                break;
            }
            node = nextNode(node);
        }

        // Se interseção encontrada, remover os outros nós
        if (found && clearIntersection(rule, nextNode, nextRemoveNode, prevRemoveNode)) {
            //printf("Intersect Rmv: %d (%d, %d)\n", nextNode(rule)->num, nextNode(rule)->row, nextNode(rule)->col);
            return 1;
        }

        rule = rule->fRule;
    }
    return 0;
}

/**
 * @brief procura por naked groups
 * @param origin
 * @param groupSize
 * @param nextNode
 * @param prevNode
 * @return
 */

int checkNakedGroups(Node *origin, int groupSize, Node *(*nextNode)(Node *), Node *(*prevNode)(Node *)) {
    Node *rule = origin->up->fRule, *node;
    int *dict = (int *) calloc(origin->num + 1, sizeof(int)), count = 0;

    while (rule != NULL) {
        for (int i = 0; i <= origin->num; i++) {
            *(dict + i) = 0;
        }
        node = rule->up;
        while (node != NULL) {
            (*(dict + node->num))++;
            (*dict)++;
            node = node->up;
        }

        if (findGroup(rule, dict, groupSize, groupSize - 1, &count, nextNode)) {
            count = removeGroupNodes(rule, NULL, dict, prevNode) + count;

            if (count > 0) {
                //printf("<-------------------> Group %d, %d: (%d,%d)\n", rule->up->num, rule->up->up->num, rule->up->row, rule->up->col);
                return 1;
            }

        }
        rule = rule->fRule;
    }
    return 0;
}

/**
 * @brief Verifica a existência de um X-Wing
 * @details https://www.sudokuwiki.org/X_Wing_Strategy
 * @param origin
 * @param nextNode
 * @param prevNode
 * @param nextRemoveNode
 * @param prevRemoveNode
 * @param unitNode
 * @return 1-Encontrou e removeu candidatos / 0-Ou nao encontrou ou nao removeu
 */

int checkXWing(Node *origin, Node *(*nextNode)(Node *), Node *(*prevNode)(Node *), Node *(*nextRemoveNode)(Node *),
               Node *(*prevRemoveNode)(Node *), int (*unitNode)(Node *)) {
    Node *rule = nextNode(origin)->fRule, *node;
    int count;

    while (rule != NULL) {
        count = 0;
        node = nextNode(rule);
        while (nextNode(node) != NULL) {
            if(isSameBox(node, nextNode(node), origin->num)) {
                count = 0;
                break;
            }
            count++;
            node = nextNode(node);
        }
        if (count == 1) {
            node = prevRemoveNode(nextNode(rule));
            while (node->num != 0) {
                if (prevNode(node)->num == 0 && unitNode(nextNode(node)) == unitNode(nextNode(nextNode(rule))) &&
                    nextNode(nextNode(node)) == NULL) {
                    if (removeXWing(nextNode(rule), node, nextNode, nextRemoveNode, prevRemoveNode)) {
                        printf("<-------------------------> XWing %d (%d, %d)\n", node->num, node->row, node->col);
                        return 1;
                    }
                }
                node = prevRemoveNode(node);
            }

            node = nextRemoveNode(nextNode(rule));
            while (node != NULL) {
                if (prevNode(node)->num == 0 && unitNode(nextNode(node)) == unitNode(nextNode(nextNode(rule))) &&
                    nextNode(nextNode(node)) == NULL) {
                    if (removeXWing(node, nextNode(rule), nextNode, nextRemoveNode, prevRemoveNode)) {
                        printf("<-------------------------> XWing %d (%d, %d)\n", node->num, node->row, node->col);
                        return 1;
                    }
                }
                node = nextRemoveNode(node);

            }

        }
        rule = rule->fRule;
    }
    return 0;
}

/**
 * @brief Remove candidatos descobertos usando a tecnica de x-wing
 * @param node1
 * @param node2
 * @param nextNode
 * @param nextRemoveNode
 * @param prevRemoveNode
 * @return
 */

int removeXWing(Node *node1, Node *node2, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                Node *(*prevRemoveNode)(Node *)) {
    Node *remove = NULL;
    int count = 0;

    while (node2 != NULL) {

        remove = prevRemoveNode(node2);
        while (remove->num != 0) {
            remove = prevRemoveNode(remove);
        }

        count = removeNodesBetweenTwoNodes(remove, node2, nextRemoveNode) + count;
        count = removeNodesBetweenTwoNodes(node2, node1, nextRemoveNode) + count;
        count = removeNodesBetweenTwoNodes(node1, NULL, nextRemoveNode) + count;

        node1 = nextNode(node1);
        node2 = nextNode(node2);

    }
    return count;
}

/**
 * @brief procura por naked groups
 * @param rule
 * @param dict
 * @param total
 * @param num
 * @param count
 * @param nextNode
 * @return
 */

int findGroup(Node *rule, int *dict, int total, int num, int *count, Node *(*nextNode)(Node *)) {
    if (*dict > total || num == 0) {
        if (*dict == total && num == 0) {
            *count = removeGroupNodes(rule, NULL, dict, nextNode) + (*count);
            return 1;
        } else {
            return 0;
        }
    }

    Node *node, *nextRule = nextNode(rule);

    while (nextRule != NULL) {
        node = nextRule->up;
        while (node != NULL) {
            if (*(dict + node->num) == 0) {
                (*dict)++;
            }
            (*(dict + node->num))++;
            node = node->up;
        }

        if (findGroup(nextRule, dict, total, num - 1, count, nextNode)) {
            *count = removeGroupNodes(rule, nextRule, dict, nextNode) + (*count);
            return 1;
        }

        node = nextRule->up;
        while (node != NULL) {
            (*(dict + node->num))--;
            if (*(dict + node->num) == 0) {
                (*dict)--;
            }
            node = node->up;
        }

        nextRule = nextNode(nextRule);
    }

    return 0;
}

/**
 * @brief Remove nós dos grupos já descobertos
 * @param ruleStart
 * @param ruleStop
 * @param dict
 * @param nextNode
 * @return
 */

int removeGroupNodes(Node *ruleStart, Node *ruleStop, const int *dict, Node *(*nextNode)(Node *)) {
    Node *rule = nextNode(ruleStart), *node, *nodeNext;
    int count = 0;

    while (rule != ruleStop) {
        node = rule->up;
        while (node != NULL) {
            nodeNext = node->up;
            if (*(dict + node->num) != 0) {
                count++;
                disconnectNode(node);
                free(node);
            }
            node = nodeNext;
        }

        rule = nextNode(rule);
    }

    return count;
}

/**
 * @brief Se um candidato for válido adiciona o ao tabuleiro
 * @param first
 * @param new
 */

void addNumber(Node *first, Node *new) {
    Node *node = first;
    for (int row = 0; row < new->row; row++) {
        node = node->s;
    }
    for (int col = 0; col < new->col; col++) {
        node = node->e;
    }

    node->num = new->num;
}

/**
 * @brief Remove as ligações com um nó
 * @param nodeRule
 * @param nextNode
 * @param nextRemoveNode
 * @param prevRemoveNode
 * @return
 */

int clearIntersection(Node *nodeRule, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                      Node *(*prevRemoveNode)(Node *)) {
    int count = 0;
    Node *node, *nodeAux;

    // Começar na unit onde se vai remover
    node = nextNode(nodeRule);
    while (prevRemoveNode(node) != NULL) node = prevRemoveNode(node);

    // Remover todos os nós entre dois nós
    nodeAux = nextNode(nodeRule);
    while (node != NULL) {
        count = removeNodesBetweenTwoNodes(node, nodeAux, nextRemoveNode) + count;
        node = nodeAux;
        if (nodeAux != NULL) {
            nodeAux = nextNode(nodeAux);
        }
    }

    return count;
}

/**
 * @brief cria os candidatos
 * @param size
 * @return
 */

Node *createPossibilities(int size) {
    Node *node, *node_line = NULL, *node_prevline = NULL, *node_prev = NULL, *node_layer = NULL, *nodeAux = NULL;
    Node *node_sudoku, *node_sudoku_line;
    int root = (int) sqrt(size), rrow, rcol;

    Node *origin = (Node *) calloc(1, sizeof(Node));
    Node *row = (Node *) calloc(1, sizeof(Node)), *col = (Node *) calloc(1, sizeof(Node));
    Node *box = (Node *) calloc(1, sizeof(Node)), *number = (Node *) calloc(1, sizeof(Node));
    Node *pdiag = (Node *) calloc(1, sizeof(Node)), *sdiag = (Node *) calloc(1, sizeof(Node));

    origin->num = size;
    origin->e = row;
    origin->s = col;
    origin->fbox = box;
    origin->se = pdiag;
    origin->sw = sdiag;
    origin->up = number;

    for (int num = 0; num <= size; num++) {

        node_prevline = NULL;
        node_line = NULL;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                node = (Node *) calloc(1, sizeof(Node));

                node->num = num;
                node->row = i;
                node->col = j;

                // Se não existir um primeiro nó então é este
                if (i == 0 && j == 0 && num == 0) {
                    node_layer = node;
                }

                // Ligar linha
                if (j != 0) {
                    // Não está na primeira coluna, ligar ao nó anterior
                    node_prev->e = node;
                    node->w = node_prev;
                    node_prev = node_prev->e;
                } else {
                    // Está na primeira coluna, não existe um nó anterior, associa-se
                    node_prev = node;

                    // Ligar à unidade das linhas
                    if (num > 0) {
                        row->fRule = (Node *) calloc(1, sizeof(Node));
                        row->fRule->bRule = row;
                        row = row->fRule;

                        row->e = node;
                        node->w = row;
                    }
                }

                // Ligar coluna
                if (i != 0) {
                    // Não está na primeira linha, ligar ao nó anterior
                    node_prevline->s = node;
                    node->n = node_prevline;
                    node_prevline = node_prevline->e;
                } else if (num > 0) {
                    // Está na primeira linha, ligar à unidade das colunas
                    col->fRule = (Node *) calloc(1, sizeof(Node));
                    col->fRule->bRule = col;
                    col = col->fRule;

                    col->s = node;
                    node->n = col;
                }

                // Ligar entre camadas, se estiver acima da primeira
                if (num > 0) {
                    if (i == 0 && j == 0) {
                        node->down = node_layer;
                        node_layer = node;
                    } else if (j == 0) {
                        node->down = node->n->down->s;
                    } else {
                        node->down = node->w->down->e;
                    }
                    node->down->up = node;
                }

                // Ligar a regra dos números únicos se estiver na primeira camada
                if (num == 0) {
                    number->fRule = node;
                    number->fRule->bRule = number;
                    number = number->fRule;
                }

                // Ligar diagonais se o tabuleiro for do tamanho do SudokuX
                if (size <= 16) {

                    // Diagonal principal
                    if (i == j) {
                        if (i != 0) {
                            // Não está na primeira linha, ligar ao nó anterior
                            node->nw = node->w->n;
                            node->nw->se = node;
                        } else if (num > 0) {
                            // Está na primeira linha, ligar à unidade das diagonais principais
                            pdiag->fRule = (Node *) calloc(1, sizeof(Node));
                            pdiag->fRule->bRule = pdiag;
                            pdiag = pdiag->fRule;

                            pdiag->se = node;
                            node->nw = pdiag;
                        }
                    }

                    // Diagonal secundária
                    if (i == size - j - 1) {
                        if (i != 0) {
                            // Não está na primeira linha, ligar ao nó anterior
                            node->ne = node->n->e;
                            node->ne->sw = node;
                        } else if (num > 0) {
                            // Está na primeira linha, ligar à unidade das diagonais secundárias
                            sdiag->fRule = (Node *) calloc(1, sizeof(Node));
                            sdiag->fRule->bRule = sdiag;
                            sdiag = sdiag->fRule;

                            sdiag->sw = node;
                            node->ne = sdiag;
                        }
                    }
                }

                //Ligar Regiões
                rcol = j % root;
                rrow = i % root;
                nodeAux = node;
                if (rcol == 0 && rrow == 0) {
                    // Está no ínicio da região, ligar à unidade das regiões
                    if (num > 0) {
                        box->fRule = (Node *) calloc(1, sizeof(Node));
                        box->fRule->bRule = box;
                        box = box->fRule;

                        box->fbox = node;
                        node->bbox = box;
                    }
                } else {
                    // Não está no ínicio da região, ligar ao nó anterior da região
                    if (rcol == 0) {
                        nodeAux = nodeAux->n;
                        while (nodeAux->col % root != (root - 1)) {
                            nodeAux = nodeAux->e;
                        }
                        node->bbox = nodeAux;
                        nodeAux->fbox = node;
                    } else {
                        node->bbox = node->w;
                        node->w->fbox = node;
                    }
                }
            }

            //Se não existe linha associar
            if (i == 0) {
                node_line = node_layer;
            } else {
                node_line = node_line->s;
            }
            node_prevline = node_line;
        }
    }

    return origin;
}

/**
 * @brief inicializa o tabuleiros removendo os candidatos não viáveis
 * @param origin
 * @param sudoku_first
 * @return
 */

int initPossibilities(Node *origin, Node *sudoku_first) {
    int count = 0;
    Node *nodeSudoku = sudoku_first;
    Node *nodeSudokuLine = sudoku_first;
    Node *node = origin->up->fRule, *nodeRemove;

    while (nodeSudokuLine != NULL) {
        while (nodeSudoku != NULL) {
            if (nodeSudoku->num != 0) {
                count++;
                nodeRemove = node;
                node = node->bRule;
                while (nodeRemove->num != nodeSudoku->num) {
                    nodeRemove = nodeRemove->up;
                }
                removeConnectedNodes(nodeRemove);
                free(nodeRemove);
            }
            nodeSudoku = nodeSudoku->e;
            node = node->fRule;
        }
        nodeSudokuLine = nodeSudokuLine->s;
        nodeSudoku = nodeSudokuLine;
    }

    return count;
}

/**
 * @brief remove conexões entre nós
 * @param node
 */

void removeConnectedNodes(Node *node) {
    removeNodesBetweenTwoNodes(node, NULL, nodeN);
    removeNodesBetweenTwoNodes(node, NULL, nodeS);
    removeNodesBetweenTwoNodes(node, NULL, nodeW);
    removeNodesBetweenTwoNodes(node, NULL, nodeE);
    removeNodesBetweenTwoNodes(node, NULL, nodeNE);
    removeNodesBetweenTwoNodes(node, NULL, nodeNW);
    removeNodesBetweenTwoNodes(node, NULL, nodeSE);
    removeNodesBetweenTwoNodes(node, NULL, nodeSW);
    removeNodesBetweenTwoNodes(node, NULL, nodeUP);
    removeNodesBetweenTwoNodes(node, NULL, nodeDOWN);
    removeNodesBetweenTwoNodes(node, NULL, nodeFBOX);
    removeNodesBetweenTwoNodes(node, NULL, nodeBBOX);
}

/**
 * @brief remove as conexões entre um nó no meio de dois
 * @param nodeStart
 * @param nodeStop
 * @param nextNode
 * @return
 */

int removeNodesBetweenTwoNodes(Node *nodeStart, Node *nodeStop, Node *(*nextNode)(Node *)) {
    Node *nodeCurr = nextNode(nodeStart), *nodeNext;
    int count = 0;

    while (nodeCurr != nodeStop) {
        count++;
        nodeNext = nextNode(nodeCurr);
        disconnectNode(nodeCurr);
        free(nodeCurr);
        nodeCurr = nodeNext;
    }

    return count;
}

/**
 * @brief disconecta um nó
 * @param node
 */

void disconnectNode(Node *node) {
    if (node->n != NULL) {
        node->n->s = node->s;
    }

    if (node->s != NULL) {
        node->s->n = node->n;
    }

    if (node->w != NULL) {
        node->w->e = node->e;
    }

    if (node->e != NULL) {
        node->e->w = node->w;
    }

    if (node->nw != NULL) {
        node->nw->se = node->se;
    }

    if (node->ne != NULL) {
        node->ne->sw = node->sw;
    }

    if (node->se != NULL) {
        node->se->nw = node->nw;
    }

    if (node->sw != NULL) {
        node->sw->ne = node->ne;
    }

    if (node->bbox != NULL) {
        node->bbox->fbox = node->fbox;
    }

    if (node->fbox != NULL) {
        node->fbox->bbox = node->bbox;
    }

    if (node->down != NULL) {
        node->down->up = node->up;
    }

    if (node->up != NULL) {
        node->up->down = node->down;
    }

    if (node->bRule != NULL) {
        node->bRule->fRule = node->fRule;
    }

    if (node->fRule != NULL) {
        node->fRule->bRule = node->bRule;
    }
}

/**
 * @brief Verifica se dois nós estão na mesma linha
 * @param node1
 * @param node2
 * @param size
 * @return
 */

int isSameRow(Node *node1, Node *node2, int size) {
    return node1->row == node2->row;
}

/**
 * @brief Verifica se dois nós estão na mesma coluna
 * @param node1
 * @param node2
 * @param size
 * @return
 */

int isSameCol(Node *node1, Node *node2, int size) {
    return node1->col == node2->col;
}

/**
 * @brief Verifica se dois nós estão na diagonal principal
 * @param node1
 * @param node2
 * @param size
 * @return
 */

int isSamePDiag(Node *node1, Node *node2, int size) {
    return node1->col == node1->row && node2->col == node2->row;
}

/**
 * @brief Verifica se dois nós estão na diagonal secundária
 * @param node1
 * @param node2
 * @param size
 * @return
 */

int isSameSDiag(Node *node1, Node *node2, int size) {
    return node1->row == size - node1->col - 1 && node2->row == size - node2->col - 1;
}

/**
 * @brief Verifica se dois nós estão na mesma região
 * @param node1
 * @param node2
 * @param size
 * @return
 */

int isSameBox(Node *node1, Node *node2, int size) {
    int root = sqrt(size);
    return node1->row / root == node2->row / root && node1->col / root == node2->col / root;
}

/**
 * @brief devolve a linha do nó passado
 * @param node
 * @return
 */

int unitRow(Node *node) {
    return node->row;
}

/**
 * @brief devolve a coluna do nó passado
 * @param node
 * @return
 */

int unitCol(Node *node) {
    return node->col;
}

/**
 * @brief avançar para norte
 * @param node
 * @return
 */

Node *nodeN(Node *node) {
    return node->n;
}

/**
 * @brief avançar para sul
 * @param node
 * @return
 */

Node *nodeS(Node *node) {
    return node->s;
}

/**
 * @brief avançar para oeste
 * @param node
 * @return
 */

Node *nodeW(Node *node) {
    return node->w;
}

/**
 * @brief avançar para este
 * @param node
 * @return
 */

Node *nodeE(Node *node) {
    return node->e;
}

/**
 * @brief avançar para nordeste
 * @param node
 * @return
 */

Node *nodeNE(Node *node) {
    return node->ne;
}

/**
 * @brief avançar para noroeste
 * @param node
 * @return
 */

Node *nodeNW(Node *node) {
    return node->nw;
}

/**
 * @brief avançar para sudeste
 * @param node
 * @return
 */

Node *nodeSE(Node *node) {
    return node->se;
}

/**
 * @brief avançar para sudoeste
 * @param node
 * @return
 */

Node *nodeSW(Node *node) {
    return node->sw;
}

/**
 * @brief ascende uma camada
 * @param node
 * @return
 */

Node *nodeUP(Node *node) {
    return node->up;
}

/**
 * @brief descende uma camada
 * @param node
 * @return
 */

Node *nodeDOWN(Node *node) {
    return node->down;
}

/**
 * @brief passa para a região à frente
 * @param node
 * @return
 */

Node *nodeFBOX(Node *node) {
    return node->fbox;
}

/**
 * @brief passa para a região atrás
 * @param node
 * @return
 */

Node *nodeBBOX(Node *node) {
    return node->bbox;
}

Node *nodeFRULE(Node *node) {
    return node->fRule;
}

Node *nodeBRULE(Node *node) {
    return node->bRule;
}
