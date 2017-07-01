//
// Created by Leozki on 2017/6/2.
//

#ifndef QASM_STRING_TABLE_H
#define QASM_STRING_TABLE_H

#include "LinkedList.h"
#include "Constants.h"

// 字符串表添加
int AddString(LinkedList *pList, char *pstrString);

// 字符串表
LinkedList g_StringTable;


#endif //QASM_STRING_TABLE_H
