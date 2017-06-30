//
// Created by Leozki on 2017/6/5.
//

#include "SourceLoader.h"
#include "StringUtils.h"

char g_pstrSourceFilename[MAX_FILENAME_SIZE];
char g_pstrExecFilename[MAX_FILENAME_SIZE];

char **g_ppstrSourceCode = NULL;
int g_iSourceCodeSize;

FILE *g_pSourceFile = NULL;

// 加载源文件
void LoadSourceFile() {

    if (!(g_pSourceFile = fopen(g_pstrSourceFilename, "rb")))
        ExitOnError("Can't Open SourceFile");

    // 计算源文件有多少行
    while (!feof(g_pSourceFile)) {
        if (fgetc(g_pSourceFile) == '\n') {
            g_iSourceCodeSize++;
        }
    }
    g_iSourceCodeSize++;

    fclose(g_pSourceFile);

    // 再以 ASCII 模式遍历一遍
    if (!(g_pSourceFile = fopen(g_pstrSourceFilename, "r")))
        ExitOnError("Can't Open SourceFile");

    if (!(g_ppstrSourceCode = (char **) malloc(g_iSourceCodeSize * sizeof(char *))))
        ExitOnError("Alloc Space Failed");

    // 开始读取源码

    for (int iCurrLineIndex = 0; iCurrLineIndex < g_iSourceCodeSize; ++iCurrLineIndex) {
        if (!(g_ppstrSourceCode[iCurrLineIndex] = (char *) malloc(MAX_SOURCE_LINE_SIZE + 1)))
            ExitOnError("Alloc Line Space Failed");

        // 读取当前行
        fgets(g_ppstrSourceCode[iCurrLineIndex], MAX_SOURCE_LINE_SIZE, g_pSourceFile);

        // 清除注释
        StripComments(g_ppstrSourceCode[iCurrLineIndex]);
        // 清除空白符号
        TrimWhitespace(g_ppstrSourceCode[iCurrLineIndex]);

        // 强制在每一行结尾加入换行符, 以便解析出一个换行TOKEN
        int iNewLineIndex = strlen(g_ppstrSourceCode[iCurrLineIndex]) - 1;
        if (g_ppstrSourceCode[iCurrLineIndex][iNewLineIndex] != '\n') {
            g_ppstrSourceCode[iCurrLineIndex][iNewLineIndex + 1] = '\n';
            g_ppstrSourceCode[iCurrLineIndex][iNewLineIndex + 2] = '\0';
        }
    }

    printf("LoadSourceFile \"%s\" completed,total lines =  %d\n\n", g_pstrSourceFilename, g_iSourceCodeSize);
    fclose(g_pSourceFile);
}