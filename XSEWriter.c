//
// Created by Leozki on 2017/6/30.
//

#include <stdio.h>
#include "Constants.h"
#include "XSEWriter.h"
#include "SourceLoader.h"
#include "Parser.h"
#include "FuncTable.h"
#include "StringTable.h"
#include "HostAPICallTable.h"

#define XSE_ID_STRING               "XSE0"

#define VERSION_MAJOR               0
#define VERSION_MINOR               8

void BuildXSE() {

    FILE *pExecFile;
    if (!(pExecFile = fopen(g_pstrExecFilename, "wb"))) {
        ExitOnError("Could not open executable file for output ");
    }

    // ------------------------------------------------------
    // Header
    // ------------------------------------------------------

    // Id string(4 bytes)
    fwrite(XSE_ID_STRING, 4, 1, pExecFile);

    // Version (2 bytes)
    char cVersionMajor = VERSION_MAJOR;
    char cVersionMinor = VERSION_MINOR;
    fwrite(&cVersionMajor, 1, 1, pExecFile);
    fwrite(&cVersionMinor, 1, 1, pExecFile);

    // stack size (4 bytes)
    fwrite(&g_ScriptHeader.iStackSize, 4, 1, pExecFile);

    // global data size (4 bytes)
    fwrite(&g_ScriptHeader.iGlobalDataSize, 4, 1, pExecFile);

    // _Main() flag (1 byte)
    char cIsMainPresent = 0;
    if (g_ScriptHeader.iIsMainFuncPresent) {
        cIsMainPresent = 1;
    }
    fwrite(&cIsMainPresent, 1, 1, pExecFile);

    // _Main() function index (4 bytes)
    fwrite(&g_ScriptHeader.iMainFuncIndex, 4, 1, pExecFile);

    // priorty type (1 byte)
    fwrite(&g_ScriptHeader.iPriorityType, 1, 1, pExecFile);

    // user-defined priority (4 bytes)
    fwrite(&g_ScriptHeader.iUserPriority, 4, 1, pExecFile);

    // ------------------------------------------------------
    // 指令流
    // ------------------------------------------------------

    // instruction count (4 bytes)
    fwrite(&g_iInstrStreamSize, 4, 1, pExecFile);

    for (int iCurrInstrIndex = 0; iCurrInstrIndex < g_iInstrStreamSize; ++iCurrInstrIndex) {
        // opcode
        short sOpcode = (short) g_pInstrStream[iCurrInstrIndex].iOpcode;
        fwrite(&sOpcode, 2, 1, pExecFile);
        // opcount
        char iOpCount = (char) g_pInstrStream[iCurrInstrIndex].iOpCount;
        fwrite(&iOpCount, 1, 1, pExecFile);
        //
        for (int iCurrOpIndex = 0; iCurrOpIndex < iOpCount; ++iCurrOpIndex) {
            Op CurrOp = g_pInstrStream[iCurrInstrIndex].pOpList[iCurrOpIndex];
            char cOpType = (char) CurrOp.iType;
            fwrite(&cOpType, 1, 1, pExecFile);
            switch (CurrOp.iType) {
                case OP_TYPE_INT:
                    fwrite(&CurrOp.iIntLiteral, sizeof(int), 1, pExecFile);
                    break;
                case OP_TYPE_FLOAT:
                    fwrite(&CurrOp.fFloatLiteral, sizeof(float), 1, pExecFile);
                    break;
                case OP_TYPE_STRING_INDEX:
                    fwrite(&CurrOp.iStringTableIndex, sizeof(int), 1, pExecFile);
                    break;
                case OP_TYPE_INSTR_INDEX:
                    fwrite(&CurrOp.iInstrIndex, sizeof(int), 1, pExecFile);
                    break;
                case OP_TYPE_ABS_STACK_INDEX:
                    fwrite(&CurrOp.iStackIndex, sizeof(int), 1, pExecFile);
                    break;
                case OP_TYPE_REL_STACK_INDEX:
                    fwrite(&CurrOp.iStackIndex, sizeof(int), 1, pExecFile);
                    fwrite(&CurrOp.iOffsetIndex, sizeof(int), 1, pExecFile);
                    break;
                case OP_TYPE_FUNC_INDEX:
                    fwrite(&CurrOp.iFuncIndex, sizeof(int), 1, pExecFile);
                    break;
                case OP_TYPE_HOST_API_CALL_INDEX:
                    fwrite(&CurrOp.iHostAPICallIndex, sizeof(int), 1, pExecFile);
                    break;
                case OP_TYPE_REG:
                    fwrite(&CurrOp.iReg, sizeof(int), 1, pExecFile);
                    break;
                default:
                    break;
            }
        }

    }

    int iCurrNode;
    LinkedListNode *pNode;

    // --------------------------------------
    // 字符串表
    // --------------------------------------

    //  string count (4 bytes)

    fwrite(&g_StringTable.iNodeCount, 4, 1, pExecFile);
    pNode = g_StringTable.pHead;
    char cParamCount;

    for (iCurrNode = 0; iCurrNode < g_StringTable.iNodeCount; ++iCurrNode) {
        char *pstrCurrString = (char *) pNode->pData;
        int iCurrStringLength = strlen(pstrCurrString);
        // 该字符串长度
        fwrite(&iCurrStringLength, 4, 1, pExecFile);
        // 字符串内容
        fwrite(pstrCurrString, strlen(pstrCurrString), 1, pExecFile);
        pNode = pNode->pNext;
    }

    // --------------------------------------
    // 函数表
    // --------------------------------------

    // function count (4 bytes)

    fwrite(&g_FuncTable.iNodeCount, 4, 1, pExecFile);
    pNode = g_FuncTable.pHead;

    for (iCurrNode = 0; iCurrNode < g_FuncTable.iNodeCount; ++iCurrNode) {
        FuncNode *pFunc = (FuncNode *) pNode->pData;
        fwrite(&pFunc->iEntryPoint, sizeof(int), 1, pExecFile);
        cParamCount = (char) pFunc->iParamCount;
        fwrite(&cParamCount, 1, 1, pExecFile);

        // local data size (4 bytes)
        fwrite(&pFunc->iLocalDataSize, sizeof(int), 1, pExecFile);
        // function name length (1 byte)
        char cFuncNameLength = (char) strlen((const char *) pFunc->pstrName);
        fwrite(&cFuncNameLength, 1, 1, pExecFile);
        // unction name (N bytes)
        fwrite(&pFunc->pstrName, strlen((const char *) pFunc->pstrName), 1, pExecFile);
        pNode = pNode->pNext;
    }


    // --------------------------------------
    // 主应用程序调用表
    // --------------------------------------

    // call count (4 bytes)

    fwrite(&g_HostAPICallTable.iNodeCount, 4, 1, pExecFile);

    pNode = g_HostAPICallTable.pHead;
    for (iCurrNode = 0; iCurrNode < g_HostAPICallTable.iNodeCount; ++iCurrNode) {
        char *pstrCurrHostAPICall = (char *) pNode->pData;
        char cCurrHostAPICallLength = (char) strlen(pstrCurrHostAPICall);

        // length (1 byte)
        fwrite(&cCurrHostAPICallLength, 1, 1, pExecFile);
        // string data (N bytes)
        fwrite(pstrCurrHostAPICall, strlen(pstrCurrHostAPICall), 1, pExecFile);

        pNode = pNode->pNext;
    }

    fclose(pExecFile);
}
