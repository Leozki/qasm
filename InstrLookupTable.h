//
// Created by Leozki on 2017/6/2.
//

#ifndef QASM_INSTRLOOKUPTABLE_H
#define QASM_INSTRLOOKUPTABLE_H

#include "Constants.h"

typedef int OpTypes;

typedef struct _InstrLookup {
    char pstrMnemonic[MAX_INSTR_MNEMONIC_SIZE]; // 助记符
    int iOpcode;                                    // 操作码
    int iOpCount;                                   // 操作数个数
    OpTypes *OpList;                               // 操作数列表指针
} InstrLookup;

void SetOpType(int iInstrIndex, int iOpIndex, OpTypes iOpType);

// 添加指令
int AddInstrLookup(char *pstrMnemonic, int iOpcode, int iOpCount);

// 初始化指令表
void InitInstrTable();


// 根据助记符查找指令
int GetInstrByMnemonic(char *pstrMnemonic, InstrLookup *pInstr);

extern  InstrLookup g_InstrTable[MAX_INSTR_LOOKUP_COUNT];


#endif //QASM_INSTRLOOKUPTABLE_H

