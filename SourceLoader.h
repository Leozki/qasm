//
// Created by Leozki on 2017/6/5.
//

#ifndef QASM_SOURCELOADER_H
#define QASM_SOURCELOADER_H
#include "Constants.h"

char ** g_ppstrSourceCode = NULL;
int g_iSourceCodeSize;

FILE * g_pSourceFile = NULL;

char g_pstrSourceFilename [ MAX_FILENAME_SIZE ],	// Source code filename
        g_pstrExecFilename [ MAX_FILENAME_SIZE ];	// Executable filename

void LoadSourceFile();

#endif //QASM_SOURCELOADER_H
