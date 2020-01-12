#include "utils_linked.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

void enqueueSudoku(SudokuQueue *sudokuQueue, SudokuQueueNode *sudoku) {
    if (sudokuQueue->total == 0) {
        sudokuQueue->first = sudoku;
    } else {
        sudokuQueue->last->next = sudoku;
    }
    sudokuQueue->last = sudoku;
    sudokuQueue->total++;
}

SudokuQueueNode *dequeueSudoku(SudokuQueue *sudokuQueue) {
    SudokuQueueNode *sudoku = sudokuQueue->first;
    if (sudokuQueue->total != 0) {
        if (sudokuQueue->total == 1) {
            sudokuQueue->first = NULL;
            sudokuQueue->last = NULL;
        } else {
            sudokuQueue->first = sudokuQueue->first->next;
        }
        sudokuQueue->total--;
    }
    return sudoku;
}


SudokuQueueNode *cloneSudoku(SudokuQueueNode *sudoku) {
    SudokuQueueNode *sudokuClone = calloc(1, sizeof(SudokuQueueNode));
    sudokuClone->size = sudoku->size;

    Node *node, *node_prevline = NULL, *node_line  = NULL, *node_prev;
    Node *nodeSudoku = sudoku->first, *nodeSudokuLine = sudoku->first;

    for (int i = 0; i < sudokuClone->size; i++) {
        node_prev = NULL;
        for (int j = 0; j < sudokuClone->size; j++) {
            // Criar nó e colocar valor do tabuleiro
            node = (Node *) calloc(1, sizeof(Node));
            node->num = nodeSudoku->num;
            node->row = i;
            node->col = j;
            // Se não existir um primeiro nó então é este
            if (sudokuClone->first == NULL) {
                sudokuClone->first = node;
            }

            // Se não existir um nó anterior cria-se
            if (node_prev == NULL) {
                node_prev = node;
            } else {
                // Existe nó anterior logo liga-se (Este <--> Oeste)
                node_prev->e = node;
                node->w = node_prev;
                node_prev = node_prev->e;
            }

            // Se existir nó da linha anterior liga-se (Norte <--> Sul)
            if (node_prevline != NULL) {
                node_prevline->s = node;
                node->n = node_prevline;
                node_prevline = node_prevline->e;
            }

            // Ligar se estiver na diagonal principal e não na primeira linha
            if (i == j && i != 0) {
                node->nw = node->w->n;
                node->nw->se = node;
            }

            // Ligar se estiver na diagonal secundária e não na primeira linha
            if (i == sudokuClone->size - j - 1 && i != 0) {
                node->ne = node->n->e;
                node->ne->sw = node;
            }
            //Ligar Regiões
            int root = (int) sqrt(sudokuClone->size);
            int rcol, rrow;
            rcol = j % root;
            rrow = i % root;
            Node *rnode = node;
            if (!(rcol == 0 && rrow == 0)) {
                if (rcol == 0) {
                    rnode = rnode->n;
                    while (rnode->col % root != (root - 1)) {
                        rnode = rnode->e;
                    }
                    node->bbox = rnode;
                    rnode->fbox = node;
                } else {
                    node->bbox = node->w;
                    node->w->fbox = node;
                }
            }

            nodeSudoku = nodeSudoku->e;
        }

        //Se não existe linha associar
        if (node_line == NULL) {
            node_line = sudokuClone->first;
        } else {
            node_line = node_line->s;
        }
        node_prevline = node_line;

        nodeSudokuLine = nodeSudokuLine->s;
        nodeSudoku = nodeSudokuLine;

    }

    return sudokuClone;
}

int isConsistentLinked(SudokuQueueNode *sudoku) {
    Node *node = sudoku->first, *nodeFirstCol = sudoku->first;

    while(nodeFirstCol != NULL) {
        while(node != NULL) {
            if(node->num != 0 && !isValidPlacementLinked(node, node->num)) {
                return 0;
            }
            node = node->e;
        }
        nodeFirstCol = nodeFirstCol->s;
        node = nodeFirstCol;
    }

    return 1;
}

