//
// Created by Leozki on 2017/6/2.
//

#ifndef QASM_LINKEDLIST_H
#define QASM_LINKEDLIST_H


typedef struct _LinkedListNode {
    void *pData;
    struct _LinkedListNode *pNext;
} LinkedListNode;

typedef struct _LinkedList {
    int iNodeCount;
    LinkedListNode *pHead, *pTail;
} LinkedList;

void InitLinkedList(LinkedList *pList);

void FreeLinkedList(LinkedList *pList);

int AddNode(LinkedList *pList, void *pData);



#endif //QASM_LINKEDLIST_H
