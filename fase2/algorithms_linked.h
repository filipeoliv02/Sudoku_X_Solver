#ifndef SUDOKU_ALGORITHMS_LINKED_H
#define SUDOKU_ALGORITHMS_LINKED_H

/**
 * @brief Um nó representa uma célula, um candidato de uma célula ou suporte para estruturas de dados mais complexas.
 * @details Um nó contém 3 números inteiros que representam as coordenadas, também contém direções onde se podem encontrar
 * outros nós, cada par de direções opostas representa uma unidade, por exemplo sul e norte é a unidade das colunas,
 * num plano bi-dimensional apresenta os oito pontos cardeais assim como outros nós da mesma região, além disso também
 * apresenta nós para subir e descer entre camadas de candidatos e nós exclusivos às regras do sudoku.
 */
typedef struct node {
    int num, row, col;
    struct node *n, *ne, *e, *se, *s, *sw, *w, *nw, *ascend, *descend;
    struct node *fBox, *bBox; // Forwards/Backwards na região
    struct node *fRule, *bRule; // Forwards/Backwards na regra
} Node;

/**
 * @brief Um único elemento de uma fila de sudokus.
 * @details Contém um tabuleiro, o tamanho do tabuleiro e o próximo tabuleiro na fila.
 */
typedef struct sudokuQueueNode {
    int size;
    Node *first;
    struct sudokuQueueNode *next;
} SudokuQueueNode;

/***
 * @brief Fila de tabuleiros sudoku
 */
typedef struct sudokuQueue {
    int total;
    SudokuQueueNode *first, *last;
} SudokuQueue;

void solveLinkedSudokuBruteForce(SudokuQueue *sudokuSolvedQueue, SudokuQueueNode *sudoku, Node *node);

void solveLinkedSudokuOptimized(SudokuQueue *sudokuSolvedQueue, SudokuQueueNode *sudoku);

int isValidPlacementLinked(Node *node, int num);

void disconnectNode(Node *node);

Node *nodeN(Node *node);

Node *nodeS(Node *node);

Node *nodeW(Node *node);

Node *nodeE(Node *node);

Node *nodeNE(Node *node);

Node *nodeNW(Node *node);

Node *nodeSE(Node *node);

Node *nodeSW(Node *node);

Node *nodeASCEND(Node *node);

Node *nodeDESCEND(Node *node);

Node *nodeFBOX(Node *node);

Node *nodeBBOX(Node *node);

#endif //SUDOKU_ALGORITHMS_LINKED_H
