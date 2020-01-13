#include "algorithms_linked.h"
#include "utils_linked.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../fase1/utils.h"

/**
 * Esta flag determina se os naked groups (pairs, triples, quads, etc) estão limitados a um máximo de quads, quando
 * está a '0', ou se os grupos são dinamicos quando está a '1', no caso de sudoku 9x9 o maior grupo é um quad logo só
 * faz sentido estar dinâmico se for um sudoku maior.
 */
#define FLAG_DYNAMIC_GROUPS 0

int isValidPlacementInDirection(Node *node, int num, Node *(*nextNode)(Node *));

Node *createCandidates(int size);

int initCandidates(Node *origin, Node *sudoku_first);

int isPossible(Node *origin);

int isRuleConsistent(Node *origin, Node *(*nextNode)(Node *));

void addNumber(Node *first, Node *new);

int stratSingles(Node *origin, Node *first, int *count);

int stratIntersectionRemoval(Node *origin);

int stratNakedGroups(Node *origin);

int stratXWing(Node *origin);

int removeXWing(Node *node1, Node *node2, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                Node *(*prevRemoveNode)(Node *));

int checkSingles(Node *origin, Node *first, Node *(*nextNode)(Node *));

int checkIntersectionRemoval(Node *origin, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                             Node *(*prevRemoveNode)(Node *), int (*isSameUnit)(Node *, Node *, int));

int clearIntersection(Node *nodeRule, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                      Node *(*prevRemoveNode)(Node *));

int checkNakedGroups(Node *origin, int groupSize, Node *(*nextNode)(Node *), Node *(*prevNode)(Node *));

int checkXWing(Node *origin, Node *(*nextNode)(Node *), Node *(*prevNode)(Node *), Node *(*nextRemoveNode)(Node *),
               Node *(*prevRemoveNode)(Node *), int (*unitNode)(Node *));

int findGroup(Node *rule, int *dict, int total, int num, int *count, Node *(*nextNode)(Node *));

int removeGroupNodes(Node *ruleStart, Node *ruleStop, const int *dict, Node *(*nextNode)(Node *));

void removeConnectedNodes(Node *node);

int removeNodesBetweenTwoNodes(Node *nodeStart, Node *nodeStop, Node *(*nextNode)(Node *));

int isSameRow(Node *node1, Node *node2, int size);

int isSameCol(Node *node1, Node *node2, int size);

int isSamePDiag(Node *node1, Node *node2, int size);

int isSameSDiag(Node *node1, Node *node2, int size);

int isSameBox(Node *node1, Node *node2, int size);

int unitRow(Node *node);

int unitCol(Node *node);

/**
 * @brief Resolve sudokus através do algoritmo bruteforce
 * @details Resolve sudokus através do algoritmo bruteforce usando listas ligadas
 * @param queue
 * @param node
 */
void solveLinkedSudokuBruteForce(SudokuQueue *sudokuSolvedQueue, SudokuQueueNode *sudoku, Node *node) {
    if (node == NULL) {
        if(sudokuSolvedQueue != NULL) {
            SudokuQueueNode *sudokuClone = cloneSudoku(sudoku);
            enqueueSudoku(sudokuSolvedQueue, sudokuClone);
        }

        printf("Solucao usando Bruteforce:\n");
        printSudokuLinked(sudoku);
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
        solveLinkedSudokuBruteForce(sudokuSolvedQueue, sudoku, newNode);
    } else {
        for (int num = 1; num <= sudoku->size; num++) {
            if (isValidPlacementLinked(node, num)) {
                node->num = num;
                solveLinkedSudokuBruteForce(sudokuSolvedQueue, sudoku, newNode);
            }
        }
        node->num = 0;
    }
}

/**
 * @brief Verifica se o número passado pode ser colocado no tabuleiro
 * @details Verifica se o número passado pode ser colocado na (linha, coluna, região, diagonal(ais)) do tabuleiro
 * usando listas ligadas
 * @param sudoku
 * @param node
 * @param num
 * @return
 */
