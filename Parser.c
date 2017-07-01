//
// Created by Leozki on 2017/6/28.
//

#include "Parser.h"
#include "FuncTable.h"
#include "InstrLookupTable.h"
#include "LexicalAnalyzer.h"
#include "Error.h"
#include "SymbolTable.h"
#include "LabelTable.h"
#include "StringTable.h"
#include "HostAPICallTable.h"

ScriptHeader g_ScriptHeader;
int g_iIsSetStackSizeFound;
int g_iIsSetPriorityFound;
Instr *g_pInstrStream = NULL;
int g_iInstrStreamSize; // 指令流的大小
int g_iCurrInstrIndex; // 当前指令的索引

void AssmblSourceFile() {
    // 初始化
    g_ScriptHeader.iStackSize = 0;
    g_ScriptHeader.iIsMainFuncPresent = FALSE;

    g_iInstrStreamSize = 0;
    g_iIsSetStackSizeFound = FALSE;
    g_iIsSetPriorityFound = FALSE;
    g_ScriptHeader.iGlobalDataSize = 0;

    int iIsFuncActive = FALSE;
    FuncNode *pCurrFunc;
    int iCurrFuncIndex = 0;
    char pstrCurrFuncName[MAX_IDENT_SIZE];
    int iCurrFuncParamCount = 0;
    int iCurrFuncLocalDataSize = 0;

    InstrLookup CurrInstr;

    // 重置分析器
    ResetLexer();

    // 循环扫描每一行

    while (TRUE) {
        // 没有 token 可以分析了就跳出
        if (END_OF_TOKEN_STREAM == GetNextToken())
            break;
        // printf("CurrToken = %d \n", g_Lexer.CurrToken);

        // 根据不同类型的 token 做出选择
        switch (g_Lexer.CurrToken) {
            // SetStackSize
            case TOKEN_TYPE_SETSTACKSIZE: {
                // 函数内部不允许定义 SetStackSize
                if (iIsFuncActive)
                    ExitOnCodeError(ERROR_MSSG_LOCAL_SETSTACKSIZE);

                // SetStackSize 只能出现一次
                if (g_iIsSetStackSizeFound)
                    ExitOnCodeError(ERROR_MSSG_MULTIPLE_SETSTACKSIZES);

                // SetStackSize 后只能跟 int
                if (TOKEN_TYPE_INT != GetNextToken())
                    ExitOnCodeError(ERROR_MSSG_INVALID_STACK_SIZE);

                g_ScriptHeader.iStackSize = atoi(GetCurrLexeme());

                g_iIsSetStackSizeFound = TRUE;
                break;
            }
            case TOKEN_TYPE_SETPRIORITY: {
                if (iIsFuncActive)
                    ExitOnCodeError(ERROR_MSSG_LOCAL_SETPRIORITY);
                if (g_iIsSetPriorityFound)
                    ExitOnCodeError(ERROR_MSSG_MULTIPLE_SETPRIORITIES);

                GetNextToken();

                // SetPriority 50
                // SetPriority Low
                switch (g_Lexer.CurrToken) {
                    case TOKEN_TYPE_INT: {
                        g_ScriptHeader.iUserPriority = atoi(GetCurrLexeme());
                        g_ScriptHeader.iStackSize = PRIORITY_USER;
                        break;
                    }
                    case TOKEN_TYPE_IDENT: {
                        if (stricmp(g_Lexer.pstrCurrLexeme, PRIORITY_LOW_KEYWORD) == 0)
                            g_ScriptHeader.iPriorityType = PRIORITY_LOW;
                        else if (stricmp(g_Lexer.pstrCurrLexeme, PRIORITY_MED_KEYWORD) == 0)
                            g_ScriptHeader.iPriorityType = PRIORITY_MED;
                        else if (stricmp(g_Lexer.pstrCurrLexeme, PRIORITY_HIGH_KEYWORD) == 0)
                            g_ScriptHeader.iPriorityType = PRIORITY_HIGH;
                        else
                            ExitOnCodeError(ERROR_MSSG_INVALID_PRIORITY);
                        break;
                    }
                    default:
                        ExitOnCodeError(ERROR_MSSG_INVALID_PRIORITY);
                        break;
                }
                g_iIsSetPriorityFound = TRUE;
                break;
            }
            case TOKEN_TYPE_VAR: {
                if (TOKEN_TYPE_IDENT != GetNextToken())
                    ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);

                char pstrIdent[MAX_IDENT_SIZE] = {0};
                strcpy(pstrIdent, GetCurrLexeme());

                // 默认数组大小为1
                int iSize = 1;
                // 向前看一个符号,判断一下是不是数组
                if ('[' == GetLookAheadChar()) {
                    if (TOKEN_TYPE_OPEN_BRACKET != GetNextToken())
                        ExitOnCharExpectedError('[');

                    if (TOKEN_TYPE_INT != GetNextToken())
                        ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_SIZE);
                    iSize = atoi(GetCurrLexeme());

                    // 确认一下数组大小是大于0的
                    if (iSize <= 0)
                        ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_SIZE);

                    // 确认下一个符号是 ]
                    if (TOKEN_TYPE_CLOSE_BRACKET != GetNextToken())
                        ExitOnCharExpectedError(']');
                }

                // 计算在栈中的索引
                int iStackIndex;
                if (iIsFuncActive)
                    iStackIndex = 0 - (iCurrFuncLocalDataSize + 2);
                else
                    // 全局的,所以等于 iGlobalDataSize
                    iStackIndex = g_ScriptHeader.iGlobalDataSize;

                if (-1 == AddSymbol(pstrIdent, iSize, iStackIndex, iCurrFuncIndex))
                    ExitOnError(ERROR_MSSG_IDENT_REDEFINITION);

                if (iIsFuncActive)
                    iCurrFuncLocalDataSize += iSize;
                else
                    g_ScriptHeader.iGlobalDataSize += iSize;

                break;
            }
            case TOKEN_TYPE_FUNC: {
                if (iIsFuncActive)
                    ExitOnError(ERROR_MSSG_NESTED_FUNC);

                // 读取下一个token 函数名
                if (TOKEN_TYPE_IDENT != GetNextToken())
                    ExitOnError(ERROR_MSSG_IDENT_EXPECTED);

                char *pstrFuncName = GetCurrLexeme();

                // 计算函数的入口点
                int iEntryPoint = g_iInstrStreamSize;

                // 把函数加入到函数表中
                int iFuncIndex = AddFunc(pstrFuncName, iEntryPoint);
                if (-1 == iFuncIndex)
                    ExitOnError(ERROR_MSSG_FUNC_REDEFINITION);

                // 判断是否是 Main 函数
                if (0 == strcmp(pstrFuncName, MAIN_FUNC_NAME)) {
                    // 记录 Main 函数已经出现
                    g_ScriptHeader.iIsMainFuncPresent = TRUE;
                    g_ScriptHeader.iMainFuncIndex = iFuncIndex;
                }

                // 设置分析已经进入了函数的作用于
                iIsFuncActive = TRUE;
                // 记录当前分析的函数
                strcpy(pstrCurrFuncName, pstrFuncName);
                iCurrFuncIndex = iFuncIndex;
                iCurrFuncParamCount = 0;
                iCurrFuncLocalDataSize = 0;

                // 忽略直到下一个 { 之前的空行
                while (TOKEN_TYPE_NEWLINE == GetNextToken());
                // 没有空行了,下一个 token 必须为 {
                if (TOKEN_TYPE_OPEN_BRACE != g_Lexer.CurrToken)
                    ExitOnCharExpectedError('{');
                g_iInstrStreamSize++;
                break;
            }
            case TOKEN_TYPE_CLOSE_BRACE: {
                if (!iIsFuncActive)
                    ExitOnCharExpectedError('}');

                // 到函数尾了,参数和变量都搜集完毕,设置当前函数的属性
                SetFuncInfo(pstrCurrFuncName, iCurrFuncParamCount, iCurrFuncLocalDataSize);

                // 离开函数作用域
                iIsFuncActive = FALSE;

                printf("Find Func = %s ,ParamCount =%d, LocalDataSize = %d \n",
                       pstrCurrFuncName, iCurrFuncParamCount, iCurrFuncLocalDataSize);
                break;
            }
            case TOKEN_TYPE_PARAM: {
                if (!iIsFuncActive)
                    ExitOnError(ERROR_MSSG_GLOBAL_PARAM);

                // Main 函数没有参数
                if (strcmp(pstrCurrFuncName, MAIN_FUNC_NAME) == 0)
                    ExitOnCodeError(ERROR_MSSG_MAIN_PARAM);

                if (GetNextToken() != TOKEN_TYPE_IDENT)
                    ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);

                // 记录当前函数的参数数量
                iCurrFuncParamCount++;
                break;
            }
            case TOKEN_TYPE_INSTR: {
                if (!iIsFuncActive)
                    ExitOnCodeError(ERROR_MSSG_GLOBAL_INSTR);
                ++g_iInstrStreamSize;

                break;
            }
            case TOKEN_TYPE_IDENT: {
                if (GetLookAheadChar() != ':')
                    ExitOnCodeError(ERROR_MSSG_INVALID_INSTR);
                if (!iIsFuncActive)
                    ExitOnCodeError(ERROR_MSSG_GLOBAL_LINE_LABEL);
                char *pstrIdent = GetCurrLexeme();

                int iTargetIndex = g_iInstrStreamSize - 1;

                int iFuncIndex = iCurrFuncIndex;


                if (AddLabel(pstrIdent, iTargetIndex, iFuncIndex) == -1)
                    ExitOnCodeError(ERROR_MSSG_LINE_LABEL_REDEFINITION);

                break;
            }
            default:
                if (g_Lexer.CurrToken != TOKEN_TYPE_NEWLINE)
                    ExitOnCodeError(ERROR_MSSG_INVALID_INPUT);
                break;
        }
        if (!SkipToNextLine())
            break;
    }

    // 第二遍填充指令信息
    g_pInstrStream = (Instr *) malloc(g_iInstrStreamSize * sizeof(Instr));
    for (int iCurrInstrIndex = 0; iCurrInstrIndex < g_iInstrStreamSize; ++iCurrInstrIndex)
        g_pInstrStream[iCurrInstrIndex].pOpList = NULL;
    g_iCurrInstrIndex = 0;

    ResetLexer();

    while (TRUE) {
        if (GetNextToken() == END_OF_TOKEN_STREAM)
            break;

        switch (g_Lexer.CurrToken) {
            case TOKEN_TYPE_FUNC: {
                GetNextToken();
                pCurrFunc = GetFuncByName(GetCurrLexeme());
                iIsFuncActive = TRUE;
                // 计算栈索引时需要用到
                iCurrFuncParamCount = 0;
                iCurrFuncIndex = pCurrFunc->iIndex;
                while (GetNextToken() == TOKEN_TYPE_NEWLINE);
                break;
            }
            case TOKEN_TYPE_CLOSE_BRACE: {
                iIsFuncActive = FALSE;

                if (strcmp((const char *) pCurrFunc->pstrName, MAIN_FUNC_NAME) == 0) {

                    // Main() 最后调用 Exit()
                    g_pInstrStream[g_iCurrInstrIndex].iOpcode = INSTR_EXIT;
                    g_pInstrStream[g_iCurrInstrIndex].iOpCount = 1;


                    // Main() 返回值是0
                    g_pInstrStream[g_iCurrInstrIndex].pOpList = (Op *) malloc(sizeof(Op));
                    g_pInstrStream[g_iCurrInstrIndex].pOpList[0].iType = OP_TYPE_INT;
                    g_pInstrStream[g_iCurrInstrIndex].pOpList[0].iIntLiteral = 0;
                } else {
                    // 不是Main() 自动添加 Ret 在函数尾
                    g_pInstrStream[g_iCurrInstrIndex].iOpcode = INSTR_RET;
                    g_pInstrStream[g_iCurrInstrIndex].iOpCount = 0;
                    g_pInstrStream[g_iCurrInstrIndex].pOpList = NULL;
                }

                ++g_iCurrInstrIndex;

                break;
            }
            case TOKEN_TYPE_PARAM: {
                if (!iIsFuncActive)
                    ExitOnError(ERROR_MSSG_GLOBAL_PARAM);

                if (GetNextToken() != TOKEN_TYPE_IDENT)
                    ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
                char *pstrIdent = GetCurrLexeme();

                // 参数的栈索引
                // 函数调用时栈帧的情况
                // 先 从右到左 push 参数,然后 push 返回地址.然后再 push 局部变量.
                // 栈顶的 iFuncIndex 是为了定位返回地址.
                // 用 iFuncIndex 从函数表中读取出 FuncNode 结构,从而读取局部变的数据大小(iLocalDataSize),就可以定位 Return Address
                /*
                 *                                                                 -1 | iFuncIndex OffsetIndex|
                 *                                          -1 | local0         |  -2 | local0                |
                 *                                          -2 | local1         |  -3 | local1                |
                 *                                          -3 | local2         |  -4 | local2                |
                 *                  -1 | Return Address|    -4 | Return Address |  -5 | Return Address        |
                 * -1 | param2 |    -2 | param2        |    -5 | param2         |  -6 | param2                |
                 * -2 | param1 | -> -3 | param1        | -> -6 | param1         |  -7 | param1                |
                 * -3 | param0 |    -4 | param0        |    -7 | param0         |  -8 | param0                |
                 *       -----          ----------------        ----------------       -----------------------
                 */
                int iStackIndex = -(pCurrFunc->iLocalDataSize + 2 + (iCurrFuncParamCount + 1));
                if (AddSymbol(pstrIdent, 1, iStackIndex, iCurrFuncIndex) == -1)
                    ExitOnCodeError(ERROR_MSSG_IDENT_REDEFINITION);
                ++iCurrFuncParamCount;

                break;
            }
            case TOKEN_TYPE_INSTR: {
                // 处理汇编指令
                GetInstrByMnemonic(GetCurrLexeme(), &CurrInstr);
                g_pInstrStream[g_iCurrInstrIndex].iOpcode = CurrInstr.iOpcode;
                g_pInstrStream[g_iCurrInstrIndex].iOpCount = CurrInstr.iOpCount;

                Op *pOpList = (Op *) malloc(CurrInstr.iOpCount * sizeof(Op));

                for (int iCurrOpIndex = 0; iCurrOpIndex < CurrInstr.iOpCount; ++iCurrOpIndex) {
                    OpTypes CurrOpTypes = CurrInstr.OpList[iCurrOpIndex];
                    Token InitOpToken = GetNextToken();
                    switch (InitOpToken) {
                        case TOKEN_TYPE_INT:
                            if (CurrOpTypes & OP_FLAG_TYPE_INT) {
                                pOpList[iCurrOpIndex].iType = OP_TYPE_INT;
                                pOpList[iCurrOpIndex].iIntLiteral = atoi(GetCurrLexeme());
                            } else
                                ExitOnCodeError(ERROR_MSSG_INVALID_OP);
                            break;

                        case TOKEN_TYPE_FLOAT:
                            if (CurrOpTypes & OP_FLAG_TYPE_FLOAT) {
                                pOpList[iCurrOpIndex].iType = OP_TYPE_FLOAT;
                                pOpList[iCurrOpIndex].fFloatLiteral = (float) atof(GetCurrLexeme());
                            } else
                                ExitOnCodeError(ERROR_MSSG_INVALID_OP);
                            break;
                        case TOKEN_TYPE_QUOTE: {
                            if (CurrOpTypes & OP_FLAG_TYPE_STRING) {
                                GetNextToken();
                                switch (g_Lexer.CurrToken) {
                                    case TOKEN_TYPE_QUOTE: {
                                        // 空字符串转化为0
                                        pOpList[iCurrOpIndex].iType = OP_TYPE_INT;
                                        pOpList[iCurrOpIndex].iIntLiteral = 0;
                                        break;
                                    }
                                    case TOKEN_TYPE_STRING: {
                                        // 添加到字符串表
                                        char *pstrString = GetCurrLexeme();
                                        int iStringIndex = AddString(&g_StringTable, pstrString);

                                        if (GetNextToken() != TOKEN_TYPE_QUOTE)
                                            ExitOnCharExpectedError('\\');

                                        pOpList[iCurrOpIndex].iType = OP_TYPE_STRING_INDEX;
                                        pOpList[iCurrOpIndex].iStringTableIndex = iStringIndex;
                                        break;
                                    }
                                    default:
                                        ExitOnCodeError(ERROR_MSSG_INVALID_STRING);
                                }
                            } else
                                ExitOnCodeError(ERROR_MSSG_INVALID_OP);

                            break;
                        }
                        case TOKEN_TYPE_REG_RETVAL:
                            if (CurrOpTypes & OP_FLAG_TYPE_REG) {
                                // Set a register type
                                pOpList[iCurrOpIndex].iType = OP_TYPE_REG;
                                pOpList[iCurrOpIndex].iReg = 0;
                            } else
                                ExitOnCodeError(ERROR_MSSG_INVALID_OP);
                            break;

                            // Identifiers
                            // 1.Variables/Array Indices
                            // 2.Line Labels
                            // 3.Function Names
                            // 4.Host API Calls

                        case TOKEN_TYPE_IDENT: {
                            if (CurrOpTypes & OP_FLAG_TYPE_MEM_REF) {
                                char pstrIdent[MAX_IDENT_SIZE];
                                strcpy(pstrIdent, GetCurrLexeme());

                                if (!GetSymbolByIdent(pstrIdent, iCurrFuncIndex))
                                    ExitOnCodeError(ERROR_MSSG_UNDEFINED_IDENT);

                                int iBaseIndex = GetStackIndexByIdent(pstrIdent, iCurrFuncIndex);

                                if (GetLookAheadChar() != '[') {
                                    // 不是数组
                                    if (GetSizeByIdent(pstrIdent, iCurrFuncIndex) > 1)
                                        ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_NOT_INDEXED);

                                    pOpList[iCurrOpIndex].iType = OP_TYPE_ABS_STACK_INDEX;
                                    pOpList[iCurrOpIndex].iIntLiteral = iBaseIndex;
                                } else {

                                    if (GetSizeByIdent(pstrIdent, iCurrFuncIndex) == 1)
                                        ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY);

                                    if (GetNextToken() != TOKEN_TYPE_OPEN_BRACKET)
                                        ExitOnCharExpectedError('[');

                                    Token IndexToken = GetNextToken();
                                    if (IndexToken == TOKEN_TYPE_INT) {
                                        int iOffsetIndex = atoi(GetCurrLexeme());
                                        pOpList[iCurrOpIndex].iType = OP_TYPE_ABS_STACK_INDEX;
                                        pOpList[iCurrOpIndex].iStackIndex = iBaseIndex + iOffsetIndex;
                                    } else if (IndexToken == TOKEN_TYPE_IDENT) {
                                        char *pstrIndexIdent = GetCurrLexeme();

                                        if (!GetSymbolByIdent(pstrIndexIdent, iCurrFuncIndex))
                                            ExitOnCodeError(ERROR_MSSG_UNDEFINED_IDENT);

                                        if (GetSizeByIdent(pstrIndexIdent, iCurrFuncIndex) > 1)
                                            ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_INDEX);

                                        int iOffsetIndex = GetStackIndexByIdent(pstrIndexIdent, iCurrFuncIndex);

                                        pOpList[iCurrOpIndex].iType = OP_TYPE_REL_STACK_INDEX;
                                        pOpList[iCurrOpIndex].iStackIndex = iBaseIndex;
                                        pOpList[iCurrOpIndex].iOffsetIndex = iOffsetIndex;
                                    } else {
                                        ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_INDEX);
                                    }

                                    if (GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET)
                                        ExitOnCharExpectedError('[');
                                }
                            }

                            if (CurrOpTypes & OP_FLAG_TYPE_LINE_LABEL) {
                                char *pstrLabelIdent = GetCurrLexeme();
                                LabelNode *pLabel = GetLabelByIdent(pstrLabelIdent, iCurrFuncIndex);

                                // 如果 标签 不存在
                                if (!pLabel)
                                    ExitOnCodeError(ERROR_MSSG_UNDEFINED_LINE_LABEL);

                                pOpList[iCurrOpIndex].iType = OP_TYPE_INSTR_INDEX;
                                pOpList[iCurrOpIndex].iInstrIndex = pLabel->iTargetIndex;
                            }

                            if (CurrOpTypes & OP_FLAG_TYPE_FUNC_NAME) {
                                char *pstrFuncName = GetCurrLexeme();
                                FuncNode *pFunc = GetFuncByName(pstrFuncName);

                                // 调用的方法不存在
                                if (!pFunc)
                                    ExitOnCodeError(ERROR_MSSG_UNDEFINED_FUNC);


                                pOpList[iCurrOpIndex].iType = OP_TYPE_FUNC_INDEX;
                                pOpList[iCurrOpIndex].iFuncIndex = pFunc->iIndex;
                            }

                            // host API call
                            if (CurrOpTypes & OP_FLAG_TYPE_HOST_API_CALL) {
                                char *pstrHostAPICall = GetCurrLexeme();
                                int iIndex = AddString(&g_HostAPICallTable, pstrHostAPICall);

                                pOpList[iCurrOpIndex].iType = OP_TYPE_HOST_API_CALL_INDEX;
                                pOpList[iCurrOpIndex].iHostAPICallIndex = iIndex;
                            }
                            break;
                        }
                        default:
                            ExitOnCodeError(ERROR_MSSG_INVALID_OP);
                            break;
                    }
                    if (iCurrOpIndex < CurrInstr.iOpCount - 1)
                        if (GetNextToken() != TOKEN_TYPE_COMMA)
                            ExitOnCharExpectedError(',');
                }

                if (GetNextToken() != TOKEN_TYPE_NEWLINE)
                    ExitOnCodeError(ERROR_MSSG_INVALID_INPUT);

                g_pInstrStream[g_iCurrInstrIndex].pOpList = pOpList;
                ++g_iCurrInstrIndex;
                break;
            }
        }
        if (!SkipToNextLine())
            break;
    }
}
