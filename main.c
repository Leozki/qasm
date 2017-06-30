#include <stdio.h>
#include "Constants.h"
#include "LinkedList.h"
#include "InstrLookupTable.h"
#include "SymbolTable.h"
#include "LabelTable.h"
#include "FuncTable.h"
#include "StringTable.h"
#include "SourceLoader.h"
#include "Assembler.h"

void Init() {
    // 初始化指令表
    InitInstrTable();
    // 初始化其他表
    InitLinkedList(&g_symbolTable);
    InitLinkedList(&g_LabelTable);
    InitLinkedList(&g_funcTable);
    InitLinkedList(&g_stringTable);
}


int main(int argc, char *argv[]) {
    if (argc < 2) return 0;

    // 全局保存源文件的文件名,并转换为大写
    strcpy(g_pstrSourceFilename, argv[1]);
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

    printf("Begin process:%s...\n\n", g_pstrSourceFilename);

    // 编译处理源文件
    AssmblSourceFile();

    return 0;
}