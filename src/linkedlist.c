#include "global.h"
#include "linkedlist.h"
#include "recomputils.h"
#include "recompdata.h"

struct LinkedListNode {
    void *data;
    struct LinkedListNode *next;
    struct LinkedListNode *prev;
};

struct LinkedList {
    LinkedListNode *start;
    LinkedListNode *end;
    U32MemoryHashmapHandle dataToNodes;
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

bool isNodeInList(LinkedList *list, LinkedListNode *n) {
    if (!n) {
        return false;
    }

    return recomputil_u32_memory_hashmap_contains(list->dataToNodes, (uintptr_t)n->data);
}

// Removes node from list but does not delete it
void extractNodeInList(LinkedList *list, LinkedListNode *n) {
    if (isNodeInList(list, n)) {
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

        n->next = NULL;
        n->prev = NULL;
    }
}

LinkedListNode *extractOrCreateNodeInList(LinkedList *list, void *data) {
    bool isNewNode = recomputil_u32_memory_hashmap_create(list->dataToNodes, (uintptr_t)data);

    LinkedListNode *n = recomputil_u32_memory_hashmap_get(list->dataToNodes, (uintptr_t)data);

    if (isNewNode) {
        extractNodeInList(list, n);
    } else {
        n->data = data;
        n->next = NULL;
        n->prev = NULL;
    }

    return n;
}

void destroyNodeInList(LinkedList *list, LinkedListNode *n) {
    extractNodeInList(list, n);
    recomputil_u32_memory_hashmap_erase(list->dataToNodes, (uintptr_t)n->data);
}

void LinkedList_initList(LinkedList *list) {
    list->start = NULL;
    list->end = NULL;
    list->dataToNodes = recomputil_create_u32_value_hashmap();
}

bool LinkedList_isEmpty(LinkedList *list) {
    return list->start == NULL;
}

void LinkedList_destroyList(LinkedList *list) {
    LinkedListNode *curr = list->start;

    LinkedListNode *next = NULL;

    while (curr) {
        destroyNodeInList(list, curr);
    }
}

LinkedListNode *LinkedList_start(const LinkedList *list) {
    return list->start;
}

LinkedListNode *LinkedList_end(const LinkedList *list) {
    return list->end;
}

bool insertNodeBefore(LinkedList *list, LinkedListNode *existingLinkedListNode, LinkedListNode *LinkedListNodeToInsert) {
    LinkedListNodeToInsert->next = existingLinkedListNode;
    LinkedListNodeToInsert->prev = existingLinkedListNode->prev;

    if (existingLinkedListNode->prev) {
        existingLinkedListNode->prev->next = LinkedListNodeToInsert;
    }

    if (list->start == existingLinkedListNode) {
        list->start = LinkedListNodeToInsert;
    }
}

bool insertNodeAfter(LinkedList *list, LinkedListNode *existingLinkedListNode, LinkedListNode *LinkedListNodeToInsert) {
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
    LinkedListNode *n = extractOrCreateNodeInList(list, data);

    if (LinkedList_isEmpty(list)) {
        list->start = n;
        list->end = n;
    } else {
        insertNodeBefore(list, list->start, n);
    }
}

void LinkedList_addBack(LinkedList *list, void *data) {
    LinkedListNode *n = extractOrCreateNodeInList(list, data);

    if (LinkedList_isEmpty(list)) {
        list->start = n;
        list->end = n;
    } else {
        insertAfter(list, list->end, n);
    }
}

void LinkedList_removeFront(LinkedList *list) {
    if (!LinkedList_isEmpty(list)) {
        destroyNodeInList(list, list->start);
    }
}

void LinkedList_removeBack(LinkedList *list) {
    if (!LinkedList_isEmpty(list)) {
        destroyNodeInList(list, list->end);
    }
}

void LinkedList_removeOnCondition(LinkedList *list, bool (*condFunc)(void *LinkedListNodeData, void *extraData), void *extraData) {
    LinkedListNode *curr = list->start;

    while (curr) {
        if (condFunc(curr->data, extraData)) {
            destroyNodeInList(list, curr);
        }

        curr = curr->next;
    }
}

LinkedListNode *LinkedList_getNodeWithData(LinkedList *list, void *data) {
    return recomputil_u32_memory_hashmap_get(list->dataToNodes, (uintptr_t)data);
}

void LinkedList_removeData(LinkedList *list, void *data) {
    LinkedListNode *n = LinkedList_getNodeWithData(list, data);
    if (n) {
        destroyNodeInList(list, n);
    }
}