SudokuQueueNode *createEmptySudokuLinked(int size) {
    SudokuQueueNode *sudoku = (SudokuQueueNode *) malloc(sizeof(SudokuQueueNode));
    Node *node, *node_line = NULL, *node_prevline = NULL, *node_prev, *rnode;

    int root = (int) sqrt(size), rcol, rrow;

    sudoku->size = size;
    sudoku->next = NULL;
    node_prevline = NULL;
    node_line = NULL;

    for (int i = 0; i < size; i++) {
        node_prev = NULL;
        for (int j = 0; j < size; j++) {
            // Criar nó e colocar valor do tabuleiro
            node = (Node *) calloc(1, sizeof(Node));
            node->num = 0;
            node->row = i;
            node->col = j;

            // Se não existir um primeiro nó então é este
            if (sudoku->first == NULL) {
                sudoku->first = node;
            }

            // Se não existir um nó anterior cria-se
            if (node_prev == NULL) {
                node_prev = node;
            } else {
                // Existe nó anterior logo liga-se (Este <--> Oeste)
                node_prev->e = node;
                node->w = node_prev;
                node_prev = node_prev->e;
            }

            // Se existir nó da linha anterior liga-se (Norte <--> Sul)
            if (node_prevline != NULL) {
                node_prevline->s = node;
                node->n = node_prevline;
                node_prevline = node_prevline->e;
            }

            // Ligar se estiver na diagonal principal e não na primeira linha
            if (i == j && i != 0) {
                node->nw = node->w->n;
                node->nw->se = node;
            }

            // Ligar se estiver na diagonal secundária e não na primeira linha
            if (i == size - j - 1 && i != 0) {
                node->ne = node->n->e;
                node->ne->sw = node;
            }
            //Ligar Regiões
            rcol = j % root;
            rrow = i % root;
            rnode = node;
            if (!(rcol == 0 && rrow == 0)) {
                if (rcol == 0) {
                    rnode = rnode->n;
                    while (rnode->col % root != (root - 1)) {
                        rnode = rnode->e;
                    }
                    node->bbox = rnode;
                    rnode->fbox = node;
                } else {
                    node->bbox = node->w;
                    node->w->fbox = node;
                }
            }
        }

        //Se não existe linha associar
        if (node_line == NULL) {
            node_line = sudoku->first;
        } else {
            node_line = node_line->s;
        }
        node_prevline = node_line;
    }

    return sudoku;
}

SudokuQueueNode *generateRandomSudokuLinked(int size, int n) {
    srand(time(NULL));

    SudokuQueueNode *sudoku = createEmptySudokuLinked(size);
    Node *node;
    int row, col, num;

    for (int i = 0; i < n; i++) {
        row = rand() % size;
        col = rand() % size;

        node = sudoku->first;

        for(int j = 0; j < row; j++) {
            node = node->s;
        }

        for(int j = 0; j < col; j++) {
            node = node->e;
        }

        num = (rand() % size) + 1;
        if (node->num == 0 && isValidPlacementLinked(node, num)) {
            node->num = num;
        } else {
            i--;
        }
    }
    return sudoku;
}

void freeSudokuQueue(SudokuQueue *sudokuQueue) {
    while(sudokuQueue->total != 0) freeSudoku(dequeueSudoku(sudokuQueue));
    free(sudokuQueue);
}

void freeSudoku(SudokuQueueNode *sudokuQueueNode) {
    Node *nodeCurr = sudokuQueueNode->first, *nodeFirstCol = sudokuQueueNode->first, *nodeNext;

    while(nodeFirstCol != NULL) {
        nodeFirstCol = nodeFirstCol->s;
        while(nodeCurr != NULL) {
            nodeNext = nodeCurr->e;
            free(nodeCurr);
            nodeCurr = nodeNext;
        }
        nodeCurr = nodeFirstCol;
    }

    free(sudokuQueueNode);
}

void freeCandidates(Node *candidatesOrigin) {
    freeCandidateUnit(candidatesOrigin, nodeE);
    freeCandidateUnit(candidatesOrigin, nodeS);
    freeCandidateUnit(candidatesOrigin, nodeSE);
    freeCandidateUnit(candidatesOrigin, nodeSW);
    freeCandidateUnit(candidatesOrigin, nodeFBOX);
    freeCandidateUnit(candidatesOrigin, nodeASCEND);

    free(candidatesOrigin);
}

void freeCandidateUnit(Node *candidatesOrigin, Node *(*nextNode)(Node *)) {
    Node *rule = nextNode(candidatesOrigin)->fRule, *ruleNext, *node, *nodeNext;
    while(rule != NULL) {
        node = nextNode(rule);
        while(node != NULL) {
            nodeNext = nextNode(node);
            free(node);
            node = nodeNext;
        }

        ruleNext = rule->fRule;
        free(rule);
        rule = ruleNext;
    }

    free(nextNode(candidatesOrigin));
}