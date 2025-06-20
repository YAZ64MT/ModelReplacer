#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

// Linked List With unique elements

typedef struct LinkedListNode LinkedListNode;

LinkedListNode *LinkedListNode_getNext(LinkedListNode *n);

LinkedListNode *LinkedListNode_getPrev(LinkedListNode *n);

void *LinkedListNode_getData(LinkedListNode *n);

typedef struct LinkedList LinkedList;

LinkedList *LinkedList_newList();

void LinkedList_destroyList(LinkedList *list);

LinkedListNode *LinkedList_start(const LinkedList *list);

LinkedListNode *LinkedList_end(const LinkedList *list);

// If node with data already exists, it is moved to the beginning of the list
void LinkedList_addFront(LinkedList *list, void *data);

// If node with data already exists, it is moved to the end of the list
void LinkedList_addBack(LinkedList *list, void *data);

// Remove node from the beginning of the list
void LinkedList_removeFront(LinkedList *list);

// Remove node from end of list
void LinkedList_removeBack(LinkedList *list);

// second arg is a function condFunc taking two pointers and returns a bool
// third argument will be passed as the second argument to condFunc
// Each LinkedListNode's data will be passed as the first arg of condFunc
// if condFunc returns true, the LinkedListNode will be removed
void LinkedList_removeOnCondition(LinkedList *list, bool (*condFunc)(void *LinkedListNodeData, void *extraData), void *extraData);

bool LinkedList_removeData(LinkedList *list, void *data);

#endif