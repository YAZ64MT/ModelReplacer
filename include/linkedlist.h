#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct LinkedListNode LinkedListNode;

LinkedListNode *LinkedListNode_getNext(LinkedListNode *n);

LinkedListNode *LinkedListNode_getPrev(LinkedListNode *n);

void *LinkedListNode_getData(LinkedListNode *n);

typedef struct LinkedList LinkedList;

void LinkedList_initList(LinkedList *list);

void LinkedList_destroyList(LinkedList *list);

LinkedListNode *LinkedList_start(const LinkedList *list);

LinkedListNode *LinkedList_end(const LinkedList *list);

void LinkedList_insertBefore(LinkedList *list, LinkedListNode *existingLinkedListNode, LinkedListNode *LinkedListNodeToInsert);

void LinkedList_insertAfter(LinkedList *list, LinkedListNode *existingLinkedListNode, LinkedListNode *LinkedListNodeToInsert);

void LinkedList_addFront(LinkedList *list, void *data);

void LinkedList_addBack(LinkedList *list, void *data);

void LinkedList_removeFront(LinkedList *list);

void LinkedList_removeBack(LinkedList *list);

void LinkedList_removeData(LinkedList *list, void *data);

// second arg is a function condFunc taking two pointers and returns a bool
// third argument will be passed as the second argument to condFunc
// Each LinkedListNode's data will be passed as the first arg of condFunc
// if condFunc returns true, the LinkedListNode will be removed
void LinkedList_removeOnCondition(LinkedList *list, bool (*condFunc)(void *LinkedListNodeData, void *extraData), void *extraData);

void LinkedList_removeData(LinkedList *list, void *data);

#endif