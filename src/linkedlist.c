#include "global.h"
#include "linkedlist.h"
#include "recomputils.h"

struct LinkedListNode {
    void *data;
    struct LinkedListNode *next;
    struct LinkedListNode *prev;
};

struct LinkedList {
    LinkedListNode *start;
    LinkedListNode *end;
};

LinkedListNode *LinkedListNode_getNext(LinkedListNode *n) {
    return n->next;
}

LinkedListNode *LinkedListNode_getPrev(LinkedListNode *n) {
    return n->prev;
}

void *LinkedListNode_getData(LinkedListNode *n) {
    return n->data;
}

LinkedListNode *createLinkedListNode(void *data) {
    LinkedListNode *n = recomp_alloc(sizeof(LinkedListNode));
    n->data = data;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

void destroyLinkedListNode(LinkedListNode *n) {
    recomp_free(n);
}

LinkedListNode *extractLinkedListNode(LinkedList *list, LinkedListNode *n) {
    if (n) {
        if (n == list->start) {
            list->start = n->next;
        }

        if (n == list->end) {
            list->end = n->prev;
        }

        if (n->prev) {
            n->prev->next = n->next;
        }

        if (n->next) {
            n->next->prev = n->prev;
        }
    }

    return n;
}

void LinkedList_initList(LinkedList *list) {
    list->start = NULL;
    list->end = NULL;
}

bool LinkedList_isEmpty(LinkedList *list) {
    return list->start == NULL;
}

void LinkedList_destroyList(LinkedList *list) {
    LinkedListNode *curr = list->start;

    LinkedListNode *next = NULL;

    while (curr) {
        next = curr->next;
        recomp_free(curr);
        curr = next;
    }
}

LinkedListNode *LinkedList_start(const LinkedList *list) {
    return list->start;
}

LinkedListNode *LinkedList_end(const LinkedList *list) {
    return list->end;
}

void LinkedList_insertBefore(LinkedList *list, LinkedListNode *existingLinkedListNode, LinkedListNode *LinkedListNodeToInsert) {
    LinkedListNodeToInsert->next = existingLinkedListNode;
    LinkedListNodeToInsert->prev = existingLinkedListNode->prev;

    if (existingLinkedListNode->prev) {
        existingLinkedListNode->prev->next = LinkedListNodeToInsert;
    }

    if (list->start == existingLinkedListNode) {
        list->start = LinkedListNodeToInsert;
    }
}

void LinkedList_insertAfter(LinkedList *list, LinkedListNode *existingLinkedListNode, LinkedListNode *LinkedListNodeToInsert) {
    LinkedListNodeToInsert->next = existingLinkedListNode->next;
    LinkedListNodeToInsert->prev = existingLinkedListNode;

    if (existingLinkedListNode && existingLinkedListNode->next) {
        existingLinkedListNode->next->prev = LinkedListNodeToInsert;
    }

    if (list->end == existingLinkedListNode) {
        list->end = LinkedListNodeToInsert;
    }
}

void LinkedList_addFront(LinkedList *list, void *data) {
    LinkedListNode *n = createLinkedListNode(data);
    if (!list->start) {
        list->start = n;
        list->end = n;
    } else {
        LinkedList_insertBefore(list, list->start, n);
    }
}

void LinkedList_addBack(LinkedList *list, void *data) {
    LinkedListNode *n = createLinkedListNode(data);
    n->data = data;
    if (!LinkedList_isEmpty(list)) {
        list->start = n;
        list->end = n;
    } else {
        LinkedList_insertAfter(list, list->end, n);
    }
}

void LinkedList_removeFront(LinkedList *list) {
    if (!LinkedList_isEmpty(list)) {
        recomp_free(extractLinkedListNode(list, list->start));
    }
}

void LinkedList_removeBack(LinkedList *list) {
    if (!LinkedList_isEmpty(list)) {
        recomp_free(extractLinkedListNode(list, list->end));
    }
}

void LinkedList_removeOnCondition(LinkedList *list, bool (*condFunc)(void *LinkedListNodeData, void *extraData), void *extraData) {
    LinkedListNode *curr = list->start;

    while (curr) {
        if (condFunc(curr->data, extraData)) {
            extractLinkedListNode(list, curr);
            destroyLinkedListNode(curr);
        }

        curr = curr->next;
    }
}

bool isPtrEqual(void *p1, void *p2) {
    return p1 == p2;
}

void LinkedList_removeData(LinkedList *list, void *data) {
    LinkedList_removeOnCondition(list, isPtrEqual, data);
}