int isValidPlacementLinked(Node *node, int num) {
    return isValidPlacementInDirection(node, num, nodeE)
           && isValidPlacementInDirection(node, num, nodeW)
           && isValidPlacementInDirection(node, num, nodeS)
           && isValidPlacementInDirection(node, num, nodeN)
           && isValidPlacementInDirection(node, num, nodeSE)
           && isValidPlacementInDirection(node, num, nodeNW)
           && isValidPlacementInDirection(node, num, nodeSW)
           && isValidPlacementInDirection(node, num, nodeNE)
           && isValidPlacementInDirection(node, num, nodeFBOX)
           && isValidPlacementInDirection(node, num, nodeBBOX);
}

/**
 * @brief Verifica se numa certa direção o número passado não se encontra presente.
 * @param node
 * @param num
 * @param nextNode
 * @return
 */
int isValidPlacementInDirection(Node *node, int num, Node *(*nextNode)(Node *)) {
    node = nextNode(node);
    while (node != NULL) {
        if (node->num == num) {
            return 0;
        }
        node = nextNode(node);
    }
    return 1;
}

/**
 * @brief Resolve sudokus através do algoritmo optimizado usando listas ligadas
 * @param sudoku
 */
void solveLinkedSudokuOptimized(SudokuQueue *sudokuSolvedQueue, SudokuQueueNode *sudoku) {
    SudokuQueueNode *sudokuCopy = cloneSudoku(sudoku);
    Node *candidatesOrigin = createCandidates(sudokuCopy->size);
    int initialCount = initCandidates(candidatesOrigin, sudokuCopy->first);
    int count = initialCount;

    while (stratSingles(candidatesOrigin, sudokuCopy->first, &count)
           || stratIntersectionRemoval(candidatesOrigin)
           || stratNakedGroups(candidatesOrigin)
           || stratXWing(candidatesOrigin)) {
        if (!isPossible(candidatesOrigin)) {
            count = -1;
            break;
        }
    }

    freeCandidates(candidatesOrigin);

    if (count == -1) {
        printf("\nSudoku impossivel de resolver!!!\n\n");
        printSudokuLinked(sudokuCopy);
        return;
    }

    printf("Tabuleiro com %d celulas iniciais\n", initialCount);
    printf("Otimizado (linked) encontrou: %d\n", count - initialCount);
    printf("Total final: %d", count);

    if (count == sudoku->size * sudoku->size) {
        printf("\n\nSolucao:\n");
        if(sudokuSolvedQueue != NULL) {
            enqueueSudoku(sudokuSolvedQueue, sudokuCopy);
        }
        printSudokuLinked(sudokuCopy);
    } else {
        printf(" (continuar com Bruteforce)\n");
        solveLinkedSudokuBruteForce(sudokuSolvedQueue, sudokuCopy, sudokuCopy->first);
    }
}

/**
 * @brief Cria a estrutura de dados dos candidatos de um tabuleiro sudoku
 * @details Os candidatos são os números que se podem colocar num tabuleiro de sudoku, os candidatos também não estão
 * acessíveis diretamente, mas a partir dos nós de suporte que representam várias regras do sudoku, cada um destes nós
 * é uma unidade de uma regra, por exemplo uma única coluna.
 * @param size
 * @return
 */
