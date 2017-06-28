#include <stdio.h>
#include "Constants.h"
#include "LinkedList.h"
#include "InstrLookupTable.h"
#include "SymbolTable.h"
#include "LabelTable.h"
#include "FuncTable.h"
#include "StringTable.h"
#include "StringUtils.h"

char **g_ppstrSourceCode = NULL;
int g_iSourceCodeSize;                          // 源文件的行数
FILE *g_pSourceFile = NULL;

char g_pstrSourceFilename[MAX_FILENAME_SIZE];
char g_pstrExecFilename[MAX_FILENAME_SIZE];


void Init() {
    // 初始化指令表
    InitInstrTable();

    // 初始化其他表
    InitLinkedList(&g_symbolTable);
    InitLinkedList(&g_LabelTable);
    InitLinkedList(&g_funcTable);
    InitLinkedList(&g_stringTable);

}

// 加载源文件
void LoadSourceFile() {

    if (!(g_pSourceFile = fopen(g_pstrSourceFilename, "rb")))
        ExitOnError("无法打开源文件");

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
        ExitOnError("无法打开源文件");

    if (!(g_ppstrSourceCode = (char **) malloc(g_iSourceCodeSize * sizeof(char *))))
        ExitOnError("申请源文件大小内存空间失败");

    // 开始读取源码

    for (int iCurrLineIndex = 0; iCurrLineIndex < g_iSourceCodeSize; ++iCurrLineIndex) {
        if (!(g_ppstrSourceCode[iCurrLineIndex] = (char *) malloc(MAX_SOURCE_LINE_SIZE + 1)))
            ExitOnError("无法申请一行源代码的空间");

        // 读取当前行
        fgets(g_ppstrSourceCode[iCurrLineIndex], MAX_SOURCE_LINE_SIZE, g_pSourceFile);

        // 清除注释
        StripComments(g_ppstrSourceCode[iCurrLineIndex]);
        // 清除空白符号
        TrimWhitespace(g_ppstrSourceCode[iCurrLineIndex]);

        // 添加换行符
        int iNewLineIndex = strlen(g_ppstrSourceCode[iCurrLineIndex]) - 1;
        if (g_ppstrSourceCode[iCurrLineIndex][iNewLineIndex] != '\n') {
            g_ppstrSourceCode[iCurrLineIndex][iNewLineIndex + 1] = '\n';
            g_ppstrSourceCode[iCurrLineIndex][iNewLineIndex + 2] = '\0';
        }
    }

    fclose(g_pSourceFile);
}


int main(int argc, char *argv[]) {
    if (argc < 2) return 0;

    // 全局保存源文件的文件名,并转换为大写
    strcmp(g_pstrSourceFilename, argv[1]);
    strupr(g_pstrSourceFilename);

    // 检查后缀名是否是xvm
    if (!strstr(g_pstrSourceFilename, SOURCE_FILE_EXT)) {
        // 不是xvm后缀的，补上
        strcat(g_pstrSourceFilename, SOURCE_FILE_EXT);
    }

    if (argv[2]) {
        strcpy(g_pstrExecFilename, argv[2]);
        strupr(g_pstrExecFilename);

        if (!strstr(g_pstrExecFilename, EXEC_FILE_EXT)) {
            strcat(g_pstrExecFilename, EXEC_FILE_EXT);
        }
    } else {
        // 如果没有提供输出的文件名称，则以源文件的文件名为名字
        int iExeOffset = (int) (strrchr(g_pstrSourceFilename, '.') - g_pstrSourceFilename);
        strncpy(g_pstrExecFilename, g_pstrSourceFilename, (size_t) iExeOffset);

        g_pstrExecFilename[iExeOffset] = '\0';

        strcat(g_pstrExecFilename, EXEC_FILE_EXT);
    }

    // 初始化 assembler
    Init();

    // 加载源文件
    LoadSourceFile();

    printf("正在处理中:%s...\n\n", g_pstrSourceFilename);

    
    return 0;
}