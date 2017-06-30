//
// Created by Leozki on 2017/6/28.
//

#ifndef XASM_ASSEMBLER_H
#define XASM_ASSEMBLER_H

#include "Constants.h"

typedef struct _ScriptHeader {
    int iStackSize;
    int iGlobalDataSize;

    int iIsMainFuncPresent; // 是否存在Main函数
    int iMainFuncIndex;     // main函数在函数表中的索引

    int iPriorityType;
    int iUserPriority;
} ScriptHeader;

typedef struct _Op {
    int iType;
    union {
        int iIntLiteral;
        float fFloatLiteral;
        int iStringTableIndex;
        int iStackIndex;
        int iInstrIndex;
        int iFuncIndex;
        int iHostAPICallIndex;
        int iReg;
    };
    int iOffsetIndex;
} Op;

typedef struct _Instr {
    int iOpcode;
    int iOpCount;
    Op *pOpList;
} Instr;

// 编译文件
void AssmblSourceFile();

extern ScriptHeader g_ScriptHeader;
extern int g_iIsSetStackSizeFound;
extern int g_iIsSetPriorityFound;
extern Instr *g_pInstrStream;
extern int g_iInstrStreamSize; // 指令流的大小
extern int g_iCurrInstrIndex; // 当前指令的索引

#endif //XASM_ASSEMBLER_H
