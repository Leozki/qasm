//
// Created by Leozki on 2017/6/28.
//

#include "LabelTable.h"

LabelNode *GetLabelByIdent(char *pstrIdent, int iFuncIndex) {
    if (!g_LabelTable.iNodeCount)
        return NULL;

    LinkedListNode *pCurrNode = g_LabelTable.pHead;

    for (int iCurrNode = 0; iCurrNode < g_LabelTable.iNodeCount; ++iCurrNode) {
        LabelNode *pCurrLabel = (LabelNode *) pCurrNode->pData;

        // 如果名称匹配就返回当前指向的node
        if (strcmp(pCurrLabel->pstrIdent, pstrIdent) == 0
            && pCurrLabel->iFuncIndex == iFuncIndex)
            return pCurrLabel;
        pCurrNode = pCurrNode->pNext;
    }
    // 没有找到
    return NULL;
}

int AddLabel(char *pstrIdent, int iTargetIndex, int iFuncIndex) {

    // 如果标签表已经存在,则不添加
    if (GetLabelByIdent(pstrIdent,iFuncIndex))
        return -1;

    LabelNode *pNewLabel = (LabelNode*)malloc(sizeof(LabelNode));

    // 初始化
    strcpy(pNewLabel->pstrIdent,pstrIdent);
    pNewLabel->iTargetIndex = iTargetIndex;
    pNewLabel->iFuncIndex = iFuncIndex;

    // 添加到表中,并获取在表中的索引
    int iIndex = AddNode(&g_LabelTable,pNewLabel);

    pNewLabel->iIndex = iIndex;

    return iIndex;
}
