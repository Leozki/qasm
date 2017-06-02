//
// Created by Leozki on 2017/6/2.
//

#include "LinkedList.h"
#include "Constants.h"

void InitLinkedList(LinkedList *pList) {
    pList->pHead = NULL;
    pList->pTail = NULL;
    pList->iNodeCount = 0;
}

void FreeLinkedList(LinkedList *pList) {

    // 如果链表为空,则退出
    if (!pList)
        return;

    // 如果链表不为空,释放每一个节点

    if (pList->iNodeCount) {
        // 创建保存当前节点和下一个节点的指针
        LinkedListNode *pCurNode, *pNextNode;

        pCurNode = pList->pHead;

        // 遍历链表
        while (TRUE) {
            // 释放当前节点之前保存下一节点的指针
            pNextNode = pCurNode->pNext;

            // 清除当前节点数据
            if (pCurNode->pData)
                free(pCurNode->pData);

            // 清除节点
            if (pCurNode)
                free(pCurNode);

            // 如果存在下一节点就继续，否则退出
            if (pNextNode)
                pCurNode = pNextNode;
            else
                break;
        }
    }

}

int AddNode(LinkedList *pList, void *pData) {
    LinkedListNode *pNewNode = (LinkedListNode *) malloc(sizeof(LinkedListNode));

    pNewNode->pData = pData;
    pNewNode->pNext = NULL;

    // 如果当前链表是空,把头尾指针都设置成新节点
    if (!pList->iNodeCount) {

        // 把链表的头和尾指向新节点
        pList->pHead = pNewNode;
        pList->pTail = pNewNode;

    } else {
        // 把末节点的下一个节点指为新节点
        pList->pTail->pNext = pNewNode;

        // 更新链表的尾指针
        pList->pTail = pNewNode;
    }

    // 增大节点数量
    pList->iNodeCount++;

    // 返回新节点的索引

    return pList->iNodeCount - 1;
}
