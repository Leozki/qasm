//
// Created by Leozki on 2017/6/5.
//

#ifndef QASM_SOURCELOADER_H
#define QASM_SOURCELOADER_H

#include "Constants.h"

extern char g_pstrSourceFilename[MAX_FILENAME_SIZE];
extern char g_pstrExecFilename[MAX_FILENAME_SIZE];
extern char **g_ppstrSourceCode;
extern int g_iSourceCodeSize;
extern FILE *g_pSourceFile;

void LoadSourceFile();

#endif //QASM_SOURCELOADER_H