Node *createCandidates(int size) {
    Node *nodeCurr, *nodeFirstCol = NULL, *nodeTop = NULL, *nodeLeftCurr = NULL, *nodeLayerTopLeft = NULL, *nodeAux = NULL;
    int root = (int) sqrt(size), rrow, rcol;

    Node *origin = (Node *) calloc(1, sizeof(Node));
    Node *row = (Node *) calloc(1, sizeof(Node)), *col = (Node *) calloc(1, sizeof(Node));
    Node *box = (Node *) calloc(1, sizeof(Node)), *number = (Node *) calloc(1, sizeof(Node));
    Node *pdiag = (Node *) calloc(1, sizeof(Node)), *sdiag = (Node *) calloc(1, sizeof(Node));

    origin->num = size;
    origin->e = row;
    origin->s = col;
    origin->fBox = box;
    origin->se = pdiag;
    origin->sw = sdiag;
    origin->ascend = number;

    for (int num = 0; num <= size; num++) {

        nodeTop = NULL;
        nodeFirstCol = NULL;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                nodeCurr = (Node *) calloc(1, sizeof(Node));
                nodeCurr->num = num;
                nodeCurr->row = i;
                nodeCurr->col = j;

                // Ligar a regra dos números únicos se estiver na primeira camada
                if (num == 0) {
                    number->fRule = nodeCurr;
                    number->fRule->bRule = number;
                    number = number->fRule;
                }

                // Ligar linha
                if (j != 0) {
                    // Não está na primeira coluna, ligar ao nó da esquerda
                    nodeLeftCurr->e = nodeCurr;
                    nodeCurr->w = nodeLeftCurr;
                    nodeLeftCurr = nodeLeftCurr->e;
                } else {
                    // Está na primeira coluna, não existe um nó à esquerda, associa-se
                    nodeLeftCurr = nodeCurr;

                    // Ligar à unidade das linhas
                    if (num > 0) {
                        row->fRule = (Node *) calloc(1, sizeof(Node));
                        row->fRule->bRule = row;
                        row = row->fRule;

                        row->e = nodeCurr;
                        nodeCurr->w = row;
                    }
                }

                // Ligar coluna
                if (i != 0) {
                    // Não está na primeira linha, ligar ao nó de cima
                    nodeTop->s = nodeCurr;
                    nodeCurr->n = nodeTop;
                    nodeTop = nodeTop->e;
                } else if (num > 0) {
                    // Está na primeira linha, ligar à unidade das colunas
                    col->fRule = (Node *) calloc(1, sizeof(Node));
                    col->fRule->bRule = col;
                    col = col->fRule;

                    col->s = nodeCurr;
                    nodeCurr->n = col;
                }

                // Se não existir um primeiro nó então é este
                if (i == 0 && j == 0 && num == 0) {
                    nodeLayerTopLeft = nodeCurr;
                }

                // Ligar entre camadas, se estiver acima da primeira
                if (num > 0) {
                    if (i == 0 && j == 0) {
                        nodeCurr->descend = nodeLayerTopLeft;
                        nodeLayerTopLeft = nodeCurr;
                    } else if (j == 0) {
                        nodeCurr->descend = nodeCurr->n->descend->s;
                    } else {
                        nodeCurr->descend = nodeCurr->w->descend->e;
                    }
                    nodeCurr->descend->ascend = nodeCurr;
                }

                // Ligar diagonais se o tabuleiro for do tamanho do SudokuX
                if (size <= 16) {

                    // Diagonal principal
                    if (i == j) {
                        if (i != 0) {
                            // Não está na primeira linha, ligar ao nó anterior
                            nodeCurr->nw = nodeCurr->w->n;
                            nodeCurr->nw->se = nodeCurr;
                        } else if (num > 0) {
                            // Está na primeira linha, ligar à unidade das diagonais principais
                            pdiag->fRule = (Node *) calloc(1, sizeof(Node));
                            pdiag->fRule->bRule = pdiag;
                            pdiag = pdiag->fRule;

                            pdiag->se = nodeCurr;
                            nodeCurr->nw = pdiag;
                        }
                    }

                    // Diagonal secundária
                    if (i == size - j - 1) {
                        if (i != 0) {
                            // Não está na primeira linha, ligar ao nó anterior
                            nodeCurr->ne = nodeCurr->n->e;
                            nodeCurr->ne->sw = nodeCurr;
                        } else if (num > 0) {
                            // Está na primeira linha, ligar à unidade das diagonais secundárias
                            sdiag->fRule = (Node *) calloc(1, sizeof(Node));
                            sdiag->fRule->bRule = sdiag;
                            sdiag = sdiag->fRule;

                            sdiag->sw = nodeCurr;
                            nodeCurr->ne = sdiag;
                        }
                    }
                }

                //Ligar Regiões
                rcol = j % root;
                rrow = i % root;
                nodeAux = nodeCurr;
                if (rcol == 0 && rrow == 0) {
                    // Está no ínicio da região, ligar à unidade das regiões
                    if (num > 0) {
                        box->fRule = (Node *) calloc(1, sizeof(Node));
                        box->fRule->bRule = box;
                        box = box->fRule;

                        box->fBox = nodeCurr;
                        nodeCurr->bBox = box;
                    }
                } else {
                    // Não está no ínicio da região, ligar ao nó anterior da região
                    if (rcol == 0) {
                        nodeAux = nodeAux->n;
                        while (nodeAux->col % root != (root - 1)) {
                            nodeAux = nodeAux->e;
                        }
                        nodeCurr->bBox = nodeAux;
                        nodeAux->fBox = nodeCurr;
                    } else {
                        nodeCurr->bBox = nodeCurr->w;
                        nodeCurr->w->fBox = nodeCurr;
                    }
                }
            }

            // Se na primeira linha associar, senão mover para baixo
            nodeFirstCol = i == 0 ? nodeLayerTopLeft : nodeFirstCol->s;
            nodeTop = nodeFirstCol;
        }
    }

    return origin;
}

