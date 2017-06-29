//
// Created by Leozki on 2017/6/2.
//

#include "InstrLookupTable.h"



void SetOpType(int iInstrIndex, int iOpIndex, OpTypes iOpType) {
    g_InstrTable [ iInstrIndex ].OpList [ iOpIndex ] = iOpType;
}

void InitInstrTable() {

}




int GetInstrByMnemonic(char *pstrMnemonic, InstrLookup *pInstr) {
    for (int iCurrInstrIndex = 0; iCurrInstrIndex < MAX_INSTR_LOOKUP_COUNT; ++iCurrInstrIndex)
        if (strcmp(g_InstrTable[iCurrInstrIndex].pstrMnemonic, pstrMnemonic) == 0) {
            *pInstr = g_InstrTable[iCurrInstrIndex];
            return TRUE;
        }
    return FALSE;
}
