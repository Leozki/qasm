//
// Created by Leozki on 2017/6/2.
//

#ifndef QASM_FUNCTABLE_H
#define QASM_FUNCTABLE_H

#include "LinkedList.h"
#include "Constants.h"

// 函数表节点
typedef struct _FuncNode {
    int iIndex;                     // 索引
    char *pstrName[MAX_IDENT_SIZE]; // 名称
    int iEntryPoint;                // 入口点
    int iParamCount;                // 参数的个数
    int iLocalDataSize;             // 局部数据大小
} FuncNode;


FuncNode *GetFuncByName(char *strName);

int AddFunc(char *strName, int iEntryPoint);

void SetFuncInfo(char *pstrName, int iParamCount, int iLocalDataSize);


// 函数表
LinkedList g_funcTable;

#endif //QASM_FUNCTABLE_H