/**
 * @brief Inicializa os candidatos ao sudoku passado
 * @param origin
 * @param sudoku_first
 * @return
 */
int initCandidates(Node *origin, Node *sudoku_first) {
    int count = 0;
    Node *nodeSudoku = sudoku_first;
    Node *nodeSudokuLine = sudoku_first;
    Node *node = origin->ascend->fRule, *nodeRemove;

    while (nodeSudokuLine != NULL) {
        while (nodeSudoku != NULL) {
            if (nodeSudoku->num != 0) {
                count++;
                nodeRemove = node;
                node = node->bRule;
                while (nodeRemove->num != nodeSudoku->num) {
                    nodeRemove = nodeRemove->ascend;
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
 * @brief Verifica se o tabuleiro é possível de resolver
 * @details O tabuleiro não é possível quando pelo menos uma célula vazia não tem nenhum candidato
 * @param origin
 * @return
 */
int isPossible(Node *origin) {
    return isRuleConsistent(origin, nodeE)
           && isRuleConsistent(origin, nodeS)
           && isRuleConsistent(origin, nodeSE)
           && isRuleConsistent(origin, nodeSW)
           && isRuleConsistent(origin, nodeASCEND);
}

/**
 * @brief Verifica se o tabuleiro respeita as regras do sudoku
 * @details Se o nó de suporte de uma regra existir sem ter nenhum candidato então o tabuleiro é impossível
 * @param origin
 * @param nextNode - Avança na respetiva regra passada
 * @return
 */
int isRuleConsistent(Node *origin, Node *(*nextNode)(Node *)) {
    Node *rule = nextNode(origin)->fRule;
    while (rule != NULL) {
        if (nextNode(rule) == NULL) {
            return 0;
        }
        rule = rule->fRule;
    }
    return 1;
}

/**
 * @brief Se um candidato for válido adiciona-o ao tabuleiro
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
 * @brief Estratégia dos Hidden Singles e dos Naked Singles
 * @details Encontra os Hidden Singles nas Colunas, Linhas, Diagonais e Regiões e os Naked Singles.
 * https://www.sudokuwiki.org/Getting_Started
 * @param origin
 * @param first
 * @param count
 * @return
 */
int stratSingles(Node *origin, Node *first, int *count) {
    if (checkSingles(origin, first, nodeE)          // Hidden Single na linha
        || checkSingles(origin, first, nodeS)       // Hidden Single na coluna
        || checkSingles(origin, first, nodeFBOX)    // Hidden Single na região
        || checkSingles(origin, first, nodeSE)      // Hidden Single na diagonal principal
        || checkSingles(origin, first, nodeSW)      // Hidden Single na diagonal secundária
        || checkSingles(origin, first, nodeASCEND)) // Naked Single
    {
        (*count)++;
        return 1;
    }
    return 0;
}

/**
 * @brief Estratégia de Intersection Removal
 * @details Chama as sub estratégias de intersection removal: Box/Line Reduction e pointing groups nas linhas,
 * colunas, regiões e diagonais https://www.sudokuwiki.org/Intersection_Removal
 * @param origin
 * @return
 */
int stratIntersectionRemoval(Node *origin) {
    return checkIntersectionRemoval(origin, nodeS, nodeFBOX, nodeBBOX, isSameBox)     // Box/Line reduct. na coluna
           || checkIntersectionRemoval(origin, nodeE, nodeFBOX, nodeBBOX, isSameBox)  // Box/Line reduct. na linha
           || checkIntersectionRemoval(origin, nodeSE, nodeFBOX, nodeBBOX, isSameBox) // Box/Line reduct. na diag. p.
           || checkIntersectionRemoval(origin, nodeSW, nodeFBOX, nodeBBOX, isSameBox) // Box/Line reduct. na diag. s.
           || checkIntersectionRemoval(origin, nodeFBOX, nodeS, nodeN, isSameCol)     // Pointing groups na coluna
           || checkIntersectionRemoval(origin, nodeFBOX, nodeE, nodeW, isSameRow)     // Pointing groups na linha
           || checkIntersectionRemoval(origin, nodeFBOX, nodeSE, nodeNW, isSamePDiag) // Pointing groups na diag. p.
           || checkIntersectionRemoval(origin, nodeFBOX, nodeSW, nodeNE, isSameSDiag);// Pointing groups na diag. s.
}

/**
 * @brief Estratégia dos naked groups (pairs, triples, etc)
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
    return checkXWing(origin, nodeE, nodeW, nodeS, nodeN, unitCol) ||
           checkXWing(origin, nodeS, nodeN, nodeE, nodeW, unitRow);
}

/**
 * @brief Verifica a existência de singles numa regra
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
 * @brief Verifica a existência de uma intersection entre duas regras e se remove outros candidatos
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
 * @brief Verifica a existência de um grupo e se remove outros candidatos
 * @details Um grupo acontece quando numa unidade (coluna, linha, região ou diagonal) existe X números únicos em
 * X células.
 * @param origin
 * @param groupSize
 * @param nextNode
 * @param prevNode
 * @return
 */
int checkNakedGroups(Node *origin, int groupSize, Node *(*nextNode)(Node *), Node *(*prevNode)(Node *)) {
    Node *rule = origin->ascend->fRule, *node;
    // Lista dos números encontrados
    int *dict = (int *) calloc(origin->num + 1, sizeof(int)), count = 0;

    while (rule != NULL) {
        for (int i = 0; i <= origin->num; i++) {
            *(dict + i) = 0;
        }
        node = rule->ascend;
        while (node != NULL) {
            (*(dict + node->num))++;
            (*dict)++;
            node = node->ascend;
        }

        if (findGroup(rule, dict, groupSize, groupSize - 1, &count, nextNode)) {
            count = removeGroupNodes(rule, NULL, dict, prevNode) + count;

            if (count > 0) {
                //printf("<-------------------> Group %d, %d: (%d,%d)\n", rule->ascend->num, rule->ascend->ascend->num, rule->ascend->row, rule->ascend->col);
                return 1;
            }

        }
        rule = rule->fRule;
    }
    return 0;
}

/**
 * @brief Verifica a existência de um X-Wing e se remove outros candidatos
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

    while (rule != NULL) {
        node = nextNode(rule);
        if (nextNode(node) != NULL && nextNode(nextNode(node)) == NULL &&
            !isSameBox(node, nextNode(node), origin->num)) {

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
 * @brief Procura por naked groups recursivamente
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
        node = nextRule->ascend;
        while (node != NULL) {
            if (*(dict + node->num) == 0) {
                (*dict)++;
            }
            (*(dict + node->num))++;
            node = node->ascend;
        }

        if (findGroup(nextRule, dict, total, num - 1, count, nextNode)) {
            *count = removeGroupNodes(rule, nextRule, dict, nextNode) + (*count);
            return 1;
        }

        node = nextRule->ascend;
        while (node != NULL) {
            (*(dict + node->num))--;
            if (*(dict + node->num) == 0) {
                (*dict)--;
            }
            node = node->ascend;
        }

        nextRule = nextNode(nextRule);
    }

    return 0;
}

/**
 * @brief Remove candidatos excluidos por um naked grupo já descoberto
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
        node = rule->ascend;
        while (node != NULL) {
            nodeNext = node->ascend;
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
 * @brief Remove candidatos excluidos por uma interseção de duas unidades
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
 * @brief Remove todos os candidatos em todas as direções do nó passado (inclui os nós das regras)
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
    removeNodesBetweenTwoNodes(node, NULL, nodeASCEND);
    removeNodesBetweenTwoNodes(node, NULL, nodeDESCEND);
    removeNodesBetweenTwoNodes(node, NULL, nodeFBOX);
    removeNodesBetweenTwoNodes(node, NULL, nodeBBOX);
}

/**
 * @brief Remove todos os candidatos entre dois nós, na direção passada
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
 * @brief Disconecta um nó dos nós no seu redor imediato
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

    if (node->bBox != NULL) {
        node->bBox->fBox = node->fBox;
    }

    if (node->fBox != NULL) {
        node->fBox->bBox = node->bBox;
    }

    if (node->descend != NULL) {
        node->descend->ascend = node->ascend;
    }

    if (node->ascend != NULL) {
        node->ascend->descend = node->descend;
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
    int root = (int) sqrt(size);
    return node1->row / root == node2->row / root && node1->col / root == node2->col / root;
}

/**
 * @brief Devolve a linha do nó passado
 * @param node
 * @return
 */
int unitRow(Node *node) {
    return node->row;
}

/**
 * @brief Devolve a coluna do nó passado
 * @param node
 * @return
 */
int unitCol(Node *node) {
    return node->col;
}

/**
 * @brief Avançar para norte
 * @param node
 * @return
 */
Node *nodeN(Node *node) {
    return node->n;
}

/**
 * @brief Avançar para sul
 * @param node
 * @return
 */
Node *nodeS(Node *node) {
    return node->s;
}

/**
 * @brief Avançar para oeste
 * @param node
 * @return
 */
Node *nodeW(Node *node) {
    return node->w;
}

/**
 * @brief Avançar para este
 * @param node
 * @return
 */
Node *nodeE(Node *node) {
    return node->e;
}

/**
 * @brief Avançar para nordeste
 * @param node
 * @return
 */
Node *nodeNE(Node *node) {
    return node->ne;
}

/**
 * @brief Avançar para noroeste
 * @param node
 * @return
 */
Node *nodeNW(Node *node) {
    return node->nw;
}

/**
 * @brief Avançar para sudeste
 * @param node
 * @return
 */
Node *nodeSE(Node *node) {
    return node->se;
}

/**
 * @brief Avançar para sudoeste
 * @param node
 * @return
 */
Node *nodeSW(Node *node) {
    return node->sw;
}

/**
 * @brief Sobe uma camada
 * @param node
 * @return
 */
Node *nodeASCEND(Node *node) {
    return node->ascend;
}

/**
 * @brief Desce uma camada
 * @param node
 * @return
 */
Node *nodeDESCEND(Node *node) {
    return node->descend;
}

/**
 * @brief Passa para o próximo nó da mesma região
 * @param node
 * @return
 */
Node *nodeFBOX(Node *node) {
    return node->fBox;
}

/**
 * @brief Passa para o nó anterior da mesma região
 * @param node
 * @return
 */
Node *nodeBBOX(Node *node) {
    return node->bBox;
}