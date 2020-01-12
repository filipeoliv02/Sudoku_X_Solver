#ifndef SUDOKU_ALGORITHMS_LINKED_H
#define SUDOKU_ALGORITHMS_LINKED_H

typedef struct node {
    int num, row, col;
    struct node *n, *ne, *e, *se, *s, *sw, *w, *nw, *ascend, *descend, *fbox, *bbox;
    struct node *fRule, *bRule;
} Node;

typedef struct sudokuQueueNode {
    int size;
    Node *first;
    struct sudokuQueueNode *next;
} SudokuQueueNode;

typedef struct sudoku_queue {
    int total;
    SudokuQueueNode *first, *last;
} SudokuQueue;

void solveLinkedSudokuBruteForce(SudokuQueue *sudokuSolvedQueue, SudokuQueueNode *sudoku, Node *node);

void solveLinkedSudokuOptimized(SudokuQueue *sudokuSolvedQueue, SudokuQueueNode *sudoku);

int isValidPlacementLinked(Node *node, int num);

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
