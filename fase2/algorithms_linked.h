#ifndef SUDOKU_ALGORITHMS_LINKED_H
#define SUDOKU_ALGORITHMS_LINKED_H

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

void solveSudokuBruteForceLink(SudokuLinkedNode queue, Node *node);

void solveSudokuOptimizedLink(SudokuLinkedNode sudoku);

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

void disconnectNode(Node *node);

int isSameRow(Node *node1, Node *node2, int size);

int isSameCol(Node *node1, Node *node2, int size);

int isSamePDiag(Node *node1, Node *node2, int size);

int isSameSDiag(Node *node1, Node *node2, int size);

int isSameBox(Node *node1, Node *node2, int size);

int unitRow(Node *node);

int unitCol(Node *node);

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

#endif //SUDOKU_ALGORITHMS_LINKED_H
