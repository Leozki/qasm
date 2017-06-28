//
// Created by Leozki on 2017/6/28.
//

#include "LexicalAnalyzer.h"

void ResetLexer() {
    g_Lexer.iCurrSourceLine = 0;

    g_Lexer.iIndex0 = 0;
    g_Lexer.iIndex1 = 0;

    g_Lexer.CurrToken = TOKEN_TYPE_INVALID;

    g_Lexer.iCurrLexState = LEX_STATE_NO_STRING;
}
