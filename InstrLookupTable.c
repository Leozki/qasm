//
// Created by Leozki on 2017/6/2.
//

#include "InstrLookupTable.h"



InstrLookup g_InstrTable[MAX_INSTR_LOOKUP_COUNT];

void SetOpType(int iInstrIndex, int iOpIndex, OpTypes iOpType) {
    g_InstrTable[iInstrIndex].OpList[iOpIndex] = iOpType;
}

int AddInstrLookup(char *pstrMnemonic, int iOpcode, int iOpCount) {
    static int iInstrIndex = 0;

    if (iInstrIndex >= MAX_INSTR_LOOKUP_COUNT)
        return -1;

    strcpy(g_InstrTable[iInstrIndex].pstrMnemonic, pstrMnemonic);
    strupr(g_InstrTable[iInstrIndex].pstrMnemonic);

    g_InstrTable[iInstrIndex].iOpcode = iOpcode;
    g_InstrTable[iInstrIndex].iOpCount = iOpCount;

    g_InstrTable[iInstrIndex].OpList = (OpTypes *) malloc(iOpCount * sizeof(OpTypes));

    int iRetInstrIndex = iInstrIndex;
    iInstrIndex++;

    return iRetInstrIndex;
}


void InitInstrTable() {
    int iInstrIndex;

    // Mov Destination, Source
    iInstrIndex = AddInstrLookup("Mov", INSTR_MOV, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Add Destination, Source
    iInstrIndex = AddInstrLookup("Add", INSTR_ADD, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Sub Destination, Source
    iInstrIndex = AddInstrLookup("Sub", INSTR_SUB, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Mul Destination, Source
    iInstrIndex = AddInstrLookup("Mul", INSTR_MUL, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Div Destination, Source
    iInstrIndex = AddInstrLookup("Div", INSTR_DIV, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Mod Destination, Source
    iInstrIndex = AddInstrLookup("Mod", INSTR_MOD, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Exp Destination, Source

    iInstrIndex = AddInstrLookup("Exp", INSTR_EXP, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Neg Destination
    iInstrIndex = AddInstrLookup("Neg", INSTR_NEG, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Neg Destination
    iInstrIndex = AddInstrLookup("Neg", INSTR_NEG, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Inc Destination
    iInstrIndex = AddInstrLookup("Inc", INSTR_INC, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Dec Destination
    iInstrIndex = AddInstrLookup("Dec", INSTR_DEC, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // 位操作

    // And Destination, Source
    iInstrIndex = AddInstrLookup("And", INSTR_AND, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Or Destination, Source

    iInstrIndex = AddInstrLookup("Or", INSTR_OR, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // XOr Destination, Source
    iInstrIndex = AddInstrLookup("XOr", INSTR_XOR, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Not Destination
    iInstrIndex = AddInstrLookup("Not", INSTR_NOT, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // ShL Destination, Source
    iInstrIndex = AddInstrLookup("ShL", INSTR_SHL, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // ShR Destination, Source
    iInstrIndex = AddInstrLookup("ShR", INSTR_SHR, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // 字符串操作

    // Concat String0, String1
    iInstrIndex = AddInstrLookup("Concat", INSTR_CONCAT, 2);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG |
                              OP_FLAG_TYPE_STRING);

    // GetChar Destination, Source, Index
    iInstrIndex = AddInstrLookup("GetChar", INSTR_GETCHAR, 3);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG |
                              OP_FLAG_TYPE_STRING);
    SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG |
                              OP_FLAG_TYPE_INT);

    // SetChar Destination, Index, Source
    iInstrIndex = AddInstrLookup("SetChar", INSTR_SETCHAR, 3);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG |
                              OP_FLAG_TYPE_INT);
    SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG |
                              OP_FLAG_TYPE_STRING);

    // 条件跳转语句

    // Jmp Label
    iInstrIndex = AddInstrLookup("Jmp", INSTR_JMP, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_LINE_LABEL);

    // JE Op0, Op1, Label
    iInstrIndex = AddInstrLookup("JE", INSTR_JE, 3);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

    // JNE Op0, Op1, Label
    iInstrIndex = AddInstrLookup("JNE", INSTR_JNE, 3);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

    // JG Op0, Op1, Label
    iInstrIndex = AddInstrLookup("JG", INSTR_JG, 3);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

    // JL Op0, Op1, Label
    iInstrIndex = AddInstrLookup("JL", INSTR_JL, 3);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

    // JGE Op0, Op1, Label
    iInstrIndex = AddInstrLookup("JGE", INSTR_JGE, 3);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

    // JLE Op0, Op1, Label
    iInstrIndex = AddInstrLookup("JLE", INSTR_JLE, 3);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 1, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
    SetOpType(iInstrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);


    // 栈操作

    // Push Source
    iInstrIndex = AddInstrLookup("Push", INSTR_PUSH, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Pop Destination
    iInstrIndex = AddInstrLookup("Pop", INSTR_POP, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // 函数调用相关

    // Call FunctionName
    iInstrIndex = AddInstrLookup("Call", INSTR_CALL, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_FUNC_NAME);

    // Ret
    iInstrIndex = AddInstrLookup("Ret", INSTR_RET, 0);

    // CallHost FunctionName
    iInstrIndex = AddInstrLookup("CallHost", INSTR_CALLHOST, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_HOST_API_CALL);

    // 其他
    // Pause Duration
    iInstrIndex = AddInstrLookup("Pause", INSTR_PAUSE, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);

    // Exit Code
    iInstrIndex = AddInstrLookup("Exit", INSTR_EXIT, 1);
    SetOpType(iInstrIndex, 0, OP_FLAG_TYPE_INT |
                              OP_FLAG_TYPE_FLOAT |
                              OP_FLAG_TYPE_STRING |
                              OP_FLAG_TYPE_MEM_REF |
                              OP_FLAG_TYPE_REG);
}


int GetInstrByMnemonic(char *pstrMnemonic, InstrLookup *pInstr) {
    for (int iCurrInstrIndex = 0; iCurrInstrIndex < MAX_INSTR_LOOKUP_COUNT; ++iCurrInstrIndex)
        if (strcmp(g_InstrTable[iCurrInstrIndex].pstrMnemonic, pstrMnemonic) == 0) {
            *pInstr = g_InstrTable[iCurrInstrIndex];
            return TRUE;
        }
    return FALSE;
}

