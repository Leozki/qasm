//
// Created by Leozki on 2017/6/4.
//

#include "StringUtils.h"
#include "Constants.h"

int IsCharWhitespace(char cChar) {
    if (cChar == ' ' || cChar == '\t')
        return TRUE;
    else
        return FALSE;
}

int IsCharNumeric(char cChar) {

    if (cChar >= '0' && cChar <= '9')
        return TRUE;
    else
        return FALSE;
}

int IsCharIdent(char cChar) {

    if ((cChar >= '0' && cChar <= '9') ||
        (cChar >= 'A' && cChar <= 'Z') ||
        (cChar >= 'a' && cChar <= 'z') ||
        cChar == '_')
        return TRUE;
    else
        return FALSE;
}

int IsCharDelimiter(char cChar) {

    if (cChar == ':' || cChar == ',' || cChar == '"' ||
        cChar == '[' || cChar == ']' ||
        cChar == '{' || cChar == '}' ||
        IsCharWhitespace(cChar) || cChar == '\n')
        return TRUE;
    else
        return FALSE;
}

void TrimWhitespace(char *pstrString) {
    unsigned int iStringLength = strlen(pstrString);
    unsigned int iPadLength;
    unsigned int iCurrCharIndex;

    if (iStringLength > 1) {

        for (iCurrCharIndex = 0; iCurrCharIndex < iStringLength; ++iCurrCharIndex)
            if (!IsCharWhitespace(pstrString[iCurrCharIndex]))
                break;


        iPadLength = iCurrCharIndex;
        if (iPadLength) {
            for (iCurrCharIndex = iPadLength; iCurrCharIndex < iStringLength; ++iCurrCharIndex)
                pstrString[iCurrCharIndex - iPadLength] = pstrString[iCurrCharIndex];

            for (iCurrCharIndex = iStringLength - iPadLength; iCurrCharIndex < iStringLength; ++iCurrCharIndex)
                pstrString[iCurrCharIndex] = ' ';
        }

        for (iCurrCharIndex = iStringLength - 1; iCurrCharIndex > 0; --iCurrCharIndex) {
            if (!IsCharWhitespace(pstrString[iCurrCharIndex])) {
                pstrString[iCurrCharIndex + 1] = '\0';
                break;
            }
        }
    }
}

int IsStringWhitespace(char *pstrString) {
    if (!pstrString)
        return FALSE;

    if (strlen(pstrString) == 0)
        return TRUE;

    for (unsigned int iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
        if (!IsCharWhitespace(pstrString[iCurrCharIndex]) && pstrString[iCurrCharIndex] != '\n')
            return FALSE;


    return TRUE;
}

int IsStringIdent(char *pstrString) {

    if (!pstrString)
        return FALSE;

    if (strlen(pstrString) == 0)
        return FALSE;

    if (pstrString[0] >= '0' && pstrString[0] <= '9')
        return FALSE;


    for (unsigned int iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
        if (!IsCharIdent(pstrString[iCurrCharIndex]))
            return FALSE;


    return TRUE;
}


int IsStringInteger(char *pstrString) {

    if (!pstrString)
        return FALSE;


    if (strlen(pstrString) == 0)
        return FALSE;

    unsigned int iCurrCharIndex;


    for (iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
        if (!IsCharNumeric(pstrString[iCurrCharIndex]) && !(pstrString[iCurrCharIndex] == '-'))
            return FALSE;


    for (iCurrCharIndex = 1; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
        if (pstrString[iCurrCharIndex] == '-')
            return FALSE;

    return TRUE;
}


int IsStringFloat(char *pstrString) {
    if (!pstrString)
        return FALSE;

    if (strlen(pstrString) == 0)
        return FALSE;


    unsigned int iCurrCharIndex;

    for (iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
        if (!IsCharNumeric(pstrString[iCurrCharIndex]) && !(pstrString[iCurrCharIndex] == '.') &&
            !(pstrString[iCurrCharIndex] == '-'))
            return FALSE;


    int iRadixPointFound = 0;

    for (iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
        if (pstrString[iCurrCharIndex] == '.')
            if (iRadixPointFound)
                return FALSE;
            else
                iRadixPointFound = 1;

    for (iCurrCharIndex = 1; iCurrCharIndex < strlen(pstrString); ++iCurrCharIndex)
        if (pstrString[iCurrCharIndex] == '-')
            return FALSE;

    if (iRadixPointFound)
        return TRUE;
    else
        return FALSE;
}