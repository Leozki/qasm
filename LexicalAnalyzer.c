//
// Created by Leozki on 2017/6/28.
//

#include "LexicalAnalyzer.h"
#include "SourceLoader.h"
#include "StringUtils.h"
#include "InstrLookupTable.h"

void ResetLexer() {
    g_Lexer.iCurrSourceLine = 0;

    g_Lexer.iIndex0 = 0;
    g_Lexer.iIndex1 = 0;

    g_Lexer.CurrToken = TOKEN_TYPE_INVALID;

    g_Lexer.iCurrLexState = LEX_STATE_NO_STRING;
}

Token GetNextToken() {
    // 单词提取
    // 将第一个索引 Index0 移到最后一个属性自的尾部
    // 最后一个属性字尾部是用第二个索引 Index1 标识
    g_Lexer.iIndex0 = g_Lexer.iIndex1;

    // 确保没有扫描到当前行的尾部。
    // 如果一个字符串有8个字符,它的索引就是0~7
    // 因此,索引8将在字符串的外面平且需要移动到下一行.
    if (g_Lexer.iIndex0 >= strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceLine])) {
        // 如果是,就跳到下一行,确保没有超过文件尾部
        if (!SkipToNextLine())
            return END_OF_TOKEN_STREAM;
    }

    // 结束了对字符串的分析，返回正常状态
    if (LEX_STATE_END_STRING == g_Lexer.iCurrLexState)
        g_Lexer.iCurrLexState = LEX_STATE_NO_STRING;

    // 跳到下一个单词前的空白符号
    // 字符串中间的空白符号需要保留
    if (g_Lexer.iCurrLexState != LEX_STATE_IN_STRING) {
        // 扫描空白符并检查是否到行尾
        while (TRUE) {
            // 不是空白符就跳出，说明已经到了单词的开始
            if (!IsCharWhitespace(g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex0]))
                break;
            // 如果是空白符就移到下一个字符并继续扫描
            g_Lexer.iIndex0++;
        }
    }

    g_Lexer.iIndex1 = g_Lexer.iIndex0;

    // 继续扫描直到分隔符
    while (TRUE) {
        // 当前是否在扫描字符串
        if (LEX_STATE_IN_STRING == g_Lexer.iCurrLexState) {
            if (g_Lexer.iIndex1 >= strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceLine])) {
                // 字符串不能换行
                g_Lexer.CurrToken = TOKEN_TYPE_INVALID;
                return g_Lexer.CurrToken;
            }

            // 处理转义的反斜杠，向前移动2个字符
            if (g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex1] == '\\') {
                g_Lexer.iIndex1 += 2;
                continue;
            }

            // 到达字符串尾巴，结束
            if ('"' == g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex1])
                break;
            g_Lexer.iIndex1++;
        } else {
            // 不是字符串

            // 到达行尾，单词结束跳出
            if (g_Lexer.iIndex1 >= strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceLine]))
                break;

            // 移到直到遇到分隔符
            if (IsCharDelimiter(g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex1]))
                break;
            g_Lexer.iIndex1++;
        }
    }

    // 处理单个字符的情况
    if (g_Lexer.iIndex1 - g_Lexer.iIndex0 == 0)
        g_Lexer.iIndex1++;

    // Index0 和 Index1 之间就是 token 了
    unsigned int iCurrDestIndex = 0;
    for (unsigned int iCurrSourceIndex = g_Lexer.iIndex0; iCurrSourceIndex < g_Lexer.iIndex1; ++iCurrSourceIndex) {

        // 只复制反斜线之后的字符
        if (LEX_STATE_IN_STRING == g_Lexer.iCurrLexState) {
            if ('\\' == g_ppstrSourceCode[g_Lexer.iCurrSourceLine][iCurrSourceIndex])
                iCurrSourceIndex++;
        }
        g_Lexer.pstrCurrLexeme[iCurrDestIndex] = g_ppstrSourceCode[g_Lexer.iCurrSourceLine][iCurrSourceIndex];
        iCurrSourceIndex++;
    }
    g_Lexer.pstrCurrLexeme[iCurrDestIndex] = '\0';

    // 如果不是字符串，则转大写
    if (g_Lexer.iCurrLexState != LEX_STATE_IN_STRING)
        strupr(g_Lexer.pstrCurrLexeme);

    g_Lexer.CurrToken = TOKEN_TYPE_INVALID;

    // 是否是 字符串 类型
    if (strlen(g_Lexer.pstrCurrLexeme) > 1
        || g_Lexer.pstrCurrLexeme[0] != '"') {
        if (LEX_STATE_IN_STRING == g_Lexer.iCurrLexState) {
            g_Lexer.CurrToken = TOKEN_TYPE_STRING;
            return TOKEN_TYPE_STRING;
        }
    }

    // 判断单个字符的 token
    if (strlen(g_Lexer.pstrCurrLexeme) == 1) {
        switch (g_Lexer.pstrCurrLexeme[0]) {
            case '"': {
                switch (g_Lexer.iCurrLexState) {
                    case LEX_STATE_NO_STRING:
                        // 如果不是在分析字符串,则告诉词法分析器当前处于字符串中
                        g_Lexer.iCurrLexState = LEX_STATE_IN_STRING;
                        break;
                    case LEX_STATE_IN_STRING:
                        // 如果是在分析字符串,则字符串结束
                        g_Lexer.iCurrLexState = LEX_STATE_END_STRING;
                        break;
                    default:
                        break;
                }
                g_Lexer.CurrToken = TOKEN_TYPE_QUOTE;
                break;
            }
            case ',': {
                g_Lexer.CurrToken = TOKEN_TYPE_COMMA;
                break;
            }
            case ':': {
                g_Lexer.CurrToken = TOKEN_TYPE_COLON;
                break;
            }
            case '[': {
                g_Lexer.CurrToken = TOKEN_TYPE_OPEN_BRACKET;
                break;
            }
            case ']': {
                g_Lexer.CurrToken = TOKEN_TYPE_CLOSE_BRACKET;
                break;
            }
            case '{': {
                g_Lexer.CurrToken = TOKEN_TYPE_OPEN_BRACE;
                break;
            }
            case '}': {
                g_Lexer.CurrToken = TOKEN_TYPE_CLOSE_BRACE;
                break;
            }
            case '\n': {
                g_Lexer.CurrToken = TOKEN_TYPE_NEWLINE;
                break;
            }
            default:
                break;
        }
    }

    // 现在检查多字符 token
    // int ?
    if (IsStringInteger(g_Lexer.pstrCurrLexeme))
        g_Lexer.CurrToken = TOKEN_TYPE_INT;
    // float?
    if (IsStringFloat(g_Lexer.pstrCurrLexeme))
        g_Lexer.CurrToken = TOKEN_TYPE_FLOAT;

    // ident?
    if (IsStringIdent(g_Lexer.pstrCurrLexeme))
        g_Lexer.CurrToken = TOKEN_TYPE_IDENT;

    // 是否是设置属性的关键字

    // SetStackSize
    if (strcmp(g_Lexer.pstrCurrLexeme, "SETSTACKSIZE") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_SETSTACKSIZE;

    // Var/Var[]
    if (strcmp(g_Lexer.pstrCurrLexeme, "VAR") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_VAR;

    // Func
    if (strcmp(g_Lexer.pstrCurrLexeme, "FUNC") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_FUNC;

    // Param
    if (strcmp(g_Lexer.pstrCurrLexeme, "PARAM") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_PARAM;

    // RetVal
    if (strcmp(g_Lexer.pstrCurrLexeme, "_RETVAL") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_REG_RETVAL;

    // 是否是指令
    InstrLookup instr;
    if (GetInstrByMnemonic(g_Lexer.pstrCurrLexeme, &instr))
        g_Lexer.CurrToken = TOKEN_TYPE_INSTR;
    return g_Lexer.CurrToken;
}

char *GetCurrLexeme() {

    return NULL;
}

char GetLookAheadChar() {
    return 0;
}

int SkipToNextLine() {
    return 0;
}
