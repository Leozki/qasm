//
// Created by Leozki on 2017/6/28.
//

#ifndef XASM_ERROR_H
#define XASM_ERROR_H

#define ERROR_MSSG_INVALID_INPUT	\
			"无效的输入字符"

#define ERROR_MSSG_LOCAL_SETSTACKSIZE    \
            "SetStackSize 只能在全局作用域出现"

#define ERROR_MSSG_INVALID_STACK_SIZE    \
            "无效的 stack 大小"

#define ERROR_MSSG_MULTIPLE_SETSTACKSIZES    \
            "SetStackSize 只能执行一次"

#define ERROR_MSSG_LOCAL_SETPRIORITY   \
            "SetPriority 只能在全局作用域出现"

#define ERROR_MSSG_INVALID_PRIORITY    \
            "无效的 priority"

#define ERROR_MSSG_MULTIPLE_SETPRIORITIES   \
            "SetPriority 只能执行一次"

#define ERROR_MSSG_IDENT_EXPECTED    \
            "Identifier expected"

#define ERROR_MSSG_INVALID_ARRAY_SIZE    \
            "无效的数组大小"

#define ERROR_MSSG_IDENT_REDEFINITION    \
            "Identifier 重定义"

#define ERROR_MSSG_UNDEFINED_IDENT    \
            "未定义的 identifier"

#define ERROR_MSSG_NESTED_FUNC    \
            "嵌套的函数 "

#define ERROR_MSSG_FUNC_REDEFINITION    \
            "函数重定义"

#define ERROR_MSSG_UNDEFINED_FUNC    \
            "未定义的函数"

#define ERROR_MSSG_GLOBAL_PARAM    \
            "参数只能出现在函数旁边"

#define ERROR_MSSG_MAIN_PARAM    \
            "_Main () 函数不能接收参数"

#define ERROR_MSSG_GLOBAL_INSTR	\
			"Instructions 只能出现在函数旁边"

#define ERROR_MSSG_GLOBAL_LINE_LABEL	\
			"行标签 functions"

#define ERROR_MSSG_LINE_LABEL_REDEFINITION	\
			"行标签 重定义"

#define ERROR_MSSG_UNDEFINED_LINE_LABEL	\
			"未定义的行标签"

#define ERROR_MSSG_INVALID_INSTR	\
			"无效的 instruction"

void ExitOnCodeError(char *pstrErrorMssg);

void ExitOnCharExpectedError(char cChar);

#endif //XASM_ERROR_H
