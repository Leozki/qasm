//
// Created by Leozki on 2017/6/2.
//

#ifndef QASM_INSTRLOOKUPTABLE_H
#define QASM_INSTRLOOKUPTABLE_H

#include "Constants.h"

typedef int OpTypes;

typedef struct _InstrLookup
{
    char pstrMnemonic [ MAX_INSTR_MNEMONIC_SIZE ];
    int iOpcode;
    int iOpCount;
    OpTypes * OpList;
} InstrLookup;

void InitInstrTable();

InstrLookup g_InstrTable [ MAX_INSTR_LOOKUP_COUNT ];



#endif //QASM_INSTRLOOKUPTABLE_H

