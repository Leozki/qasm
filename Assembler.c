//
// Created by Leozki on 2017/6/28.
//

#include "Assembler.h"
#include "FuncTable.h"
#include "InstrLookupTable.h"
#include "LexicalAnalyzer.h"
#include "Error.h"
#include "SymbolTable.h"
#include "LabelTable.h"

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
                    ExitOnCodeError(ERROR_MSSG_INVALID_STACK_SIZE);
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

                if ( iIsFuncActive )
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
    }
}
