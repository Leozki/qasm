//
// Created by Leozki on 2017/6/2.
//

#ifndef QASM_CONSTANTS_H
#define QASM_CONSTANTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// 标识符最大长度
#define MAX_IDENT_SIZE 256


// 文件
#define MAX_FILENAME_SIZE           2048
#define SOURCE_FILE_EXT             ".XASM"
#define EXEC_FILE_EXT               ".XSE"


// 源码
#define MAX_SOURCE_CODE_SIZE        65536
#define MAX_SOURCE_LINE_SIZE        4096


void ExitOnError(char *pstrErrorMssg);
void Exit();

#endif //QASM_CONSTANTS_H
