//
// Created by Leozki on 2017/6/2.
//

#include "SymbolTable.h"

SymbolNode *GetSymbolByIdent(char *strIdent, int iFuncIndex) {
    if (!g_symbolTable.iNodeCount)
        return NULL;

    LinkedListNode *pCurNode = g_symbolTable.pHead;

    for (int iCurNode = 0; iCurNode < g_symbolTable.iNodeCount; ++iCurNode) {
        SymbolNode *pCurSymbolNode = pCurNode->pData;

        // 当前节点是否和指定标识符匹配
        if (strcmp(pCurSymbolNode->pstrIdent, strIdent)) {

            // 检查作用域是否想同或者重叠(全局/局部)
            if (pCurSymbolNode->iFuncIndex == iFuncIndex
                && pCurSymbolNode->iStackIndex >= 0) {
                // 全局变量使用负索引
                return pCurSymbolNode;
            }
        }

        pCurNode = pCurNode->pNext;
    }

    // 没有找到指定节点
    return NULL;
}

int GetStackIndexByIdent(char *pstrIdent, int iFuncIndex) {
    SymbolNode *pNode = GetSymbolByIdent(pstrIdent, iFuncIndex);
    if (pNode) {
        return pNode->iStackIndex;
    }
    return 0;
}

int GetSizeByIdent(char *pstrIdent, int iFuncIndex) {
    SymbolNode *pNode = GetSymbolByIdent(pstrIdent, iFuncIndex);
    if (pNode) {
        return pNode->iSize;
    }
    return 0;
}

int AddSymbol(char *pstrIdent, int iSize, int iStackIndex, int iFuncIndex) {

    // 如果 标签 已经存在
    if (GetSymbolByIdent(pstrIdent, iFuncIndex))
        return -1;

    // 创建新的符号节点
    SymbolNode *pNewSymbolNode = (SymbolNode *) malloc(sizeof(SymbolNode));

    strcpy(pNewSymbolNode->pstrIdent, pstrIdent);
    pNewSymbolNode->iSize = iSize;
    pNewSymbolNode->iStackIndex = iStackIndex;
    pNewSymbolNode->iFuncIndex = iFuncIndex;

    int iIndex = AddNode(&g_symbolTable, pNewSymbolNode);

    pNewSymbolNode->iIndex = iIndex;

    // 返回新符号的索引
    return iIndex;
}
