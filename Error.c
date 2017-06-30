//
// Created by Leozki on 2017/6/28.
//

#include "Error.h"
#include "stdio.h"
#include "LexicalAnalyzer.h"
#include "SourceLoader.h"


void ExitOnCodeError(char *pstrErrorMssg) {
    printf("msg: %s\n", pstrErrorMssg);
    printf("line: %d\n", g_Lexer.iCurrSourceLine);

    char pstrSourceLine[MAX_SOURCE_LINE_SIZE];
    strcpy(pstrSourceLine, g_ppstrSourceCode[g_Lexer.iCurrSourceLine]);

    for (unsigned int iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrSourceLine); ++iCurrCharIndex)
        if (pstrSourceLine[iCurrCharIndex] == '\t')
            pstrSourceLine[iCurrCharIndex] = ' ';
    printf("%s", pstrSourceLine);

    for (unsigned int iCurrSpace = 0; iCurrSpace < g_Lexer.iIndex0; ++iCurrSpace)
        printf(" ");
    printf("^\n");

    printf("Compile Failed,file=\"%s\"\n", g_pstrExecFilename);


    Exit();
}


void ExitOnCharExpectedError(char cChar) {
    char *pstrErrorMssg = (char *) malloc(strlen("' ' expected"));
    sprintf(pstrErrorMssg, "'%c' expected", cChar);
    ExitOnCodeError(pstrErrorMssg);
}
