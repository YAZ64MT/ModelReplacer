#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct Node {
    void *data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *start;
    Node *end;
} LinkedList;

void LinkedList_initList(LinkedList *list);

void LinkedList_destroyList(LinkedList *list);

Node *LinkedList_start(const LinkedList *list);

Node *LinkedList_end(const LinkedList *list);

void LinkedList_addFront(LinkedList *list, void *data);

void LinkedList_addBack(LinkedList *list, void *data);

void LinkedList_removeFront(LinkedList *list);

void LinkedList_removeBack(LinkedList *list);

void LinkedList_removeData(LinkedList *list, void *data);

// second arg is a function condFunc taking two pointers and returns a bool
// third argument will be passed as the second argument to condFunc
// Each node's data will be passed as the first arg of condFunc
// if condFunc returns true, the node will be removed
void LinkedList_removeOnCondition(LinkedList *list, bool (*condFunc)(void *nodeData, void *extraData), void *extraData);

void LinkedList_removeData(LinkedList *list, void *data);

#endif