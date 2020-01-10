#ifndef SUDOKU_ALGORITHMS_H
#define SUDOKU_ALGORITHMS_H

typedef struct node {
    int num, row, col;
    struct node *n, *ne, *e, *se, *s, *sw, *w, *nw, *up, *down, *fbox, *bbox;
    struct node *fRule, *bRule;
} Node;

typedef struct sudoku_linked_node {
    int size;
    Node *first;
    struct sudoku_linked_node *next;
} SudokuLinkedNode;

typedef struct sudoku_linked {
    int total;
    SudokuLinkedNode *first, *last;
} SudokuLinked;

typedef struct sudoku {
    int size;
    int **board;
} Sudoku;

typedef struct sudoku_list {
    int total;
    int *orderedList;
    Sudoku *sudokus;
} SudokuList;

void solveSudokuBruteForce(SudokuList *solved, Sudoku sudoku, int row, int col, long long *cost);

void solveSudokuOptimized(Sudoku unsolved, SudokuList *solved, long long *cost);

int isValidPlacement(Sudoku sudoku, int num, int row, int col);

int searchSudokus(SudokuList searchList, Sudoku sudoku);

int isPattern(Sudoku pattern, Sudoku unsolved);

void computeOrderBySize(SudokuList *a);

void solveSudokuBruteForceLink(SudokuLinkedNode queue, Node *node);

void solveSudokuOptimizedLink(SudokuLinkedNode sudoku);

void addNumber(Node *first, Node *new);

int stratSingles(Node *origin, Node *first);

int stratIntersectionRemoval(Node *origin);

int stratNakedGroups(Node *origin);

int checkSingles(Node *origin, Node *first, Node *(*nextNode)(Node *));

int checkIntersectionRemoval(Node *origin, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                          Node *(*prevRemoveNode)(Node *), int (*isSameUnit)(Node *, Node *, int));

int clearIntersection(Node *nodeRule, Node *(*nextNode)(Node *), Node *(*nextRemoveNode)(Node *),
                      Node *(*prevRemoveNode)(Node *));

int checkNakedGroups(Node *origin, Node *(*nextNode)(Node *), Node *(*prevNode)(Node *));

void removeConnectedNodes(Node *node);

int removeNodesBetweenTwoNodes(Node *nodeStart, Node *nodeStop, Node *(*nextNode)(Node *));

void disconnectNode(Node *node);

int isSameRow(Node *node1, Node *node2, int size);

int isSameCol(Node *node1, Node *node2, int size);

int isSamePDiag(Node *node1, Node *node2, int size);

int isSameSDiag(Node *node1, Node *node2, int size);

int isSameBox(Node *node1, Node *node2, int size);

Node *nodeN(Node *node);

Node *nodeS(Node *node);

Node *nodeW(Node *node);

Node *nodeE(Node *node);

Node *nodeNE(Node *node);

Node *nodeNW(Node *node);

Node *nodeSE(Node *node);

Node *nodeSW(Node *node);

Node *nodeUP(Node *node);

Node *nodeDOWN(Node *node);

Node *nodeFBOX(Node *node);

Node *nodeBBOX(Node *node);

Node *nodeFRULE(Node *node);

Node *nodeBRULE(Node *node);

#endif