//
// Created by Leozki on 2017/6/28.
//

#ifndef XASM_LEXICALANALYZER_H
#define XASM_LEXICALANALYZER_H

#include "Constants.h"

#define MAX_LEXEME_SIZE             256         // Maximum lexeme size




typedef int Token;                              // Tokenizer alias type

typedef struct _Lexer                           // The lexical analyzer/tokenizer
{
    int iCurrSourceLine;                        // Current line in the source
    unsigned int iIndex0,                       // Indices into the string
            iIndex1;
    Token CurrToken;                            // Current token
    char pstrCurrLexeme[MAX_LEXEME_SIZE];    // Current lexeme
    int iCurrLexState;                          // The current lex state
} Lexer;

void ResetLexer();

Token GetNextToken();

char *GetCurrLexeme();

char GetLookAheadChar();

int SkipToNextLine();

Lexer g_Lexer;

#endif //XASM_LEXICALANALYZER_H
