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

// 文件
#define MAX_FILENAME_SIZE           2048
#define SOURCE_FILE_EXT             ".XASM"
#define EXEC_FILE_EXT               ".XSE"


// 源码
#define MAX_SOURCE_CODE_SIZE        65536
#define MAX_SOURCE_LINE_SIZE        4096

#define MAX_INSTR_LOOKUP_COUNT      256
#define MAX_INSTR_MNEMONIC_SIZE     16

// ------------------------------------------------
// 词法分析器相关定义
// ------------------------------------------------
#define LEX_STATE_NO_STRING         0
#define LEX_STATE_IN_STRING         1
#define LEX_STATE_END_STRING        2

#define TOKEN_TYPE_INT              0
#define TOKEN_TYPE_FLOAT            1
#define TOKEN_TYPE_STRING           2
#define TOKEN_TYPE_QUOTE            3
#define TOKEN_TYPE_IDENT            4
#define TOKEN_TYPE_COLON            5
#define TOKEN_TYPE_OPEN_BRACKET     6
#define TOKEN_TYPE_CLOSE_BRACKET    7
#define TOKEN_TYPE_COMMA            8
#define TOKEN_TYPE_OPEN_BRACE       9
#define TOKEN_TYPE_CLOSE_BRACE      10
#define TOKEN_TYPE_NEWLINE          11

#define TOKEN_TYPE_INSTR            12

#define TOKEN_TYPE_SETSTACKSIZE     13
#define TOKEN_TYPE_SETPRIORITY      14
#define TOKEN_TYPE_VAR              15
#define TOKEN_TYPE_FUNC             16
#define TOKEN_TYPE_PARAM            17
#define TOKEN_TYPE_REG_RETVAL       18

#define TOKEN_TYPE_INVALID          19
#define END_OF_TOKEN_STREAM         20

// 标签最大长度
#define MAX_IDENT_SIZE 256

// ------------------------------------------------
// 线程优先级定义
// ------------------------------------------------
#define PRIORITY_USER               0           // 用户自定义
#define PRIORITY_LOW                1
#define PRIORITY_MED                2
#define PRIORITY_HIGH               3

#define PRIORITY_LOW_KEYWORD        "Low"
#define PRIORITY_MED_KEYWORD        "Med"
#define PRIORITY_HIGH_KEYWORD       "High"

#define MAIN_FUNC_NAME                "_MAIN"        // main函数的预定义

void ExitOnError(char *pstrErrorMssg);

void Exit();

#endif //QASM_CONSTANTS_H
