//
// Created by Leozki on 2017/6/5.
//

#include "Constants.h"

void Exit() {
    exit(0);
}

void ExitOnError(char *pstrErrorMssg) {
    printf("Fatal Error: %s.\n", pstrErrorMssg);
    Exit();
}