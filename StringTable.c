//
// Created by Leozki on 2017/6/2.
//

#include "StringTable.h"
#include "Constants.h"
#include "LinkedList.h"

int AddString(LinkedList *pList, char *pstrString) {

    // --- 首先检查是否字符串已经存在于表中

    LinkedListNode *pNode = pList->pHead;

    for (int iCurNode = 0; iCurNode < pList->iNodeCount; ++iCurNode) {

        // 如果当前节点的字符串等于该字符串，则返回它的索引
        if (strcmp((char *) pNode->pData, pstrString) == 0) {
            return iCurNode;
        } else {
            // 否则移动到下一个节点
            pNode = pNode->pNext;
        }
    }

    // --- 没有被添加，那么添加新的字符串
    char *pstrStringNode = (char *) malloc(strlen(pstrString) + 1);
    strcpy(pstrStringNode, pstrString);

    return AddNode(pList, pstrStringNode);
}


