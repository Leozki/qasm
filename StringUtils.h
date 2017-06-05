//
// Created by Leozki on 2017/6/4.
//

#ifndef QASM_STRINGUTILS_H
#define QASM_STRINGUTILS_H

int IsCharWhitespace(char cChar);

int IsCharNumeric(char cChar);

int IsCharIdent(char cChar);

int IsCharDelimiter(char cChar);

void TrimWhitespace(char *pstrString);

int IsStringWhitespace(char *pstrString);

int IsStringIdent(char *pstrString);

int IsStringInteger(char *pstrString);

int IsStringFloat(char *pstrString);

void StripComments ( char * pstrSourceLine );
#endif //QASM_STRINGUTILS_H
