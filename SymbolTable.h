//
// Created by Leozki on 2017/6/2.
//

#ifndef QASM_SYMBOLTABLE_H
#define QASM_SYMBOLTABLE_H

#include "LinkedList.h"
#include "Constants.h"

// 符号表节点
typedef struct _SymbolNode {
    int iIndex;                     // 索引
    char pstrIdent[MAX_IDENT_SIZE]; // 标识符
    int iSize;                      // 大小(变量是1,数组是N)
    int iStackIndex;                // 符号指向的堆栈索引
    int iFuncIndex;                 // 符号所在的函数

} SymbolNode;

SymbolNode *GetSymbolByIdent(char *pstrIdent, int iFuncIndex);
int GetStackIndexByIdent(char *pstrIdent,int iFuncIndex);
// 返回符号大小
int GetSizeByIdent(char *pstrIdent,int iFuncIndex);
// 往符号表中添加 变量 或 数组
int AddSymbol(char *pstrIdent, int iSize, int iStackIndex, int iFuncIndex);



// 符号表
LinkedList g_symbolTable;

#endif //QASM_SYMBOLTABLE_H
