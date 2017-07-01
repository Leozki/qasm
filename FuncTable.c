//
// Created by Leozki on 2017/6/2.
//

#include "FuncTable.h"
#include "Constants.h"


FuncNode *GetFuncByName(char *strName) {
    if (!g_FuncTable.iNodeCount) return NULL;

    LinkedListNode *pCurNode = g_FuncTable.pHead;

    for (int iCurNode = 0; iCurNode < g_FuncTable.iNodeCount; ++iCurNode) {
        FuncNode *pCurrFunc = (FuncNode *) pCurNode->pData;

        if (strcmp((const char *) pCurrFunc->pstrName, strName) == 0) {
            return pCurrFunc;
        } else {
            pCurNode = pCurNode->pNext;
        }
    }
    return NULL;
}

int AddFunc(char *strName, int iEntryPoint) {
    if (GetFuncByName(strName))
        return -1;

    printf("AddFunc FuncName=%s,EntryPoint=%d\n", strName, iEntryPoint);
    // 创建一个新的函数节点
    FuncNode *pNewFuncNode = (FuncNode *) malloc(sizeof(FuncNode));

    // 初始化新函数
    strcpy((char *) pNewFuncNode->pstrName, strName);
    pNewFuncNode->iEntryPoint = iEntryPoint;

    // 添加到函数表并取出索引
    int iIndex = AddNode(&g_FuncTable, pNewFuncNode);
    pNewFuncNode->iIndex = iIndex;

    // 以后会填充
    pNewFuncNode->iParamCount = -1;
    pNewFuncNode->iLocalDataSize = -1;

    return iIndex;
}

void SetFuncInfo(char *pstrName, int iParamCount, int iLocalDataSize) {
    FuncNode *pNode = GetFuncByName(pstrName);
    if (pNode) {
        pNode->iParamCount = iParamCount;
        pNode->iLocalDataSize = iLocalDataSize;
    }
}
