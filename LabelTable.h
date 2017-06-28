//
// Created by Leozki on 2017/6/28.
//

#ifndef XASM_LABELTABLE_H
#define XASM_LABELTABLE_H

#include "Constants.h"
#include "LinkedList.h"

typedef struct _LabelNode {
    int iIndex;
    char pstrIdent[MAX_IDENT_SIZE]; // 标签名
    int iTargetIndex;
    int iFuncIndex;             // 函数索引
} LabelNode;

// 通过标签名称获取标签
LabelNode *GetLabelByIdent(char *pstrIdent, int iFuncIndex);

// 添加标签到标签表
int AddLabel(char *pstrIdent, int iTargetIndex, int iFuncIndex);


LinkedList g_LabelTable;

#endif //XASM_LABELTABLE_H

